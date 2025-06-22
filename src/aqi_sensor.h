#pragma once
#include <Wire.h>
#include "SparkFun_ENS160.h"
#include <Adafruit_AHTX0.h>

class aqi_sensor{
public:

    // Measurement data structure
    struct measurements {
        int AQI                 = 0;        // Air quality index
        int TVOC_ppb           = 0;        // 3x1 array for accelerometer data [x, y, z]
        int eCO2_ppm           = 0;                       // 3x1 array for magnetometer data [x, y, z]
        int ENS_status          = 0;
        float Temp_C                = 0.0;
        float rel_humidity_percent  = 0.0;
    };

    measurements meas;

    // Initialization
    bool init();

    // Main functions
    void read();

private:
    bool is_initialized = false;

    SparkFun_ENS160 _myENS;
    Adafruit_AHTX0 _aht;
    sensors_event_t _humidity, _temp;
};