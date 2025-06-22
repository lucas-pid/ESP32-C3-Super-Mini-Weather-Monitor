#include "AirQualityDisplay.h"

LV_FONT_DECLARE(weather_icons_25pt_3bpp);
LV_FONT_DECLARE(hum_icons_10pt_3bpp);

#define THERMOMETER_SYMBOL "\xEE\xA8\x94"
#define CO2_SYMBOL "\xEE\xA8\x95"
#define TVOC_SYMBOL "\xEE\xA8\x96"
#define AQI_SYMBOL "\xEE\xA8\x97"
#define HUMIDITY_SYMBOL "\xEE\xA8\x98"
#define DROP_SYMBOL "\xEE\xA8\x99"
#define SUNRISE_SYMBOL "\xEE\xA8\x9A"
#define SUNSET_SYMBOL "\xEE\xA8\x9B"
#define HUMIDITY_PERCENTAGE_SYMBOL "\xEE\xA8\x9C"

#define ICON_SYMBOL_01N "\xEE\xA8\x81"
#define ICON_SYMBOL_01D "\xEE\xA8\x82"
#define ICON_SYMBOL_02D "\xEE\xA8\x83"
#define ICON_SYMBOL_02N "\xEE\xA8\x84"
#define ICON_SYMBOL_03D "\xEE\xA8\x85"
#define ICON_SYMBOL_03N "\xEE\xA8\x86"
#define ICON_SYMBOL_04D "\xEE\xA8\x87"
#define ICON_SYMBOL_04N "\xEE\xA8\x88"
#define ICON_SYMBOL_09D "\xEE\xA8\x89"
#define ICON_SYMBOL_09N "\xEE\xA8\x9A"
#define ICON_SYMBOL_10D "\xEE\xA8\x8B"
#define ICON_SYMBOL_10N "\xEE\xA8\x8C"
#define ICON_SYMBOL_11D "\xEE\xA8\x8D"
#define ICON_SYMBOL_11N "\xEE\xA8\x8E"
#define ICON_SYMBOL_13D "\xEE\xA8\x90"
#define ICON_SYMBOL_13N "\xEE\xA8\x91"
#define ICON_SYMBOL_50D "\xEE\xA8\x92"
#define ICON_SYMBOL_50N "\xEE\xA8\x93"

// Styles
lv_style_t AirQualityDisplay::label_style;

void AirQualityDisplay::begin() {

    lv_init();
    lv_display_t *disp = lv_tft_espi_create(screen_width_pixels, screen_height_pixels, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270); 

    // Define label style
    // Basically removes all padding/outlines
    lv_style_set_pad_all(&label_style, 0);
    lv_style_set_outline_width(&label_style, 0);
    lv_style_set_border_width(&label_style, 0);
    
    create_gui();
}

void AirQualityDisplay::create_gui() {
    // Today weather display
    today_display_container = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(today_display_container, 2, 0);
    lv_obj_set_style_outline_width(today_display_container, 0, 0);
    lv_obj_set_style_border_width(today_display_container, 0, 0);
    lv_obj_set_scrollbar_mode(today_display_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(today_display_container, screen_width_pixels/2, 155);
    lv_obj_set_pos(today_display_container, 0, 0);
    create_today_display(today_display_container);

    // Time and date display
    time_display_container = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(time_display_container, 0, 0);
    lv_obj_set_scrollbar_mode(time_display_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(time_display_container, screen_width_pixels/2, 35);
    lv_obj_align_to(time_display_container, today_display_container, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    create_time_display(time_display_container);

    // Hourly forecast display
    hourly_forecast_display_container = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(hourly_forecast_display_container, 0, 0);
    lv_obj_set_size(hourly_forecast_display_container, screen_width_pixels/2, 205);
    lv_obj_set_scrollbar_mode(hourly_forecast_display_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align_to(hourly_forecast_display_container, time_display_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    create_hourly_forecast_display(hourly_forecast_display_container);

    // Next day forecast display
    create_nextday_forecast_display(today_display_container);

    // Create object to acomodate all the sensor readings
    // AQI
    aqi_container = lv_obj_create(lv_scr_act());
    create_sensor_template_display(aqi_container, screen_width_pixels/2, 29);
    lv_obj_align_to(aqi_container, today_display_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(lv_obj_get_child_by_name(aqi_container, "sensor_symbol"), AQI_SYMBOL);
    lv_label_set_text(lv_obj_get_child_by_name(aqi_container, "value_label"), "UNHEALTHY");
    lv_obj_set_style_text_align(lv_obj_get_child_by_name(aqi_container, "value_label"), LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(lv_obj_get_child_by_name(aqi_container, "value_label"), 95);
    lv_obj_set_align(lv_obj_get_child_by_name(aqi_container, "value_label"), LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(lv_obj_get_child_by_name(aqi_container, "unit_label"), "");
    
    // co2
    eco2_container = lv_obj_create(lv_scr_act());
    create_sensor_template_display(eco2_container, 65, 29);
    lv_obj_align_to(eco2_container, aqi_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(lv_obj_get_child_by_name(eco2_container, "sensor_symbol"), CO2_SYMBOL);
    lv_label_set_text(lv_obj_get_child_by_name(eco2_container, "value_label"), "1000");
    lv_label_set_text(lv_obj_get_child_by_name(eco2_container, "unit_label"), "ppm");
    
    // tvoc
    tvoc_container = lv_obj_create(lv_scr_act());
    create_sensor_template_display(tvoc_container, 65, 29);
    lv_obj_align_to(tvoc_container, eco2_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(lv_obj_get_child_by_name(tvoc_container, "sensor_symbol"), TVOC_SYMBOL);
    lv_label_set_text(lv_obj_get_child_by_name(tvoc_container, "value_label"), "9000");
    lv_label_set_text(lv_obj_get_child_by_name(tvoc_container, "unit_label"), "ppb");

    // temp
    temp_container = lv_obj_create(lv_scr_act());
    create_sensor_template_display(temp_container, 55, 29);
    lv_obj_align_to(temp_container, eco2_container, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    lv_label_set_text(lv_obj_get_child_by_name(temp_container, "sensor_symbol"), THERMOMETER_SYMBOL);
    lv_label_set_text(lv_obj_get_child_by_name(temp_container, "value_label"), "27.2°");
    lv_label_set_text(lv_obj_get_child_by_name(temp_container, "unit_label"), "");

    // humidity
    hum_container = lv_obj_create(lv_scr_act());
    create_sensor_template_display(hum_container, 55, 29);
    lv_obj_align_to(hum_container, temp_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(lv_obj_get_child_by_name(hum_container, "sensor_symbol"), HUMIDITY_SYMBOL);
    lv_label_set_text(lv_obj_get_child_by_name(hum_container, "value_label"), "90%");
    lv_label_set_text(lv_obj_get_child_by_name(hum_container, "unit_label"), "");
}

void AirQualityDisplay::create_nextday_forecast_display(lv_obj_t*& parent_obj){
    
    // Loop over the number of forecast days
    for (int ii = 0; ii < FORECAST_DAYS; ii++) {
        // Create daily information template
        nextday_forecast_display[ii] = lv_obj_create(parent_obj);
        create_dayly_template_display(nextday_forecast_display[ii]);

        // Align to previous
        if (ii > 0){
            lv_obj_align_to(nextday_forecast_display[ii], nextday_forecast_display[ii-1], LV_ALIGN_OUT_RIGHT_TOP, SPACING_SMALL, 0);
        }else if (ii == 0)
        {
            lv_obj_align_to(nextday_forecast_display[ii], lv_obj_get_child_by_name(today_display_container, "summary_label"), LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        }
    }
}

void AirQualityDisplay::create_sensor_template_display(lv_obj_t*& child_obj, int width, int height){

    // Remove padding
    lv_obj_set_style_pad_all(child_obj, 0, 0);
    // Remove outline
    lv_obj_set_style_outline_width(child_obj, 0, 0);
    // Set size
    lv_obj_set_size(child_obj, width, height);
    // Disable scroll bar
    lv_obj_set_scrollbar_mode(child_obj, LV_SCROLLBAR_MODE_OFF);

    // Icon
    lv_obj_t *icon_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(icon_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(icon_obj, &weather_icons_25pt_3bpp, 0);
    // Set size
    lv_obj_set_size(icon_obj, SYMBOL_FONT_SIZE, SYMBOL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(icon_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(icon_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Set initial text
    lv_label_set_text(icon_obj, THERMOMETER_SYMBOL);
    //Set name of object
    lv_obj_set_name(icon_obj, "sensor_symbol");

    // Sensor value label
    lv_obj_t *value_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(value_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(value_obj, &lv_font_montserrat_14, 0);
    // Set size
    lv_obj_set_size(value_obj, lv_pct(100), 14);
    // Set outline
    lv_obj_set_style_outline_width(value_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(value_obj, LV_TEXT_ALIGN_RIGHT, 0);
    // Set initial text
    lv_label_set_text(value_obj, "1000");
    //Set name of object
    lv_obj_set_name(value_obj, "value_label");

    // Sensor value unit label
    lv_obj_t *unit_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(unit_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(unit_obj, &lv_font_montserrat_10, 0);
    // Set size
    lv_obj_set_size(unit_obj, lv_pct(100), 10);
    // Align below
    lv_obj_align_to(unit_obj, value_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    // Set outline
    lv_obj_set_style_outline_width(unit_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(unit_obj, LV_TEXT_ALIGN_RIGHT, 0);
    // Set initial text
    lv_label_set_text(unit_obj, "ppm");
    //Set name of object
    lv_obj_set_name(unit_obj, "unit_label");

}

void AirQualityDisplay::create_sunrise_sunset_template_display(lv_obj_t*& child_obj){

    // Remove padding
    lv_obj_set_style_pad_all(child_obj, 0, 0);
    // Remove outline
    lv_obj_set_style_outline_width(child_obj, 0, 0);
    // Set size
    lv_obj_set_size(child_obj, 60, 55);
    // Disable scroll bar
    lv_obj_set_scrollbar_mode(child_obj, LV_SCROLLBAR_MODE_OFF);

    // Sunrise icon
    lv_obj_t *sunrise_icon_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(sunrise_icon_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(sunrise_icon_obj, &weather_icons_25pt_3bpp, 0);
    // Set size
    lv_obj_set_size(sunrise_icon_obj, SYMBOL_FONT_SIZE, SYMBOL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(sunrise_icon_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(sunrise_icon_obj, LV_TEXT_ALIGN_CENTER, 0);
    // Set initial text
    lv_label_set_text(sunrise_icon_obj, SUNRISE_SYMBOL);
    //Set name of object
    lv_obj_set_name(sunrise_icon_obj, "sunrise_symbol");

    // Sunrise label
    lv_obj_t *sunrise_label_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(sunrise_label_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(sunrise_label_obj, &lv_font_montserrat_10, 0);
    // Set size
    lv_obj_set_size(sunrise_label_obj, 30, SMALL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(sunrise_label_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(sunrise_label_obj, LV_TEXT_ALIGN_CENTER, 0);
    // Align to symbol
    lv_obj_align_to(sunrise_label_obj, sunrise_icon_obj, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    // Set initial text
    lv_label_set_text(sunrise_label_obj, "99:99");
    //Set name of object
    lv_obj_set_name(sunrise_label_obj, "sunrise_label");

    // Sunset icon
    lv_obj_t *sunset_icon_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(sunset_icon_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(sunset_icon_obj, &weather_icons_25pt_3bpp, 0);
    // Set size
    lv_obj_set_size(sunset_icon_obj, SYMBOL_FONT_SIZE, SYMBOL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(sunset_icon_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(sunset_icon_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Align to sunrise
    lv_obj_align_to(sunset_icon_obj, sunrise_icon_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    // Set initial text
    lv_label_set_text(sunset_icon_obj, SUNSET_SYMBOL);
    //Set name of object
    lv_obj_set_name(sunset_icon_obj, "sunset_symbol");

    // Sunset label
    lv_obj_t *sunset_label_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(sunset_label_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(sunset_label_obj, &lv_font_montserrat_10, 0);
    // Set size
    lv_obj_set_size(sunset_label_obj, 30, SMALL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(sunset_label_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(sunset_label_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Align to symbol
    lv_obj_align_to(sunset_label_obj, sunset_icon_obj, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    // Set initial text
    lv_label_set_text(sunset_label_obj, "99:99");
    //Set name of object
    lv_obj_set_name(sunset_label_obj, "sunset_label");

}

void AirQualityDisplay::create_dayly_template_display(lv_obj_t*& child_obj, int width, int height){
    // Remove padding
    lv_obj_set_style_pad_all(child_obj, 0, 0);
    // Remove outline
    lv_obj_set_style_outline_width(child_obj, 0, 0);
    // Set size
    lv_obj_set_size(child_obj, width, height);
    // Disable scroll bar
    lv_obj_set_scrollbar_mode(child_obj, LV_SCROLLBAR_MODE_OFF);
    
    //Label with name of the day
    lv_obj_t *day_label_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(day_label_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(day_label_obj, &lv_font_montserrat_14, 0);
    // Set size
    lv_obj_set_size(day_label_obj, lv_pct(100), MEDIUM_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(day_label_obj, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(day_label_obj, LV_TEXT_ALIGN_CENTER, 0);
    // Set initial text
    lv_label_set_text(day_label_obj, "XXX");
    //Set name of object
    lv_obj_set_name(day_label_obj, "day_label");

    // Forecast Icon
    lv_obj_t *forecast_icon_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(forecast_icon_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(forecast_icon_obj, &weather_icons_25pt_3bpp, 0);
    // Set size
    lv_obj_set_size(forecast_icon_obj, SYMBOL_FONT_SIZE, SYMBOL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(forecast_icon_obj, 0, 0);
    // Align to day
    lv_obj_align_to(forecast_icon_obj, day_label_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(forecast_icon_obj, LV_TEXT_ALIGN_CENTER, 0);
    // Set initial text
    lv_label_set_text(forecast_icon_obj, ICON_SYMBOL_10D);
    //Set name of object
    lv_obj_set_name(forecast_icon_obj, "forecast_symbol");

    // Max temperature label
    lv_obj_t *max_temp_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(max_temp_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(max_temp_obj, &lv_font_montserrat_14, 0);
    // Set size
    lv_obj_set_size(max_temp_obj, 30, MEDIUM_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(max_temp_obj, 0, 0);
    // Align to icon
    lv_obj_align_to(max_temp_obj, forecast_icon_obj, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
    // Set text alignment
    lv_obj_set_style_text_align(max_temp_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Set initial text
    lv_label_set_text(max_temp_obj, "99°");
    //Set name of object
    lv_obj_set_name(max_temp_obj, "max_temp_label");

    // Min temperature label
    lv_obj_t *min_temp_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(min_temp_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(min_temp_obj, &lv_font_montserrat_10, 0);
    // Set size
    lv_obj_set_size(min_temp_obj, 30, SMALL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(min_temp_obj, 0, 0);
    // Align to icon
    lv_obj_align_to(min_temp_obj, max_temp_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(min_temp_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Set initial text
    lv_label_set_text(min_temp_obj, "99°");
    //Set name of object
    lv_obj_set_name(min_temp_obj, "min_temp_label");

    // Rain Probability
    lv_obj_t *rain_prop_obj = lv_label_create(child_obj);
    // Use label style
    lv_obj_add_style(rain_prop_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(rain_prop_obj, &lv_font_montserrat_10, 0);
    // Set size
    lv_obj_set_size(rain_prop_obj, 30, SMALL_FONT_SIZE);
    // Set outline
    lv_obj_set_style_outline_width(rain_prop_obj, 0, 0);
    // Align to icon
    lv_obj_align_to(rain_prop_obj, min_temp_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(rain_prop_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Set initial text
    lv_label_set_text(rain_prop_obj, "99%");
    //Set name of object
    lv_obj_set_name(rain_prop_obj, "rain_prob_label");

}

void AirQualityDisplay::create_hourly_forecast_display(lv_obj_t*& parent_obj){

    // Loop through integers
    for (int ii = 0; ii < FORECAST_HOURS; ii++) {
        // Create object container for the hour forecast display
        hourly_forecast_display[ii] = lv_obj_create(parent_obj);
        // Remove padding
        lv_obj_set_style_pad_all(hourly_forecast_display[ii], 0, 0);
        // Remove padding
        lv_obj_set_style_border_width(hourly_forecast_display[ii], 0, 0);
        // Set size
        lv_obj_set_size(hourly_forecast_display[ii], lv_pct(100), SYMBOL_FONT_SIZE+1);
        // Disable scroll bar
        lv_obj_set_scrollbar_mode(hourly_forecast_display[ii], LV_SCROLLBAR_MODE_OFF);
        // Set alignment with previous object
        if (ii > 0){
            lv_obj_align_to(hourly_forecast_display[ii], hourly_forecast_display[ii-1], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        }

        // ////////////// Labels
        
        // Time
        lv_obj_t *time_label_obj = lv_label_create(hourly_forecast_display[ii]);
        // Use label style
        lv_obj_add_style(time_label_obj, &label_style, 0);
        // Set font
        lv_obj_set_style_text_font(time_label_obj, &lv_font_montserrat_14, 0);
        // Set size
        lv_obj_set_size(time_label_obj, 40, 15);
        // Align to the center
        lv_obj_set_align(time_label_obj, LV_ALIGN_LEFT_MID);
        // Set text alignment
        lv_obj_set_style_text_align(time_label_obj, LV_TEXT_ALIGN_CENTER, 0);
        // Set initial text
        lv_label_set_text(time_label_obj, "00:00");
        //Set name of object
        lv_obj_set_name(time_label_obj, "time_label");

        // Forecast Icon
        lv_obj_t *forecast_icon_obj = lv_label_create(hourly_forecast_display[ii]);
        // Use label style
        lv_obj_add_style(forecast_icon_obj, &label_style, 0);
        // Set font
        lv_obj_set_style_text_font(forecast_icon_obj, &weather_icons_25pt_3bpp, 0);
        // Set size
        lv_obj_set_size(forecast_icon_obj, SYMBOL_FONT_SIZE, lv_pct(100));
        // Align to time
        lv_obj_set_pos(forecast_icon_obj, 40, 0);
        //lv_obj_align_to(forecast_icon_obj, time_label_obj, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
        // Set text alignment
        lv_obj_set_style_text_align(forecast_icon_obj, LV_TEXT_ALIGN_CENTER, 0);
        // Set initial text
        lv_label_set_text(forecast_icon_obj, ICON_SYMBOL_10N);
        //Set name of object
        lv_obj_set_name(forecast_icon_obj, "forecast_symbol");

        // Rain probability symbol
        lv_obj_t *rain_prob_icon_obj = lv_label_create(hourly_forecast_display[ii]);
        // Use label style
        lv_obj_add_style(rain_prob_icon_obj, &label_style, 0);
        // // Set font
        lv_obj_set_style_text_font(rain_prob_icon_obj, &hum_icons_10pt_3bpp, 0);
        // // Set size
        lv_obj_set_size(rain_prob_icon_obj, 25, SMALL_FONT_SIZE);
        // Align to forecast icon
        lv_obj_align_to(rain_prob_icon_obj, forecast_icon_obj, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
        // Set text alignment
        lv_obj_set_style_text_align(rain_prob_icon_obj, LV_TEXT_ALIGN_CENTER, 0);
        // Set initial text
        lv_label_set_text(rain_prob_icon_obj, HUMIDITY_SYMBOL);
        //Set name of object
        lv_obj_set_name(rain_prob_icon_obj, "rain_prob_symbol");

        // Rain probability label
        lv_obj_t *rain_prob_label_obj = lv_label_create(hourly_forecast_display[ii]);
        // Use label style
        lv_obj_add_style(rain_prob_label_obj, &label_style, 0);
        // // Set font
        lv_obj_set_style_text_font(rain_prob_label_obj, &lv_font_montserrat_10, 0);
        // // Set size
        lv_obj_set_size(rain_prob_label_obj, 25, SMALL_FONT_SIZE+5);
        // Align to forecast icon
        lv_obj_align_to(rain_prob_label_obj, rain_prob_icon_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 1);
        // Set text alignment
        lv_obj_set_style_text_align(rain_prob_label_obj, LV_TEXT_ALIGN_CENTER, 0);
        // Set initial text
        lv_label_set_text(rain_prob_label_obj, "00");
        //Set name of object
        lv_obj_set_name(rain_prob_label_obj, "rain_prob_label");

        // Temperature label
        lv_obj_t *temp_label_obj = lv_label_create(hourly_forecast_display[ii]);
        // Use label style
        lv_obj_add_style(temp_label_obj, &label_style, 0);
        // // Set font
        lv_obj_set_style_text_font(temp_label_obj, &lv_font_montserrat_14, 0);
        // // Set size
        lv_obj_set_size(temp_label_obj, 30, 15);
        // Align to forecast icon
        lv_obj_align_to(temp_label_obj, forecast_icon_obj, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
        // Set text alignment
        lv_obj_set_style_text_align(temp_label_obj, LV_TEXT_ALIGN_LEFT, 0);
        // Set initial text
        lv_label_set_text(temp_label_obj, "25°");
        //Set name of object
        lv_obj_set_name(temp_label_obj, "temperature_label");
    }
}

void AirQualityDisplay::create_time_display(lv_obj_t*& parent_obj){

    // Create label for time
    lv_obj_t *time_label_obj = lv_label_create(parent_obj);
    // Use label style
    lv_obj_add_style(time_label_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(time_label_obj, &lv_font_montserrat_14, 0);
    // Set size (== the font size)
    lv_obj_set_size(time_label_obj, lv_pct(100), MEDIUM_FONT_SIZE);
    // Align on the top centered
    lv_obj_set_align(time_label_obj, LV_ALIGN_TOP_MID);
    // Set text alignment
    lv_obj_set_style_text_align(time_label_obj, LV_TEXT_ALIGN_CENTER, 0);
    // Set initial text
    lv_label_set_text(time_label_obj, "17:50:36");
    //Set name of object
    lv_obj_set_name(time_label_obj, "time_label");

    // Create label for date
    lv_obj_t *date_label_obj = lv_label_create(parent_obj);
    // Use label style
    lv_obj_add_style(date_label_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(date_label_obj, &lv_font_montserrat_14, 0);
    // Set size (== the font size)
    lv_obj_set_size(date_label_obj, lv_pct(100), MEDIUM_FONT_SIZE);
    // Align bellow time centered
    lv_obj_align_to(date_label_obj, time_label_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    // Set text alignment
    lv_obj_set_style_text_align(date_label_obj, LV_TEXT_ALIGN_CENTER, 0);
    // Set initial text
    lv_label_set_text(date_label_obj, "07/06/2025");
    //Set name of object
    lv_obj_set_name(date_label_obj, "date_label");

}

void AirQualityDisplay::create_today_display(lv_obj_t*& parent_obj){

    // Create label with name of the city
    lv_obj_t *city_label_obj = lv_label_create(parent_obj);
    // Use label style
    lv_obj_add_style(city_label_obj, &label_style, 0);
    // Set font
    lv_obj_set_style_text_font(city_label_obj, &lv_font_montserrat_14, 0);
    // Set size (== twice the font size)
    lv_obj_set_size(city_label_obj, lv_pct(100), 32);
    // Set initial text
    lv_label_set_text(city_label_obj, display_data.city_name);
    // Center alignment
    lv_obj_set_style_text_align(city_label_obj, LV_TEXT_ALIGN_CENTER, 0);
    //Set name of object
    lv_obj_set_name(city_label_obj, "city_label");

    // Today's forecast weather template
    today_display = lv_obj_create(parent_obj);
    create_dayly_template_display(today_display);
    lv_obj_align_to(today_display, city_label_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(lv_obj_get_child_by_name(today_display, "day_label"), "TODAY");

    // sunset sunrise template
    sunset_sunrise_display_container = lv_obj_create(parent_obj);
    create_sunrise_sunset_template_display(sunset_sunrise_display_container);
    lv_obj_align_to(sunset_sunrise_display_container, today_display, LV_ALIGN_OUT_RIGHT_TOP, SPACING_SMALL, 0);

    // Description label
    lv_obj_t *description_label_obj = lv_label_create(parent_obj);
    // Use label style
    lv_obj_add_style(description_label_obj, &label_style, 0);
    // Set size. Size of the font and 50% of the today section
    lv_obj_set_size(description_label_obj, lv_pct(100), SMALL_FONT_SIZE+2);
    // Align bellow the today
    lv_obj_align_to(description_label_obj, today_display, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    // Set text alignment
    lv_obj_set_style_text_align(description_label_obj, LV_TEXT_ALIGN_LEFT, 0);
    // Set font
    lv_obj_set_style_text_font(description_label_obj, &lv_font_montserrat_10, 0);
    // Set text
    lv_label_set_text(description_label_obj, "Sunny day, go to the beach.");
    // Set scroll mode
    lv_label_set_long_mode(description_label_obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
    //Set name of object
    lv_obj_set_name(description_label_obj, "summary_label");

}

void AirQualityDisplay::update_forecast() {

    // Update current weather display
    char text[10];
    sprintf(text, "%d°", display_data.daily[0].temp_max_C);
    lv_label_set_text(lv_obj_get_child_by_name(today_display, "max_temp_label"), text);
    sprintf(text, "%d°", display_data.daily[0].temp_min_C);
    lv_label_set_text(lv_obj_get_child_by_name(today_display, "min_temp_label"), text);
    sprintf(text, "%d%%", display_data.daily[0].pop_percent);
    lv_label_set_text(lv_obj_get_child_by_name(today_display, "rain_prob_label"), text);
    sprintf(text, "%02d:%02d", display_data.daily[0].sunrise_hour_min[0], display_data.daily[0].sunrise_hour_min[1]);
    lv_label_set_text(lv_obj_get_child_by_name(sunset_sunrise_display_container, "sunrise_label"), text);
    sprintf(text, "%02d:%02d", display_data.daily[0].sunset_hour_min[0], display_data.daily[0].sunset_hour_min[1]);
    lv_label_set_text(lv_obj_get_child_by_name(sunset_sunrise_display_container, "sunset_label"), text);
    char* icon_text = get_symbol_text(display_data.daily[0].icon);
    lv_label_set_text(lv_obj_get_child_by_name(today_display, "forecast_symbol"), icon_text);
    const char* main_char = display_data.daily[0].summary.c_str();
    lv_label_set_text(lv_obj_get_child_by_name(today_display_container, "summary_label"), main_char);

    // Update hourly
    for (int ii = 0; ii < FORECAST_HOURS; ii++){
        sprintf(text, "%d°", display_data.hourly[ii].temp_C);
        lv_label_set_text(lv_obj_get_child_by_name(hourly_forecast_display[ii], "temperature_label"), text);
        sprintf(text, "%02d:00", display_data.hourly[ii].hour_24h);
        lv_label_set_text(lv_obj_get_child_by_name(hourly_forecast_display[ii], "time_label"), text);
        sprintf(text, "%d%%", display_data.hourly[ii].pop_percent);
        lv_label_set_text(lv_obj_get_child_by_name(hourly_forecast_display[ii], "rain_prob_label"), text);
        icon_text = get_symbol_text(display_data.hourly[ii].icon);
        lv_label_set_text(lv_obj_get_child_by_name(hourly_forecast_display[ii], "forecast_symbol"), icon_text);
    }

    // Update daily
    for (int ii = 0; ii < FORECAST_DAYS; ii++){
        sprintf(text, "%d°", display_data.daily[ii+1].temp_max_C);
        lv_label_set_text(lv_obj_get_child_by_name(nextday_forecast_display[ii], "max_temp_label"), text);
        sprintf(text, "%d°", display_data.daily[ii+1].temp_min_C);
        lv_label_set_text(lv_obj_get_child_by_name(nextday_forecast_display[ii], "min_temp_label"), text);
        sprintf(text, "%d%%", display_data.daily[ii+1].pop_percent);
        lv_label_set_text(lv_obj_get_child_by_name(nextday_forecast_display[ii], "rain_prob_label"), text);
        icon_text = get_symbol_text(display_data.daily[ii+1].icon);
        lv_label_set_text(lv_obj_get_child_by_name(nextday_forecast_display[ii], "forecast_symbol"), icon_text);
        char* wday_text = getWeekdayShort(display_data.daily[ii+1].day_int);
        lv_label_set_text(lv_obj_get_child_by_name(nextday_forecast_display[ii], "day_label"), wday_text);
    }
}

char* AirQualityDisplay::get_symbol_text(String iconCode){

    if (iconCode == "01d") {
        return ICON_SYMBOL_01D;
    } else if (iconCode == "01n") {
        return ICON_SYMBOL_01N;
    } else if (iconCode == "02d") {
        return ICON_SYMBOL_02D;
    } else if (iconCode == "02n") {
        return ICON_SYMBOL_02N;
    } else if (iconCode == "03d" || iconCode == "03n") {
        return ICON_SYMBOL_03D;
    } else if (iconCode == "04d" || iconCode == "04n") {
        return ICON_SYMBOL_04D;
    } else if (iconCode == "09d" || iconCode == "09n") {
        return ICON_SYMBOL_09D;
    } else if (iconCode == "10d") {
        return ICON_SYMBOL_10D;
    } else if (iconCode == "10n") {
        return ICON_SYMBOL_10N;
    } else if (iconCode == "11d" || iconCode == "11n") {
        return ICON_SYMBOL_11D;
    } else if (iconCode == "13d" || iconCode == "13n") {
        return ICON_SYMBOL_13D;
    } else if (iconCode == "50d" || iconCode == "50n") {
        return ICON_SYMBOL_50D;
    } else {
        return ICON_SYMBOL_13D;
    }
}

char* AirQualityDisplay::getWeekdayShort(int weekday) {
  switch (weekday) {
    case 0: return "SUN";
    case 1: return "MON";
    case 2: return "TUE";
    case 3: return "WED";
    case 4: return "THU";
    case 5: return "FRI";
    case 6: return "SAT";
    default: return "UNK"; // Unknown
  }
}

char* AirQualityDisplay::get_aqi_txt(int aqi) {
  switch (aqi) {
    case 5: return "UNHEALTHY";
    case 4: return "POOR";
    case 3: return "MODERATE";
    case 2: return "GOOD";
    case 1: return "EXCELLENT";
    default: return "UNK"; // Unknown
  }
}

void AirQualityDisplay::update_time() {
    // Update time
    lv_label_set_text(lv_obj_get_child_by_name(time_display_container, "time_label"),  display_data.time_char);
}

void AirQualityDisplay::update_sensor() {
    // Update all sensors
    char text[10];
    // Temperature
    sprintf(text, "%.1f°", display_data.sensor.Temp_C);
    lv_label_set_text(lv_obj_get_child_by_name(temp_container, "value_label"),  text);
    // co2
    sprintf(text, "%d", display_data.sensor.eCO2_ppm);
    lv_label_set_text(lv_obj_get_child_by_name(eco2_container, "value_label"),  text);
    // tvoc
    sprintf(text, "%d", display_data.sensor.TVOC_ppb);
    lv_label_set_text(lv_obj_get_child_by_name(tvoc_container, "value_label"),  text);
    // humidity
    sprintf(text, "%.0f%%", display_data.sensor.rel_humidity_percent);
    lv_label_set_text(lv_obj_get_child_by_name(hum_container, "value_label"),  text);
    // aqi
    char* AQI_text = get_aqi_txt(display_data.sensor.AQI);
    lv_label_set_text(lv_obj_get_child_by_name(aqi_container, "value_label"),  AQI_text);
    lv_color_t aqi_color = get_aqi_color(display_data.sensor.AQI); // get background color
    lv_obj_set_style_bg_color(aqi_container, aqi_color, 0);
}

// Helper function for AQI colors
lv_color_t AirQualityDisplay::get_aqi_color(int aqi) {
    static const lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_BLUE),       // 1
        lv_palette_main(LV_PALETTE_GREEN),      // 2
        lv_palette_main(LV_PALETTE_YELLOW),     // 3
        lv_palette_main(LV_PALETTE_ORANGE),     // 4
        lv_palette_main(LV_PALETTE_RED)         // 5
    };
    return (aqi >= 1 && aqi <= 5) ? colors[aqi-1] : lv_color_white();
}