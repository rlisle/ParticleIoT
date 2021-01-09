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

// Run all tests during setup
void setup() {
    IoT::begin("192.168.10.184", "RonTest");

    Device::add(new Light(7, "blueLed", false, true));

    // Basic devices allow Alexa to control the name
    // and can also turn off other activities.
    Device::add(new Device("cleaning"));
    Device::add(new Device("sleeping"));
    Device::add(new Device("partofday"));

    int changedSleeping = Device::getChangedValue("sleeping");
    Log.info("changedSleeping = %d",changedSleeping);
    
    int changedPartOfDay = Device::getChangedValue("partofday");
    int sleeping = Device::value("sleeping");
    int partOfDay = Device::value("partofday");

    //TODO: Test IoT
    
    //TODO: Test PartOfDay
    
    //TODO: Test Light
    
    // TODO: Test NCD8Light
    
    // TODO: Test Switch
    
    Log.info("setAllInsideLights %d",value);
}

void loop() {
    
    IoT::loop();
    
    //TODO: Maybe blink the led or other visual checks

}
