
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
    if( IoT::_mqttManager != NULL ) {
        Log.info("Test _mqttManager set: pass")
    } else {
        numTestsFailed++;
        Log.error("Test _mqttManager set: fail");
    }
}

void UnitTest::device() {
    // Test buildDevicesVariable
    numTests++;
    Device::buildDevicesVariable();
    if( globalDevicesVariable == "blueLed,test1") {
        Log.info("Test buildDevicesVariable: pass");
    } else {
        numTestsFailed++;
        Log.error("Test buildDevicesVariable: fail");
    }
}

void UnitTest::partOfDay() {
    // Test PartOfDay: TODO:
    numTests++;
    PartOfDay *partOfDay = PartOfDay();
    if( globalDevicesVariable == "blueLed,test1") {
        Log.info("Test buildDevicesVariable: pass");
    } else {
        numTestsFailed++;
        Log.error("Test buildDevicesVariable: fail");
    }
}
