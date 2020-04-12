/******************************************************************
 light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 - Brightness changes the "on" level
   If a light is already on, it's brightness is changed.
 - Switch turns a light off, or on to the currently set brightness level
 - setPercent immediately sets the light level without affecting saved brightness setting
 - When localPin changes,
   - If local mode, light is toggled between 0 and 100 percent
   - In automatic mode, MQTT event is sent for switch name
 - Currently, local mode is set always

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2019-11-09: v3.1.0 Add local pin
 2019-01-03: v3.0.0 Assume use of Backup SRAM to persist percent across resets.
 2017-10-28: Convert to v2.
 2017-05-19: Extract to separate plugin library
 2017-05-15: Make devices generic
 2017-03-24: Rename Patriot
 2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#include "PatriotLight.h"

#define kDebounceDelay 50

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, bool isInverted, bool forceDigital)
        : Device(name, DeviceType::Light),
          _pin(pinNum),
          _isInverted(isInverted),
          _forceDigital(forceDigital)
{
    _localMode                = true;
    _localPinNum              = 0;          // 0 = none
    _localPinName             = "unnamed";
    _localPinActiveHigh       = false;
    _lastReadTime             = 0;

    _brightness               = 100;    // Brightness defaults to 100
    _dimmingDuration          = isPwmSupported() ? 2.0 : 0;
    _targetPercent            = 0;
    _currentPercent           = 0.0;
    _incrementPerMillisecond  = 0.0;
    _lastUpdateTime           = 0;
    pinMode(pinNum, OUTPUT);
    outputPWM();                        // Set initial state to persisted value
}

/**
 * Set Local pin
 * @param pinNum Int pin number
 * @param pinName String name of pin
 * @param activeHigh Bool set if high when On (normally low)
 */
void Light::setLocalPin(int pinNum, String pinName, bool activeHigh) {
    _localPinNum = pinNum;
    _localPinName = pinName;
    _localPinActiveHigh = activeHigh;
    pinMode(pinNum, INPUT_PULLUP);
}

/**
 * Set percent
 * @param percent Int 0 to 100
 */
void Light::setPercent(int percent) {
    changePercent(percent);
}

/**
 * Get percent
 * @return Int current 0-100 percent value
 */
int Light::getPercent() {
    return _percent;
}

/**
 * Set brightness
 * @param percent Int 0 to 100
 */
void Light::setBrightness(int percent) {
    _brightness = percent;
    if(_percent == 0) return;       // All done if light is off
    if(_percent != _brightness) {   // If on and brightness changed, do it
        changePercent(_brightness);
    }
}

/**
 * Get brightness - leave default Device implementation
 */
 int Light::getBrightness() {
     return _brightness;
 }

 /**
  * Set switch
  * @param percent Int 0 == off, >0 = on
  */
 void Light::setSwitch(int percent) {       //TODO: change to bool
     if(percent > 0) setOn();
     else changePercent(0);
 }

 /**
  * Get switch
  * @return Int 0 == off, 100 == on
  */
 int Light::getSwitch() {
     if(_targetPercent > 0) return 100;
     return 0;
 }


/**
 * Set On
 */
void Light::setOn() {
    if(isAlreadyOn()) return;
    changePercent(_brightness);
}

/**
 * Change percent
 * @param percent Int new percent value
 */
void Light::changePercent(int percent) {
    if(_targetPercent == percent) return;

    _targetPercent = percent;
    if(_dimmingDuration == 0.0 || isPwmSupported() == false) {
        _percent = percent;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Is already on?
 * @return bool true if light is on
 */
bool Light::isAlreadyOn() {
    return _targetPercent > 0;
}

/**
 * Is already off?
 * @return bool true if light is off
 */
bool Light::isAlreadyOff() {
    return _targetPercent == 0;
}

/**
 * Start smooth dimming
 * Use float _currentPercent value to smoothly transition
 * An alternative approach would be to calculate # msecs per step
 */
void Light::startSmoothDimming() {
    if((int)_percent != _targetPercent){
        _currentPercent = _percent;
        _lastUpdateTime = millis();
        float delta = _targetPercent - _percent;
        _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
    }
}

/**
 * Set light off
 */
void Light::setOff() {
    if(isAlreadyOff()) return;
    setPercent(0);
}

/**
 * Is light on?
 * @return bool true if light is on
 */
bool Light::isOn() {
    return !isOff();
}

/**
 * Is light off?
 * @return bool true if light is off
 */
bool Light::isOff() {
    return _targetPercent == 0;
}

/**
 * Set dimming duration
 * This will only affect any future transitions
 * @param duration float number of seconds
 */
void Light::setDimmingDuration(float duration) {
    _dimmingDuration = duration;
}

/**
 * Get dimming duration
 * @return float number of dimming duration seconds
 */
float Light::getDimmingDuration() {
    return _dimmingDuration;
}

/**
 * Private Methods
 */

/**
 * loop()
 * Called periodically to perform dimming, polling, etc.
 */
void Light::loop()
{
    // Poll switch
    if (isTimeToCheckSwitch())
    {
        if (didSwitchChange())
        {
            //TODO: Set light on/off as a result of using switch
            //      This overrides stuck commands, etc.
            //      May want to provide an optional override and/or dimming later
            if(_switchState == _localPinActiveHigh) {   // Is turning ON?
                changePercent(0);
            } else {
                changePercent(100);
            }
        }
    }

    // Is fading transition underway?
    if(_percent == _targetPercent) {
        // Nothing to do.
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentPercent += _incrementPerMillisecond * millisSinceLastUpdate;
    _percent = _currentPercent;
    if(_incrementPerMillisecond > 0) {
        if(_currentPercent > _targetPercent) {
            _percent = _targetPercent;
        }
    } else {
        if(_currentPercent < _targetPercent) {
            _percent = _targetPercent;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * isTimeToCheckSwitch()
 * @return bool if enough time has elapsed to sample switch again
 */
bool Light::isTimeToCheckSwitch()
{
    long currentTime = millis();
    if (currentTime < _lastReadTime + kDebounceDelay)
    {
        return false;
    }
    _lastReadTime = currentTime;
    return true;
}

/**
 * didSwitchChange()
 * @return bool if switch has changed since last reading
 */
bool Light::didSwitchChange()
{
    bool newState = digitalRead(_localPinNum) == 0;
    if (newState == _switchState)
    {
        return false;
    }
    _switchState = newState;
    return true;
}

/**
 * Set the output PWM value (0-255) based on 0-100 percent value
 */
void Light::outputPWM() {
    if(isPwmSupported()) {
        float pwm = _percent;
        pwm *= 255.0;
        pwm /= 100.0;
        analogWrite(_pin, (int) pwm);
    } else {
        bool isOn = _percent > 49;
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        digitalWrite(_pin, isHigh ? HIGH : LOW);
    }
}

/**
 * Is PWM pin?
 * @param pin number
 * @return bool true if pin supports PWM
 */
bool Light::isPwmSupported()
{
    switch(_pin) {
        case D0:
        case D1:
        case D2:
        case D3:
        case A4:
        case A5:
        case A7:    // aka WKP
        case RX:
        case TX:
            return _forceDigital ? FALSE : TRUE;
        default:
            break;
    };
    return FALSE;
}

void Light::log(String message)
{
    IoT::log("Light: " + message);
}
