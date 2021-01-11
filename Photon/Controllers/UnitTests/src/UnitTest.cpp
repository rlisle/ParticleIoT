
#include "UnitTest.h"
#include <IoT.h>

extern String globalStatesVariable;
extern String globalDevicesVariable;

UnitTest::UnitTest() {
    numTests = 0;
    numTestsFailed = 0;
}

void UnitTest::iot() {
    // Verify begin() set _mqttManager
    numTests++;
    if( IoT::_mqttManager == NULL ) {
        numTestsFailed++;
        Log.error("_mqttManager not set");
    }
}

void UnitTest::device() {
    // Test buildDevicesVariable
    numTests++;
    Device::buildDevicesVariable();
    if( globalDevicesVariable != "blueLed,test1") {
        numTestsFailed++;
        Log.error("buildDevicesVariable fail.");
    }
}
