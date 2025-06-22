// credentials.h
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// Define structure with aggregated data
const char* ssid     = "XXX";
const char* password = "XXX";

//URL Endpoint for the API
const char* open_weather_URL = "https://api.openweathermap.org/data/3.0/onecall?";
const char* open_weather_API_key = "your_api_key";

// Replace with your location Credentials
const char* lat = "48.00280766557605";
const char* lon = "11.166976374179715";

// Your city name
char* city_name = "Munich";

// NTP setup
const char* NTP_SERVER = "ch.pool.ntp.org";
const char* TZ_INFO    = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

#endif