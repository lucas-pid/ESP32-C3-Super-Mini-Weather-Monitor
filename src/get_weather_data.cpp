#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "get_weather_data.h"

// Define a struct to hold weather data
bool WeatherClient:: update(const String& api_call) {

  HTTPClient http;

  http.begin(api_call);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String JSON_Data = http.getString();

    // Define filter to get only the relevant data
    StaticJsonDocument<688> filter;
    filter["lat"] = true;
    filter["lon"] = true;
    filter["timezone_offset"] = true;

    JsonObject filter_current = filter.createNestedObject("current");
    filter_current["temp"] = true;
    filter_current["humidity"] = true;
    filter_current["feels_like"] = true;
    filter_current["dt"] = true;

    JsonObject filter_current_weather_0 = filter_current["weather"].createNestedObject();
    filter_current_weather_0["icon"] = true;
    filter_current_weather_0["description"] = true;
    filter_current_weather_0["main"] = true;

    JsonObject filter_hourly_0 = filter["hourly"].createNestedObject();
    filter_hourly_0["dt"] = true;
    filter_hourly_0["temp"] = true;
    filter_hourly_0["humidity"] = true;
    filter_hourly_0["pop"] = true;

    JsonObject filter_hourly_0_weather_0 = filter_hourly_0["weather"].createNestedObject();
    filter_hourly_0_weather_0["main"] = true;
    filter_hourly_0_weather_0["icon"] = true;

    JsonObject filter_daily_0 = filter["daily"].createNestedObject();
    filter_daily_0["dt"] = true;

    JsonObject filter_daily_0_temp = filter_daily_0.createNestedObject("temp");
    filter_daily_0_temp["max"] = true;
    filter_daily_0_temp["min"] = true;
    filter_daily_0_temp["day"] = true;
    filter_daily_0_temp["night"] = true;
    filter_daily_0_temp["eve"] = true;
    filter_daily_0_temp["morn"] = true;
    filter_daily_0["sunrise"] = true;
    filter_daily_0["sunset"] = true;
    filter_daily_0["humidity"] = true;
    filter_daily_0["summary"] = true;
    filter_daily_0["pop"] = true;

    JsonObject filter_daily_0_weather_0 = filter_daily_0["weather"].createNestedObject();
    filter_daily_0_weather_0["main"] = true;
    filter_daily_0_weather_0["icon"] = true;

    DynamicJsonDocument doc(12288);
    DeserializationError error = deserializeJson(doc, JSON_Data, DeserializationOption::Filter(filter));

    if (!error) {
      // If no errors, parse the data

      // General data
      forecast.lat = doc["lat"]; // 48.1383
      forecast.lon = doc["lon"]; // 11.5775
      forecast.timezone_offset = doc["timezone_offset"]; // 7200

      // Current weather
      JsonObject current                = doc["current"];
      forecast.current.temp_C           = current["temp"]; // 25.03
      forecast.current.temp_feels_like_C= current["feels_like"]; // 25.03
      forecast.current.humidity_percent = current["humidity"]; // 50
      JsonObject current_weather_0      = current["weather"][0];
      forecast.current.main             = current_weather_0["main"].as<String>(); // "Clouds"
      forecast.current.description      = current_weather_0["description"].as<String>(); // "broken clouds"
      forecast.current.icon             = current_weather_0["icon"].as<String>(); // "04d"

      // Hourly data
      // Loop over json array
      int count = 0;
      for (JsonObject hourly_item : doc["hourly"].as<JsonArray>()) {

        forecast.hourly[count].dt               = hourly_item["dt"]; // 1750320000, 1750323600, 1750327200, 1750330800, 1750334400, ...
        forecast.hourly[count].temp_C           = hourly_item["temp"]; // 25.03, 24.85, 25.08, 25.62, 26.59, 27.67, 28.64, 28.71, ...
        forecast.hourly[count].humidity_percent = hourly_item["humidity"]; // 50, 51, 51, 50, 48, 48, 46, 47, 49, 56, 67, 77, ...

        JsonObject hourly_item_weather_0        = hourly_item["weather"][0];
        forecast.hourly[count].main             = hourly_item_weather_0["main"].as<String>(); // "Clouds", "Clouds", "Clouds", ...
        forecast.hourly[count].icon             = hourly_item_weather_0["icon"].as<String>(); // "04d", "04d", "03d", "03d", ...
        forecast.hourly[count].pop_percent      = static_cast<int>(std::ceil(hourly_item["pop"].as<float>()*100));

        // Increment counter and check if maximum iteration was reached
        count++;
        if (count >= MAX_HOURLY){
          break;
        }
      }

      // Daily forecast
      count = 0;
      for (JsonObject daily_item : doc["daily"].as<JsonArray>()) {

        forecast.daily[count].dt                = daily_item["dt"]; // 1750330800, 1750417200, 1750503600, 1750590000, 1750676400, ...
        forecast.daily[count].moon_phase        = daily_item["moon_phase"];
        forecast.daily[count].sunrise           = daily_item["sunrise"];
        forecast.daily[count].sunset            = daily_item["sunset"];
        forecast.daily[count].summary           = daily_item["summary"].as<String>(); // "You can expect partly cloudy in the morning, ...

        JsonObject daily_item_temp              = daily_item["temp"];
        forecast.daily[count].temp_day_C        = daily_item_temp["day"]; // 25.62, 22.85, 22.55, 28.72, 22.06, 30.2, 29.06, ...
        forecast.daily[count].temp_min_C        = daily_item_temp["min"]; // 14.04, 12.75, 11.31, 10.54, 16.77, 16.14, 18.71, ...
        forecast.daily[count].temp_max_C        = daily_item_temp["max"]; // 28.71, 23.85, 23.86, 28.75, 28.99, 30.2, 29.06, ...
        forecast.daily[count].temp_night_C      = daily_item_temp["night"]; // 17.39, 15.11, 14.31, 19.52, 18.56, 21.62, ...
        forecast.daily[count].temp_evening_C    = daily_item_temp["eve"]; // 26.78, 21.77, 22.09, 24.25, 21.98, 26.15, 24.46, ...
        forecast.daily[count].temp_morning_C    = daily_item_temp["morn"]; // 20.74, 14.31, 12.82, 13.58, 20.99, 19.07, ...

        forecast.daily[count].humidity_percent  = daily_item["humidity"]; // 50, 52, 44, 35, 75, 41, 46, 44

        JsonObject daily_item_weather_0         = daily_item["weather"][0];
        forecast.daily[count].main              = daily_item_weather_0["main"].as<String>(); // "Clouds", "Clear", "Clouds", ...
        //forecast.daily[count].description       = daily_item_weather_0["description"].as<String>(); // "scattered ...
        forecast.daily[count].icon              = daily_item_weather_0["icon"].as<String>(); // "03d", "01d", "04d", "01d", ...

        forecast.daily[count].pop_percent       = static_cast<int>(std::ceil(daily_item["pop"].as<float>()*100)); // 0, 0, 0, 0, 1, 1, 1, 1

        // Increment counter and check if maximum iteration was reached
        count++;
        if (count >= MAX_DAILY){
          break;
        }
      }

      http.end();
      return true;

    } else {
      Serial.print("Deserialization failed: ");
      Serial.println(error.f_str());
    }
  } else {
    Serial.println("HTTP request failed");
  }
  http.end();
  return false;
}
