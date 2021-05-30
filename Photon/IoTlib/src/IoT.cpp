/**
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#include "IoT.h"

// Static Variables
Device*      Device::_devices = NULL;
PubSub* IoT::_pubSub = NULL;

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 */
void IoT::begin(String controllerName)
{
    _pubSub = new PubSub(controllerName);
    
    //TODO: Move to PubSub
    // Subscribe to events. There is a 1/second limit for events.
    Particle.subscribe(kPublishName, IoT::subscribeHandler, MY_DEVICES);

    // Expose particle.io variables
    Device::expose();
}

void IoT::publish(String topic, String message)
{
    if (_pubSub != NULL) {
        _pubSub->publish(topic, message);
    }
}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    Device::loopAll();

    if (_pubSub != NULL) {
        _pubSub->loop();
    }
}

/**
 * Particle.io Subscribe Handler
 * t:patriot m:<device>:<value>
 * This method handles commands from Alexa
 */
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data = String(rawData).trim();
    String event(eventName);
    
    Log.info("Particle.io subscribe received data: '"+event+"', '"+data+"'");
    
    if (_pubSub != NULL) {
        _pubSub->parseMessage(event.toLowerCase(), data.toLowerCase());
    }
}

int IoT::handleLightSwitch(String name) {
    int lightSwitch = Device::getChangedValue(name+"Switch");
    if( lightSwitch == -1) return -1;
    Log.info("handleLightSwitch hasChanged: %d",lightSwitch);
    Device *device = Device::get(name);
    if( lightSwitch > 0 ) {
        device->saveRestoreValue();
        device->setValue(100);
    } else {
        device->restoreSavedValue();
    }
    return lightSwitch;
}

/**
 publishValue()
 param: name of state
 param: value to assign state
 return: 0 success, -1 MQTT error
 */
int IoT::publishValue(String name, int value) {
    if(_pubSub != NULL) {
        _pubSub->publish("patriot/" + name, String(value));
        return 0;
    }
    return -1;
}
