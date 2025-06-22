#ifndef GET_WEATHER_DATA_H
#define GET_WEATHER_DATA_H

#include <Arduino.h>
#include <vector>

#define MAX_HOURLY 48
#define MAX_DAILY 3

// Define structure for hourly forecast data
struct HourlyForecast {
  long dt;
  float temp_C;
  int humidity_percent;
  int pop_percent;
  String main;
  String icon;
};

// Define structure for daily forecast data
struct DailyForecast {
  long dt, sunrise, sunset;
  float temp_min_C;
  float temp_max_C;
  float temp_day_C;
  float temp_night_C;
  float temp_evening_C;
  float temp_morning_C;
  float moon_phase;
  int humidity_percent;
  int pop_percent;
  String summary, main, icon;
};

// Define structure for current weather data
struct CurrentForecast {
  long dt;
  float temp_C;
  float temp_feels_like_C;
  int humidity_percent;
  String main;
  String description;
  String icon;
};

// Define structure with aggregated data
struct WeatherForecast {
  int timezone_offset;
  float lat;
  float lon;

  CurrentForecast current;
  HourlyForecast hourly[MAX_HOURLY]; // Array for each hour
  DailyForecast daily[MAX_DAILY];    // Array for each day
};

class WeatherClient {
public:
  // Forecast data
  WeatherForecast forecast;

  // Method to make api call and update forecast data
  // Returns true on success, false on error
  bool update(const String& api_call);
};

#endif
