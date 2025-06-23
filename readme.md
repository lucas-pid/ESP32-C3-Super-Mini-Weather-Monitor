Little weather forecast and indoor quality monitor. I did this project with one of the cheap alix esp32-C3 super mini boards.

The forecast data is taken from OpenWeather. For the indoor measurements, I used an alix board that contains an ENS160 (for air quality) and an AHT21 (temperature and humidity). However I don't recommend buying these boards because the ENS160 heats up a little bit, offsetting the temperature measurements. The screen is a cheap 1.3 inch tft display with 240x240 resolution, also from aliexpress.

If you want to test by yourself you have to edit the "src/credentials.h" with your wifi credentials, API key for openweather, and coordinates for getting local weather data.

Moreover, you have to perform the following:
Open

\.pio\libdeps\c3-super-mini\TFT_eSPI\Processors\TFT_eSPI_ESP32_C3.h

Replace the line:
```
#define SPI_PORT SPI2_HOST
```
To
```
#if ESP_ARDUINO_VERSION_MAJOR < 3
#define SPI_PORT SPI2_HOST
#else
#define SPI_PORT 2
#endif
```
See https://github.com/Bodmer/TFT_eSPI/issues/3284

Display in a 240x240 tft display:
![arcs](https://github.com/lucas-pid/github-images/blob/main/weather-forecast-display/tft_display_240x240.jpg)
