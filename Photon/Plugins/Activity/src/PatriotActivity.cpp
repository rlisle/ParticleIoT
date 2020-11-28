/******************************************************************
Activity plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.
- By attaching behaviors, can be controlled by other states.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
 2020-11-23: Initial version based on PseudoDevice
******************************************************************/

#include "PatriotActivity.h"

/**
 * Constructor
 */
Activity::Activity(String name) : Device(name)
{
    _wasSetDirectly = false;
}

void Activity::setPercent(int percent) {
    _wasSetDirectly = true;
    _percent = percent;
};

void Activity::stateDidChange(States *states) {
    if(_wasSetDirectly) {
        _wasSetDirectly = false;
        return;
    }
    
    int newLevel = _behaviors.stateDidChange(states);
    log("Activity " + _name + " stateDidChange newLevel "+String(newLevel));
    if(newLevel != _percent) {
        log("Activity " + _name + " stateDidChange publishing newLevel "+String(newLevel));
        _percent = newLevel;
        String topic = "patriot/" + _name;
        String message = String(_percent);
        publish(topic,message);
    }
}


