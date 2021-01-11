/*
 * UnitTests
 *
 * Description: This sketch runs unit tests when loaded
 *
 * PartOfDay Values
 * 0 SUNRISE
 * 1 MORNING
 * 2 NOON
 * 3 AFTERNOON
 * 4 SUNSET
 * 5 DUSK
 * 6 NIGHT
 * 7 DAWN

 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 */

#include <IoT.h>
#include <PatriotPartOfDay.h>
#include <PatriotLight.h>
#include <PatriotNCD8Light.h>
#include <PatriotSwitch.h>

int numTests = 0;
int numTestsFailed = 0;

// Run all tests during setup
void setup() {
    
    IoT::begin("192.168.10.184", "UnitTests");

    Device::add(new Light(7, "blueLed", false, true));

    // Basic devices allow Alexa to control the name
    // and can also turn off other activities.
    Device::add(new Device("test1"));

    //TODO: Test IoT
    testIotBegin();
    
    //TODO: Test basic Device
    testDevice();
    
    //TODO: Test PartOfDay
    
    //TODO: Test Light
    
    // TODO: Test NCD8Light
    
    // TODO: Test Switch
    
    Log.info("%d tests run, %d failed.", numTests, numTestsFailed);
}

void loop() {
    
    IoT::loop();
    
    //TODO: Maybe blink the led or other visual checks

}

void testIot() {
    // Verify begin() set _mqttManager
    numTests++;
    if( IoT::_mqttManager == NULL ) {
        numTestsFailed++;
        Log.error("_mqttManager not set");
    }
}

void testDevice() {
    // Test buildDevicesVariable
    numTests++;
    Device::buildDevicesVariable();
    if( globalDevicesVariable != "blueLed,test1") {
        numTestsFailed++;
        Log.error("buildDevicesVariable fail.");
    }
}
