/******************************************************************
Patriot Activity plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-23: Initial version based on PseudoDevice
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

class Activity : public Device
{
public:
    /**
     * Constructor
     */
    Activity(String name);

};