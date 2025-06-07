#include <Arduino.h>
#include <TFT_eSPI.h>
#include "AirQualityDisplay.h"
#include "sensor.h"

AirQualityDisplay display;
sensor aqi_sensor;

void setup() {
    display.begin();
    aqi_sensor.init();
    Serial.begin(115200);
    delay(2000);
    Serial.println("Hello from the setup!");
}

void loop() {

    // Read sensor
    aqi_sensor.read();

    // Update display
    Serial.println("Hello from the loop!");
    display.update_display(aqi_sensor.meas.AQI, aqi_sensor.meas.TVOC_ppb,
                            aqi_sensor.meas.eCO2_ppm, aqi_sensor.meas.Temp_C, aqi_sensor.meas.rel_humidity_percent);

    
    delay(500);
}