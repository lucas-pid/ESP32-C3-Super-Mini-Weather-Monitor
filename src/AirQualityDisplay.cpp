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
    list = lv_obj_create(lv_scr_act());

    // Disable scroll bar
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    // Half the screen
    lv_obj_set_size(list, screen_width_pixels/2, screen_height_pixels);

    // Create label for each entry
    create_sensor_read_display(0, list);
    create_sensor_read_display(1, list);
    create_sensor_read_display(2, list);
    create_sensor_read_display(3, list);
    create_sensor_read_display(4, list);
}

void AirQualityDisplay::create_sensor_read_display(int row, lv_obj_t* parent_obj) {

    // Get height of parent object to divide the rows
    const int height_obj = lv_obj_get_height(parent_obj);

    // Define space between values and initial offset
    const int spacing       = 36;
    const int offset_y      = 45;

    // Define position of second column
    const int col_x         = 45;

    // Define offset on x position
    const int offset_x      = -15;

    // Compute spacing of current row
    const int row_y         = offset_y + spacing*row;

    //char* symbol;
    char symbol[16];    // Allocate enough space
    char unit[16];      //

    // Set order of values
    std::vector<const char*> values_order = {"aqi", "co2", "temp", "humidity", "tvoc"};

    // Get the label
    const char* current_label = values_order[row];

    // Define active label to point to different labels
    lv_obj_t** active_label = nullptr;

    if (strcmp(current_label, "aqi") == 0) {
        active_label = &aqi_value;
        strcpy(symbol, AQI_SYMBOL);
        strcpy(unit, "");

    } else if (strcmp(current_label, "co2") == 0){
        active_label = &eco2_value;
        strcpy(symbol, CO2_SYMBOL);
        strcpy(unit, "ppm");

    } else if (strcmp(current_label, "temp") == 0){
        active_label = &temp_value;
        strcpy(symbol, THERMOMETER_SYMBOL);
        strcpy(unit, "°C");

    } else if (strcmp(current_label, "humidity") == 0){
        active_label = &hum_value;
        strcpy(symbol, HUMIDITY_SYMBOL);
        strcpy(unit, "%");

    } else if (strcmp(current_label, "tvoc") == 0){
        active_label = &tvoc_value;
        strcpy(symbol, TVOC_SYMBOL);
        strcpy(unit, "ppb");

    } else {
        active_label = &aqi_value;
        strcpy(symbol, AQI_SYMBOL);
        strcpy(unit, "");
    }

    // create label for symbol
    lv_obj_t *symbol_label_obj = lv_label_create(parent_obj);
    lv_obj_set_height(symbol_label_obj, spacing);
    lv_obj_set_width(symbol_label_obj, col_x);
    lv_obj_set_pos(symbol_label_obj, offset_x, row_y);
    lv_label_set_text(symbol_label_obj, symbol);
    lv_obj_set_align(symbol_label_obj, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(symbol_label_obj, &symbols_AQI_36_3bpp, 0);

    // create label for number
    *active_label = lv_label_create(parent_obj);
    lv_obj_set_align(*active_label, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(*active_label, -17, row_y);
    lv_label_set_text(*active_label, "0");
    lv_obj_set_style_text_font(*active_label, &bebas_neu_24_3bpp, 0);

    // create label for unit
    lv_obj_t *unit_label = lv_label_create(parent_obj);
    lv_obj_align_to(unit_label, *active_label, LV_ALIGN_OUT_RIGHT_TOP, 0 , 5);
    lv_label_set_text(unit_label, unit);
    lv_obj_set_style_text_font(unit_label, &bebas_neu_12_3bpp, 0);
    
}

void AirQualityDisplay::update_display(int AQI, int TVOC_ppb, int eCO2_ppm, float Temp_C, float rel_humidity_percent) {
    lv_tick_set_cb(xTaskGetTickCount);
    lv_task_handler();
    lv_timer_handler();

    // Directly update
    char text[10];
    sprintf(text, "%d", AQI);
    lv_label_set_text(aqi_value, text);

    Serial.println("Function Reached");
    
    sprintf(text, "%d", eCO2_ppm);
    lv_label_set_text(eco2_value, text);

    sprintf(text, "%d", TVOC_ppb);
    lv_label_set_text(tvoc_value, text);

    sprintf(text, "%.1f", Temp_C);
    lv_label_set_text(temp_value, text);

    sprintf(text, "%.0f", rel_humidity_percent);
    lv_label_set_text(hum_value, text);
    
    // Set AQI color
    //lv_obj_set_style_text_color(aqi_value, get_aqi_color(AQI), 0);
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