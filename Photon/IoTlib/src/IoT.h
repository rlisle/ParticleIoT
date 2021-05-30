/**
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and distributes them to devices.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/
#pragma once

#include "Particle.h"
#include "constants.h"
#include "device.h"
#include "PubSub.h"

// Do NOT use 0 for this type value
// because 0 will invoke previous restoration

// PartOfDay
#define SUNRISE 1
#define MORNING 2
#define NOON 3
#define AFTERNOON 4
#define SUNSET 5
#define DUSK 6
#define NIGHT 7
#define DAWN 8

// Sleeping
#define AWAKE 1
#define RETIRING 2
#define ASLEEP 3

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 */
class IoT {

    friend PubSub;

public:

    /**
     * begin(String controllerName)
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     **/
    static void begin(String controllerName);

    static void publish(String topic, String message);

    /**
     * Loop needs to be called periodically
     */
    static void loop();

    /**
    Values used by loop programming
     */
    static int handleLightSwitch(String name);        // Requires matching Light & LightSwitch type names
    
    static int publishValue(String name, int value);  // Sends particle.io PubSub message
    
private:
    static String _controllerName;
    static PubSub* _pubSub;

    static void subscribeHandler(const char *eventName, const char *rawData);
    static void periodicReset();
};
