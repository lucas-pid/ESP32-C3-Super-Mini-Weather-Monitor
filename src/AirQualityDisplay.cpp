#include "AirQualityDisplay.h"
#include <vector>

LV_FONT_DECLARE(symbols_AQI_36_3bpp);
LV_FONT_DECLARE(bebas_neu_24_3bpp);
LV_FONT_DECLARE(bebas_neu_12_3bpp);

#define AQI_SYMBOL "A"
#define TVOC_SYMBOL "B"
#define CO2_SYMBOL "C"
#define HUMIDITY_SYMBOL "D"
#define THERMOMETER_SYMBOL "E"

void AirQualityDisplay::begin() {

    lv_init();
    lv_display_t *disp = lv_tft_espi_create(screen_width_pixels, screen_height_pixels, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270); 
    
    create_gui();
}

void AirQualityDisplay::create_gui() {

    // Create object to acomodate all the sensor readings
    indoor_display_container = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(indoor_display_container, 0, 0);

    // Disable scroll bar
    lv_obj_set_scrollbar_mode(indoor_display_container, LV_SCROLLBAR_MODE_OFF);

    // Half the screen
    lv_obj_set_size(indoor_display_container, screen_width_pixels/2, screen_height_pixels);

    // Create label for each entry
    create_sensor_read_display(0, indoor_display_container, aqi_container);
    create_sensor_read_display(1, indoor_display_container, eco2_container);
    create_sensor_read_display(2, indoor_display_container, temp_container);
    create_sensor_read_display(3, indoor_display_container, hum_container);
    create_sensor_read_display(4, indoor_display_container, tvoc_container);
}

void AirQualityDisplay::create_sensor_read_display(int row, lv_obj_t*& parent_obj, lv_obj_t*& child_obj) {

    // Get height of parent object to divide the rows
    const int height_obj = lv_obj_get_height(parent_obj);

    // Define space between values and initial offset
    const int spacing       = 37;
    const int offset_y      = 45;

    // Define position of second column
    const int col_x         = 45;

    // Define offset on x position
    const int offset_x      = 0;

    // Compute spacing of current row
    const int row_y         = offset_y + spacing*row;

    //char* symbol;
    char symbol[16];
    char unit[16];

    // Set order of values
    std::vector<const char*> values_order = {"aqi", "co2", "temp", "humidity", "tvoc"};

    // Get the label
    const char* current_label = values_order[row];

    lv_color_t c;

    if (strcmp(current_label, "aqi") == 0) {
        strcpy(symbol, AQI_SYMBOL);
        strcpy(unit, "");
        c = lv_palette_main(LV_PALETTE_BLUE);

    } else if (strcmp(current_label, "co2") == 0){
        strcpy(symbol, CO2_SYMBOL);
        strcpy(unit, "ppm");
        c = lv_palette_main(LV_PALETTE_RED);

    } else if (strcmp(current_label, "temp") == 0){
        strcpy(symbol, THERMOMETER_SYMBOL);
        strcpy(unit, "°C");
        c = lv_palette_main(LV_PALETTE_GREEN);

    } else if (strcmp(current_label, "humidity") == 0){
        strcpy(symbol, HUMIDITY_SYMBOL);
        strcpy(unit, "%");

    } else if (strcmp(current_label, "tvoc") == 0){
        strcpy(symbol, TVOC_SYMBOL);
        strcpy(unit, "ppb");

    } else {
        strcpy(symbol, AQI_SYMBOL);
        strcpy(unit, "");
    }

    // Create container for the readings
    child_obj = lv_obj_create(parent_obj);
    lv_obj_set_size(child_obj, lv_pct(100), spacing);
    lv_obj_set_pos(child_obj, 0, row_y);

    // create label for symbol
    lv_obj_t *symbol_label_obj = lv_label_create(child_obj);
    lv_obj_set_pos(symbol_label_obj, offset_x, 0);
    lv_label_set_text(symbol_label_obj, symbol);
    lv_obj_set_align(symbol_label_obj, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(symbol_label_obj, &symbols_AQI_36_3bpp, 0);

    // create label for number
    lv_obj_t *value_label_obj = lv_label_create(child_obj);
    lv_obj_set_align(value_label_obj, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(value_label_obj, -30, 0);
    lv_obj_set_style_text_font(value_label_obj, &bebas_neu_24_3bpp, 0);
    lv_obj_set_name(value_label_obj, current_label);

    // create label for unit
    lv_obj_t *unit_label_obj = lv_label_create(child_obj);
    lv_obj_align_to(unit_label_obj, value_label_obj, LV_ALIGN_OUT_RIGHT_TOP, 25 , 5);
    lv_label_set_text(unit_label_obj, unit);
    lv_obj_set_style_text_font(unit_label_obj, &bebas_neu_12_3bpp, 0);

    lv_obj_set_style_bg_color(child_obj, c, 0);
    lv_obj_set_scrollbar_mode(child_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(child_obj, 1, 0);
    
}

void AirQualityDisplay::update_display(int AQI, int TVOC_ppb, int eCO2_ppm, float Temp_C, float rel_humidity_percent) {
    lv_tick_set_cb(xTaskGetTickCount);
    lv_task_handler();
    lv_timer_handler();

    // Directly update
    char text[10];
    sprintf(text, "%d", AQI);
    lv_label_set_text(lv_obj_get_child_by_name(aqi_container, "aqi"), text);

    sprintf(text, "%d", eCO2_ppm);
    lv_label_set_text(lv_obj_get_child_by_name(eco2_container, "co2"), text);

    sprintf(text, "%d", TVOC_ppb);
    lv_label_set_text(lv_obj_get_child_by_name(tvoc_container, "tvoc"), text);

    sprintf(text, "%.1f", Temp_C);
    lv_label_set_text(lv_obj_get_child_by_name(temp_container, "temp"), text);

    sprintf(text, "%.0f", rel_humidity_percent);
    lv_label_set_text(lv_obj_get_child_by_name(hum_container, "humidity"), text);
    
    // Set AQI color
    // lv_obj_set_style_text_color(aqi_value, get_aqi_color(AQI), 0);
}

// Helper function for AQI colors
lv_color_t AirQualityDisplay::get_aqi_color(int aqi) {
    static const lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_GREEN),   // 1
        lv_palette_main(LV_PALETTE_YELLOW),  // 2
        lv_palette_main(LV_PALETTE_ORANGE),  // 3
        lv_palette_main(LV_PALETTE_RED),     // 4
        lv_palette_main(LV_PALETTE_PURPLE)   // 5
    };
    return (aqi >= 1 && aqi <= 5) ? colors[aqi-1] : lv_color_black();
}