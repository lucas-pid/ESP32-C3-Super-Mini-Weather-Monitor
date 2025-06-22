/*******************************************************************************
 * Size: 10 px
 * Bpp: 3
 * Opts: --bpp 3 --size 10 --font WeatherIcons.ttf --range 59928,59929,59932 --format lvgl -o hum_icons_10pt_3bpp.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef HUM_ICONS_10PT_3BPP
#define HUM_ICONS_10PT_3BPP 1
#endif

#if HUM_ICONS_10PT_3BPP

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+EA18 "" */
    0x2, 0x70, 0xe, 0x71, 0xd0, 0x25, 0x0, 0x28,
    0x2, 0x81, 0x99, 0x20, 0x3c, 0x88, 0x1e, 0x21,
    0x81, 0x8a, 0x12, 0x9, 0x3c, 0x0,

    /* U+EA19 "" */
    0x3, 0xfe, 0xb8, 0x81, 0xae, 0xfc, 0x40, 0x32,
    0xcf, 0xe0, 0x16, 0x0, 0x29, 0xb, 0x20, 0x5b,
    0xb, 0xa0, 0x47, 0x84, 0x22, 0x35, 0x88, 0x14,
    0x8e, 0x0,

    /* U+EA1C "" */
    0x3, 0xfe, 0xb8, 0xe, 0xba, 0x60, 0x26, 0x5a,
    0xb4, 0x2, 0xf6, 0x80, 0x12, 0x6c, 0x23, 0xc8,
    0xb3, 0x6a, 0x72, 0x1d, 0xcf, 0xa4, 0x14, 0x6d,
    0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 160, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 22, .adv_w = 160, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 48, .adv_w = 160, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 0, 0, 2
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 59928, .range_length = 5, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 5, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 3,
    .kern_classes = 0,
    .bitmap_format = 1,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_montserrat_10;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t hum_icons_10pt_3bpp = {
#else
lv_font_t hum_icons_10pt_3bpp = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 9,          /*The maximum line height required by the font*/
    .base_line = -1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_10,
#endif
    .user_data = NULL,
};



#endif /*#if HUM_ICONS_10PT_3BPP*/

