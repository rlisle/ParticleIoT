/**
 PubSub.h
 
 This class handles all Particle.io PubSub interactions.
 
 Note: to avoid making this a singleton,
 the caller must provide global callback handlers (see externs).
 
 http://www.github.com/rlisle/Patriot
 
 Written by Ron Lisle
 
 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
 
 */
#pragma once
#include "Particle.h"

class PubSub
{
public:
    LogLevel    _logLevel;  // Currently unused.

    PubSub(String controllerName);
    
    void        publish(String topic, String message);
    void        parseMessage(String topic, String message);
    void        loop();
    void        handler(char* topic, byte* payload, unsigned int length);
        
private:
    String        _controllerName;
    system_tick_t _lastAliveTime;   // Send out alive messages periodically
    
    void      (*_callback)(char*,uint8_t*,unsigned int);
    int       parseValue(String message);
    void      parseLogLevel(String message);
    void      sendAlivePeriodically();
};
