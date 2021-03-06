/******************************************************************
motorized device control

This object extends Patriot to support motorized devices such as
 motorized vents or curtains. It is expected that devices will take
 a fixed amount of time to open or close. That duration is specified
 when the device is instantiated.

 For example, the roof vent on my RV takes about 15 seconds to go
 from closed to fully open. So the vent device will be instantiated
 as follows:

 Motorized *vent = Motorized(D3, D4, 15000, "Roof Vent")

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-09-17: Initial version based on fan plugin.
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

enum MotorizedState { inactive, opening, closing };

class Motorized : public Device
{
 public:
    Motorized(int8_t openPinNum, int8_t closePinNum, int8_t seconds, String name);
    void            setPulseMode(int8_t msecsX10);
    virtual void    setPercent(int percent);
    virtual void    loop();

 private:
    String  _name;
    int8_t  _openPinNum;
    int8_t  _closePinNum;
    int8_t  _durationSeconds;       // Seconds needed to fully open or close
    int8_t  _pulseDurationX10msecs; // Length of start/stop pulses in msecs

    // TODO: switch to enums
    int8_t      _state;             // 0=inactive, 1=opening, 2=closing
    int8_t      _mode;              // 0=hold on/off, 1=pulse on/off, pulse both to stop

    unsigned long   _stopTimeMsecs;     // Time to stop motor
    unsigned long   _pulseTimeMsecs;    // Time to end pulse (if mode == 1)

    unsigned long calcStopTime(int percent);
    void          turnOffMotor();
};
