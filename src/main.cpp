#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include "AirQualityDisplay.h"
#include "aqi_sensor.h"
#include "get_weather_data.h"
#include "esp_sntp.h"
#include "credentials.h"

AirQualityDisplay display;
aqi_sensor aqi_sensor_obj;
WeatherClient weather;

// URL call
String api_call = String(open_weather_URL) + "lat=" + String(lat) + "&lon=" + String(lon) + "&exclude=minutely&units=metric&appid=" + String(open_weather_API_key);

tm timeinfo;
time_t now;

bool update_sensor_widgets;
bool update_forecast_bool;

void update_display_forecast(){
  //
  time_t forecast_time;
  struct tm* timeinfo_forecast;

  // Make api call to get up to date forecast
  if (WiFi.status() == WL_CONNECTED) {
    //Set HTTP Request Final URL with Location and API key information
    weather.update(api_call);
  }

  // Update hourly
  for(int ii = 0; ii < FORECAST_HOURS; ii++){
    display.display_data.hourly[ii].icon = weather.forecast.hourly[ii].icon;
    display.display_data.hourly[ii].pop_percent = weather.forecast.hourly[ii].pop_percent;
    display.display_data.hourly[ii].temp_C = weather.forecast.hourly[ii].temp_C;

    // Get time of forecast
    forecast_time                             = static_cast<time_t>(weather.forecast.hourly[ii].dt);
    timeinfo_forecast                         = localtime(&forecast_time);
    display.display_data.hourly[ii].hour_24h  = timeinfo_forecast->tm_hour;
  }

  // Update daily
  for(int ii = 0; ii < FORECAST_DAYS+1; ii++){
    display.display_data.daily[ii].icon = weather.forecast.daily[ii].icon;
    display.display_data.daily[ii].pop_percent = weather.forecast.daily[ii].pop_percent;
    display.display_data.daily[ii].temp_max_C = weather.forecast.daily[ii].temp_max_C;
    display.display_data.daily[ii].temp_min_C = weather.forecast.daily[ii].temp_min_C;
    display.display_data.daily[ii].summary   = weather.forecast.daily[ii].summary;

    // Get day of forecast
    forecast_time                             = static_cast<time_t>(weather.forecast.daily[ii].dt);
    timeinfo_forecast                         = localtime(&forecast_time);
    display.display_data.daily[ii].day_int    = timeinfo_forecast->tm_wday;

    // Get time of sunrise
    forecast_time                             = static_cast<time_t>(weather.forecast.daily[ii].sunrise);
    timeinfo_forecast                         = localtime(&forecast_time);
    display.display_data.daily[ii].sunrise_hour_min[0]  = timeinfo_forecast->tm_hour;
    display.display_data.daily[ii].sunrise_hour_min[1]  = timeinfo_forecast->tm_min;

    // Get time of sunset
    forecast_time                             = static_cast<time_t>(weather.forecast.daily[ii].sunset);
    timeinfo_forecast                         = localtime(&forecast_time);
    display.display_data.daily[ii].sunset_hour_min[0]  = timeinfo_forecast->tm_hour;
    display.display_data.daily[ii].sunset_hour_min[1]  = timeinfo_forecast->tm_min;
  }

  // Call method to update display
  display.update_forecast();
}

void read_sensors(void *pvParameters) {
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    // Read sensor
    aqi_sensor_obj.read();
    // Copy sensor measurements to display object
    display.display_data.sensor = aqi_sensor_obj.meas;
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void update_display_sensors(void *pvParameters) {
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    // Set to true
    update_sensor_widgets = true;
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void update_forecast(void *pvParameters) {
  const TickType_t xFrequency = pdMS_TO_TICKS(1000*60*30);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    // Set to true
    update_forecast_bool = true;
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {

  Serial.begin(115200);

  delay(4000);
  Serial.println("Hello from the setup!");

  WiFi.begin(ssid, password);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print ( "." );
    if (++counter > 100) {
      ESP.restart();
      Serial.print ( "." );
    }
  }

  Serial.println("\n\nWiFi connected\n\n");

  // set notification call-back function
  configTzTime(TZ_INFO, NTP_SERVER);  

  if (WiFi.status() == WL_CONNECTED) {
    //Set HTTP Request Final URL with Location and API key information
    weather.update(api_call);
  }

  // Set city name
  display.display_data.city_name = city_name;

  // Initialize display
  display.begin();

  // Initialize sensors
  aqi_sensor_obj.init();

  // Add task to read sensors
  xTaskCreate(
    read_sensors,
    "Read Sensors Task",
    4096,
    NULL,
    2,      // High priority
    NULL
  );

  // Add task to trigger the update of the sensors widgets
  xTaskCreate(
    update_display_sensors,
    "Update display Sensors Task",
    512,
    NULL,
    1,      // Low priority
    NULL
  );

  // Add task to trigger the update of the forecast
  xTaskCreate(
    update_forecast,
    "Update Forecast Task",
    512,
    NULL,
    1,      // Low priority
    NULL
  );

  delay(2000);
  Serial.println("Setup Done!");
}

void loop() {

  // Update time and date
  getLocalTime(&timeinfo);
  strftime(display.display_data.time_char, sizeof(display.display_data.time_char), "%H:%M:%S", &timeinfo);
  strftime(display.display_data.date_char, sizeof(display.display_data.date_char), "%B/%d/%Y", &timeinfo);

  // Update displayed time
  display.update_time();

  // Update sensors widgets
  if (update_sensor_widgets){
    display.update_sensor();
    update_sensor_widgets = false;
  }

  // Update forecast
  if (update_forecast_bool){
    update_display_forecast();
    update_forecast_bool = false;
  }
    
  // Update display
  lv_tick_set_cb(xTaskGetTickCount);
  lv_task_handler();
  lv_timer_handler();
}