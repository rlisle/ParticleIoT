/**
PubSub.cpp

This class handles all Particle.io PubSub interactions.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#include "PubSub.h"
#include "constants.h"
#include "device.h"
#include "IoT.h"

#define ALIVE_MSG_SECONDS 60 * 15

PubSub::PubSub(String controllerName)
{
    _controllerName = controllerName.toLowerCase();

    // We'll want to start with ALL whenever debugging new code.
    // Beware of Particle messages limits though (about 200k = 2/minute)
    // Use PubSub to switch to error when done testing or vs. a vs.
    _logLevel = LOG_LEVEL_ERROR;

    Time.zone(-6.0);
}

void PubSub::publish(String topic, String message) {
    Particle.publish(topic,message);
}

void PubSub::loop()
{
    sendAlivePeriodically();
}

void PubSub::sendAlivePeriodically() {
    system_tick_t secondsSinceLastAlive = Time.now() - _lastAliveTime;
    if(secondsSinceLastAlive > ALIVE_MSG_SECONDS) {
        _lastAliveTime = Time.now();
        publish("patriot/alive", _controllerName);   //TODO: add timestamp, maybe use Log instead
    }
}

//Mark - Parser

// topic and messages are already lowercase
void PubSub::parseMessage(String topic, String message)
{
    // This creates an infinite loop. Don't do it.
    //log("Parser received: " + topic + ", " + message, LogDebug);
    
    // New Protocol: patriot/<name>  <value>
    if(topic.startsWith(kPublishName+"/")) {
        String subtopic = topic.substring(kPublishName.length()+1);
        
        // Look for reserved names
        // ALIVE
        if(subtopic == "alive" || subtopic.startsWith("alive/")) {
            // message is controller name
            // Ignore it.

        // LOG
        } else if(subtopic == "log" || subtopic.startsWith("log/")) {
            // Ignore it.

        // LOGLEVEL
        } else if(subtopic.startsWith("loglevel")) {
            if(subtopic == "loglevel/"+_controllerName) {
                Log.info(_controllerName + " setting logLevel = " + message);
                parseLogLevel(message);
            }
            
        // MEMORY
        } else if(subtopic == "memory") {
            if(message == _controllerName) {
                publish( "debug/"+_controllerName, String::format("Free memory = %d", System.freeMemory()));
            }
            
        // PING
        } else if(subtopic == "ping") {
            // Respond if ping is addressed to us
            if(message == _controllerName) {
                Log.trace("Ping addressed to us");
                publish(kPublishName + "/pong", _controllerName);
            }
            
        // PONG
        } else if(subtopic == "pong") {
            // Ignore it.
            
        // RESET
        } else if(subtopic == "reset") {
            // Respond if reset is addressed to us
            if(message == _controllerName) {
                Log.info("Reset addressed to us");
                Device::resetAll();
                System.reset(RESET_NO_WAIT);
            }
                
        // STATES
        } else if(subtopic == "states") {
            if(message == _controllerName) {
                Log.info("Received states addressed to us");
                Device::publishStates(_controllerName);
            }
            
        // DEVICE
        } else {
            
            int value = parseValue(message);
            Device *device = Device::get(subtopic);
            if( device != NULL ) {
                
                // Handle save/restore value
                Log.info("Parser setting device " + subtopic + " to " + value);
                if( value == 100 ) {
                    device->saveRestoreValue();
                    device->setValue(value);
                } else if( value == 0 ) {
                    device->restoreSavedValue();
                } else {
                    device->setValue(value);
                }
                Device::buildDevicesVariable();
                
            } else {
                Log.info("Parsed unknown subtopic "+subtopic);
            }
        }
    } else {
        // Not addressed or recognized by us
        Log.error("Parser: Not our message: "+String(topic)+" "+String(message));
    }
}

int PubSub::parseValue(String message)
{
    if(message == "on") {
        return 100;
    } else if(message == "off") {
        return 0;
    }
    return message.toInt();
}

void PubSub::parseLogLevel(String message) {
    LogLevel level = LOG_LEVEL_ERROR;
    if (message == "none") level = LOG_LEVEL_NONE;         // 70
    else if (message == "error") level = LOG_LEVEL_ERROR;  // 50
    else if (message == "warn") level = LOG_LEVEL_WARN;    // 40
    else if (message == "info") level = LOG_LEVEL_INFO;    // 30
    else if (message == "trace") level = LOG_LEVEL_TRACE;  // 1
    else if (message == "all") level = LOG_LEVEL_ALL;      // 1
    else return;

    _logLevel = level;
}
