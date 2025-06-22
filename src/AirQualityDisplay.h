#pragma once
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "aqi_sensor.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
#define MEDIUM_FONT_SIZE 14
#define SMALL_FONT_SIZE  10
#define SYMBOL_FONT_SIZE 25
#define SPACING_SMALL 2
#define FORECAST_HOURS 8
#define FORECAST_DAYS 2

// Define structure for daily forecast data
struct DailyForecastData {
  int day_int; // Starts from
  int sunrise_hour_min[2]; // First index hour, second index minute
  int sunset_hour_min[2];  // First index hour, second index minute
  int pop_percent;
  int temp_max_C;
  int temp_min_C;
  String icon;
  String summary;
};

// Define structure for current weather data
struct CurrentForecastData {
  int day_int; // Starts from
  int pop_percent;
  int temp_max_C;
  int temp_min_C;
  String icon;
  String main;
};

// Define structure for hourly forecast data
struct HourlyForecastData {
  int hour_24h; // Hour in 24h format 
  int pop_percent;
  int temp_C;
  String icon;
};

// Define structure for sensor data
struct SensorData {
  int AQI;
  int TVOC_ppb;
  int eCO2_ppm;
  float Temp_C;
  float rel_humidity_percent;
};

// Define structure with aggregated data
struct WeatherForecastData {
  char time_char[9] = "19:50:12";
  char date_char[11];
  char* city_name;

  aqi_sensor::measurements sensor;
  // CurrentForecastData current;
  HourlyForecastData hourly[FORECAST_HOURS]; // Array for each hour
  DailyForecastData daily[FORECAST_DAYS+1];    // Array for each day
};

class AirQualityDisplay {
public:
    //AirQualityDisplay();
    void begin();
    void update_time();
    void update_sensor();
    void update_forecast();
    void define_styles();

    // Define struct with all the data needed
    WeatherForecastData display_data;
    
private:
    static const int screen_width_pixels = 240;
    static const int screen_height_pixels = 240;
    
    lv_obj_t* aqi_container;
    lv_obj_t* tvoc_container;
    lv_obj_t* eco2_container;
    lv_obj_t* temp_container;
    lv_obj_t* hum_container;
    lv_obj_t* indoor_display_container;
    lv_obj_t* today_display_container;
    lv_obj_t* today_display;
    lv_obj_t* sunset_sunrise_display_container;
    lv_obj_t* time_display_container;
    lv_obj_t* hourly_forecast_display_container;
    lv_obj_t* hourly_forecast_display[FORECAST_HOURS];
    lv_obj_t* nextday_forecast_display_container;
    lv_obj_t* nextday_forecast_display[FORECAST_DAYS];

    static lv_style_t label_style;
    static lv_style_t box_style;
    
    uint32_t draw_buf[DRAW_BUF_SIZE / 4];
    
    void create_gui();
    void create_today_display(lv_obj_t*& parent_obj);
    void create_time_display(lv_obj_t*& parent_obj);
    void create_hourly_forecast_display(lv_obj_t*& parent_obj);
    void create_nextday_forecast_display(lv_obj_t*& parent_obj);
    void create_dayly_template_display(lv_obj_t*& child_obj, int width = 55, int height = 55);
    void create_sunrise_sunset_template_display(lv_obj_t*& child_obj);
    void create_sensor_template_display(lv_obj_t*& child_obj, int width, int height);
    char* get_symbol_text(String iconCode);
    char* getWeekdayShort(int weekday);
    char* get_aqi_txt(int aqi);
    lv_color_t get_aqi_color(int aqi);
};

