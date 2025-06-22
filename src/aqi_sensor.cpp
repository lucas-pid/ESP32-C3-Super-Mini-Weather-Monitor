#include <Wire.h>
#include "SparkFun_ENS160.h"
#include <Adafruit_AHTX0.h>
#include "aqi_sensor.h"

// Inialization function
// Change accordingly depending on sensor
bool aqi_sensor::init() {

    Wire.begin();

    // Initialize ENS sensor
    if(!_myENS.begin()){
        while(1);
    }

    _myENS.setOperatingMode(SFE_ENS160_RESET);

    _myENS.setOperatingMode(SFE_ENS160_STANDARD);

    _aht.begin();

    return true; // or false if initialization fails
}

// Read sensor
void aqi_sensor::read() {

    if( _myENS.checkDataStatus() )
	{
        meas.AQI         = _myENS.getAQI();
        meas.TVOC_ppb    = _myENS.getTVOC();
        meas.eCO2_ppm    = _myENS.getECO2();
        meas.ENS_status  = _myENS.getFlags();
    };

    _aht.getEvent(&_humidity, &_temp);// populate temp and humidity objects with fresh data
    meas.Temp_C = _temp.temperature;
    meas.rel_humidity_percent = _humidity.relative_humidity;
};
