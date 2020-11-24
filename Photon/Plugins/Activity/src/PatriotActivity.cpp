/******************************************************************
Activity plugin

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

#include "PatriotActivity.h"

/**
 * Constructor
 */
Activity::Activity(String name) : Device(name)
{
    // Nothing else to do
}