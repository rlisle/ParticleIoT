
#include "UnitTest.h"
#include <IoT.h>
#include <PatriotPartOfDay.h>

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
        Log.info("Test _mqttManager set passed");
    } else {
        numTestsFailed++;
        Log.error("Test _mqttManager set failed");
    }
}

void UnitTest::device() {
    // Test buildDevicesVariable
    numTests++;
    Device::buildDevicesVariable();
    if( globalDevicesVariable == "blueLed, test1") {
        Log.info("Test buildDevicesVariable passed");
    } else {
        numTestsFailed++;
        Log.error("Test buildDevicesVariable failed");
        Log.error( "globalDevicesVariable: '" + globalDevicesVariable + "' expected to be 'blueLed, test1'" );
    }
}

void UnitTest::partOfDay() {
    
    PartOfDay *partOfDay = new PartOfDay();
    
    numTests++;
    if( partOfDay != NULL) {
        Log.info("Test PartOfDay instantiates passed");
    } else {
        numTestsFailed++;
        Log.error("Test PartOfDay instantiates failed");
        Log.error("Aborting remaining PartOfDay tests");
        return;
    }
    
    numTests++;
    partOfDay->begin();
    if( partOfDay->_lastPollTime == 0
       && partOfDay->_month == 0
       && partOfDay->_day == 0) {
        Log.info("Test PartOfDay state after begin passed");
    } else {
        numTestsFailed++;
        Log.error("Test PartOfDay state after begin failed");
        Log.error("Expected _lastPollTime, _month, and _Day to be all 0 but were: %ld, %d, %d",partOfDay->_lastPollTime,partOfDay->_month,partOfDay->_day);
    }

    
    delete partOfDay;
}
