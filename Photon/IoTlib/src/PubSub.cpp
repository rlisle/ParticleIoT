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

//#define MQTT_TIMEOUT_SECONDS 60*5
#define ALIVE_MSG_SECONDS 60 * 60

PubSub::PubSub(String controllerName)
{
    _controllerName = controllerName.toLowerCase();
    _logging = 0;

    // We'll want to start with ALL whenever debugging new code.
    // Beware of Particle messages limits though (about 200k = 2/minute)
    // Use PubSub to switch to error when done testing or vs. a vs.
    _logLevel = LOG_LEVEL_ERROR;

    Time.zone(-6.0);
    
    //TODO: do we need this, and what should we pass?
    //const LogCategoryFilters &filters) : LogHandler(level, filters)

    //TODO: move subscribe code from IoT to here?
    
}

bool PubSub::publish(String topic, String message) {
    if(_pubSub != NULL) {
        _pubSub->publish(topic,message);
        return true;
    }
    return false;
}

void PubSub::loop()
{
    if(_pubSub != NULL) {
        sendAlivePeriodically()
        reconnectCheck();
    }
}

void PubSub::sendAlivePeriodically() {
    system_tick_t secondsSinceLastAlive = Time.now() - _lastAliveTime;
    if(secondsSinceLastAlive > ALIVE_MSG_SECONDS) {
        secondsSinceLastAlive = Time.now()
        publish("patriot/alive", _controllerName)   //TODO: add timestamp, maybe use Log instead
    }
}

//void PubSub::msgHandler(char* rawTopic, byte* payload, unsigned int length) {
//
//    char p[length + 1];
//    memcpy(p, payload, length);
//    p[length] = 0;
//    String message(p);
//    String topic(rawTopic);
//
//    _lastMQTTtime = Time.now();
//
//    parseMessage(topic.toLowerCase(), message.toLowerCase());
//}

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

// The following methods are taken from Particle FW, specifically spark::StreamLogHandler.
// See https://github.com/spark/firmware/blob/develop/wiring/src/spark_wiring_logging.cpp
const char* PubSub::extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* PubSub::extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1;                                                                                                                         // Skip return type
        } else if (*s == '(') {
            break;                                                                                                                         // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

// This method is how we are called by the LogManager
void PubSub::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
    String s;

//    LOG_LEVEL_ALL = 1
//    LOG_LEVEL_TRACE = 1
//    LOG_LEVEL_INFO = 30
//    LOG_LEVEL_WARN= 40
//    LOG_LEVEL_ERROR = 50
//    LOG_LEVEL_NONE = 70
    if (level < _logLevel) return;
    
    // Source file
    if (attr.has_file) {
        s = extractFileName(attr.file);                                                                                 // Strip directory path
        s.concat(s);                                                                                 // File name
        if (attr.has_line) {
            s.concat(":");
            s.concat(String(attr.line));                                                                                                                         // Line number
        }
        if (attr.has_function) {
            s.concat(", ");
        } else {
            s.concat(": ");
        }
    }

    // Function name
    if (attr.has_function) {
        size_t n = 0;
        s = extractFuncName(attr.function, &n);                                                                                 // Strip argument and return types
        s.concat(s);
        s.concat("(): ");
    }

    // Level
    s.concat(levelName(level));
    s.concat(": ");

    // Message
    if (msg) {
        s.concat(msg);
    }

    // Additional attributes
    if (attr.has_code || attr.has_details) {
        s.concat(" [");
        // Code
        if (attr.has_code) {
            s.concat(String::format("code = %p", (intptr_t)attr.code));
        }
        // Details
        if (attr.has_details) {
            if (attr.has_code) {
                s.concat(", ");
            }
            s.concat("details = ");
            s.concat(attr.details);
        }
        s.concat(']');
    }

    //TODO: If MQTT not connected, write to Serial instead
//    Serial.println(s);
    log(category, s);
}

// This is our formatter. We can format messages however we want.
void PubSub::log(const char *category, String message) {
    String time = Time.format(Time.now(), "%a %H:%M");

//    if(!_logging && strcmp(category, "app") == 0) {
    if(!_logging) {
        _logging++;
        publish("patriot/log/"+_controllerName, time + " " + message);
        _logging--;
    }
}

