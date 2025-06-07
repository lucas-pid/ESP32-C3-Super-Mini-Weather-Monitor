#include <TFT_eSPI.h>
#include <lvgl.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))

class AirQualityDisplay {
public:
    //AirQualityDisplay();
    void begin();
    void update_display(int AQI, int TVOC_ppb, int eCO2_ppm, float Temp_C, float rel_humidity_percent);
    
private:
    static const int screen_width_pixels = 240;
    static const int screen_height_pixels = 240;
    
    lv_obj_t* aqi_container;
    lv_obj_t* tvoc_container;
    lv_obj_t* eco2_container;
    lv_obj_t* temp_container;
    lv_obj_t* hum_container;
    lv_obj_t* indoor_display_container;
    lv_obj_t* test;

    static lv_style_t icon_style;
    static lv_style_t text_style_big;
    static lv_style_t text_style_normal;
    static lv_style_t text_style_small;
    
    uint32_t draw_buf[DRAW_BUF_SIZE / 4];
    
    void create_gui();
    void create_sensor_read_display(int row, lv_obj_t*& parent_obj, lv_obj_t*& child_obj);
    lv_color_t get_aqi_color(int aqi);
};