/*******************************************************************************
 * Size: 35 px
 * Bpp: 3
 * Opts: --bpp 3 --size 35 --font fa-solid-900.ttf --range 62153 --format lvgl -o symbols_35_3bpp.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef SYMBOLS_35_3BPP
#define SYMBOLS_35_3BPP 1
#endif

#if SYMBOLS_35_3BPP

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F2C9 "" */
    0x3, 0xe2, 0xe4, 0x30, 0x3f, 0xf8, 0x26, 0xe8,
    0xda, 0x7a, 0x40, 0xfe, 0x59, 0x1, 0xe5, 0x88,
    0x1f, 0x1a, 0x7, 0xfb, 0x10, 0x3d, 0xc0, 0xff,
    0xe0, 0x40, 0x38, 0x90, 0x11, 0xdf, 0xa8, 0x19,
    0x10, 0x35, 0x3, 0x72, 0x1, 0x50, 0x32, 0x3,
    0x10, 0x22, 0x40, 0x64, 0x6, 0xe0, 0x7e, 0x60,
    0x78, 0x81, 0x10, 0x3f, 0xff, 0xe0, 0x7f, 0xf1,
    0x11, 0x3, 0xff, 0x8d, 0x70, 0x1f, 0xfc, 0x52,
    0x7, 0xff, 0xf5, 0x81, 0x10, 0x3c, 0xc0, 0x98,
    0x13, 0x20, 0x5c, 0xf, 0x10, 0x22, 0xc0, 0x40,
    0x33, 0x3, 0xd4, 0xd, 0x3, 0x3, 0x40, 0x8,
    0x2, 0x5, 0x81, 0x96, 0x2, 0x2c, 0x48, 0x1,
    0x58, 0x80, 0x6c, 0xc0, 0x90, 0x6, 0x6, 0x80,
    0x10, 0x11, 0x20, 0x2e, 0x10, 0x1e, 0x20, 0x81,
    0x32, 0x5, 0xc3, 0x3, 0xc4, 0x10, 0x26, 0xc0,
    0x48, 0x14, 0x6, 0x60, 0x10, 0x11, 0xc0, 0x45,
    0xb, 0xa, 0x90, 0x18, 0x1b, 0x30, 0x35, 0x21,
    0xea, 0xc2, 0x80, 0x64, 0x18, 0x1b, 0x42, 0x0,
    0xda, 0x3, 0x40, 0x10, 0x81, 0x9e, 0xfd, 0x90,
    0x18, 0xb0, 0x2e, 0x40, 0xff, 0xe0, 0x1c, 0x6,
    0x39, 0x1, 0xfe, 0x58, 0xf, 0xad, 0x20, 0x3c,
    0xb5, 0x3, 0xf9, 0x5f, 0xad, 0x6f, 0x48, 0x1c
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 350, .box_w = 20, .box_h = 36, .ofs_x = 1, .ofs_y = -5}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 62153, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
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



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t symbols_35_3bpp = {
#else
lv_font_t symbols_35_3bpp = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 36,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -3,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if SYMBOLS_35_3BPP*/

