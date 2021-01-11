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

#include "UnitTest.h"

// Run all tests during setup
void setup() {
    
    UnitTest *test = new UnitTest();
    
    IoT::begin("192.168.10.184", "UnitTests");
    Device::add(new Light(7, "blueLed", false, true));
    Device::add(new Device("test1"));

    Log.info("#");
    Log.info("#");
    Log.info("# Starting Unit Tests");
    Log.info("#");
    Log.info("#");
    
    //Run Tests
    test->iot();
    test->device();
    test->partOfDay();
    
    //TODO: Test Light
    // TODO: Test NCD8Light
    // TODO: Test Switch
    
    // Print summary
    Log.info("#");
    Log.info("#");
    if( test->numTests > 0 && test->numTestsFailed==0 ) {
        Log.info("# SUCCESS! All %d Tests Passed\n#\n#",test->numTests);
    } else {
        Log.info("# Test Complete: %d tests run, %d failed.", test->numTests, test->numTestsFailed);
    }
    Log.info("#");
    Log.info("#");
}

void loop() {
    
    IoT::loop();
    
    //TODO: Maybe blink the led or other visual checks

}
