/**
NCD 8 Switch board Example

This example supports a switch connected to the 1st input
on an NCD 8 GPIO board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotNCD8Switch.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("MyTest");
    iot->begin();

    byte address = 0x00;    // No jumpers set
    Switch *switch1 = new Switch(address, 0, "switch1");
    iot->addDevice(switch1);
}

void loop() {
    iot->loop();
}
