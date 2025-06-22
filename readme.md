Open

\.pio\libdeps\c3-super-mini\TFT_eSPI\Processors\TFT_eSPI_ESP32_C3.h

Replace the line:

#define SPI_PORT SPI2_HOST

To

#if ESP_ARDUINO_VERSION_MAJOR < 3
#define SPI_PORT SPI2_HOST
#else
#define SPI_PORT 2
#endif

See https://github.com/Bodmer/TFT_eSPI/issues/3284