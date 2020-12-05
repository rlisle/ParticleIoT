/*
 * RearPanel Controller
 * Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
 * Author: Ron Lisle
 * Date: 11/04/17
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash RearPanel"
 *
 * Hardware3
 * 1. NCD Photon Screw Terminal board
 *    6 switch connections:
 *      A0 Ceiling (brown)
 *      A1 Loft (red)
 *      A2 Ramp Porch Floods (yellow)
 *      A3 Ramp Awning LEDs (green)
 *      A4 Rear Porch Flood (blue)
 *      A5 Rear Awning LEDs (white)
 * 2. NCD 8 PWM OC 8W I2C Dimmer board
 *      A0 Ceiling
 *      A1 Loft
 *      A2 Ramp Porch Floods
 *      A3 Ramp Awning LEDs
 *      A4 Rear Porch Flood
 *      A5 Rear Awning LEDs
 *      A6 Piano Spot

 * Other
   - built-in blue LED     D7
 *
 */
#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>
#include <PatriotActivity.h>

#define ADDRESS 1   // PWM board address A0 jumper set

String mqttServer = "192.168.10.184";

IoT *iot;

// To use persistent storage, insert "retained" before NCD8Relay
NCD8Light ceiling(ADDRESS, 0, "OfficeCeiling", 2);
NCD8Light loft(ADDRESS, 1, "Loft", 2);
NCD8Light rampPorch(ADDRESS, 2, "RampPorch", 2);
NCD8Light rampAwning(ADDRESS, 3, "RampAwning", 2);
NCD8Light rearPorch(ADDRESS, 4, "RearPorch", 2);
NCD8Light rearAwning(ADDRESS, 5, "RearAwning", 2);
NCD8Light piano(ADDRESS, 6, "Piano", 2);


Switch ceilingSwitch(A0, "OfficeCeilingSwitch");
Switch loftSwitch(A1, "LoftSwitch");
Switch rampPorchSwitch(A2, "RampPorchSwitch");
Switch rampAwningSwitch(A3, "RampAwningSwitch");
Switch rearPorchSwitch(A4, "RearPorchSwitch");
Switch rearAwningSwitch(A5, "RearAwningSwitch");

Activity goodMorning("goodmorning");

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);   // MQTT bridge enabled
    iot->setLogLevel(LogDebug);

    // BEHAVIORS
    ceiling.addBehavior(20, "goodmorning", '>', 0);
    
    ceiling.addBehavior(100, "OfficeCeilingSwitch", '>', 0);
    loft.addBehavior(100, "LoftSwitch", '>', 0);
    rampPorch.addBehavior(100, "RampPorchSwitch", '>', 0);
    rampAwning.addBehavior(100, "RampAwningSwitch", '>', 0);
    rearPorch.addBehavior(100, "RearPorchSwitch", '>', 0);
    rearAwning.addBehavior(100, "RearAwningSwitch", '>', 0);

    // ADD ALL DEVICES
    iot->addDevice(&ceiling);
    iot->addDevice(&loft);
    iot->addDevice(&rampPorch);
    iot->addDevice(&rampAwning);
    iot->addDevice(&rearPorch);
    iot->addDevice(&rearAwning);
    iot->addDevice(&piano);

    iot->addDevice(&ceilingSwitch);
    iot->addDevice(&loftSwitch);
    iot->addDevice(&rampPorchSwitch);
    iot->addDevice(&rampAwningSwitch);
    iot->addDevice(&rearPorchSwitch);
    iot->addDevice(&rearAwningSwitch);
    
    iot->addDevice(&goodMorning);
}

void loop() {
    iot->loop();
}
