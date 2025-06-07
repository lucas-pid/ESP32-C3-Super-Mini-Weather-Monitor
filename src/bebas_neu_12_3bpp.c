/*******************************************************************************
 * Size: 12 px
 * Bpp: 3
 * Opts: --bpp 3 --size 12 --font BebasNeue-Regular.ttf --symbols 0x0021 - 0x00BF --format lvgl -o bebas_neu_12_3bpp.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef BEBAS_NEU_12_3BPP
#define BEBAS_NEU_12_3BPP 1
#endif

#if BEBAS_NEU_12_3BPP

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+002D "-" */
    0x2, 0xbf, 0xa6, 0xc0,

    /* U+0030 "0" */
    0x1b, 0xd0, 0xcc, 0x49, 0xd3, 0x4, 0xf, 0xfe,
    0x47, 0x4c, 0x16, 0x62, 0x40,

    /* U+0031 "1" */
    0x1, 0x28, 0xec, 0x7, 0x40, 0x7f, 0xf3, 0x40,

    /* U+0032 "2" */
    0x17, 0xd0, 0xd4, 0x49, 0xd3, 0x1, 0x29, 0xc,
    0x7, 0x64, 0x51, 0x2, 0x4e, 0x0, 0x93, 0x43,
    0x9b, 0x48,

    /* U+0042 "B" */
    0x9f, 0xa8, 0x3, 0x52, 0x0, 0x40, 0x80, 0x20,
    0x83, 0x82, 0x6, 0x86, 0x1, 0x1e, 0x1, 0x1e,
    0xd, 0x2c,

    /* U+0046 "F" */
    0x9f, 0xc0, 0xda, 0x0, 0x90, 0xe, 0x3e, 0x3,
    0xa0, 0x2, 0x7, 0xf0,

    /* U+0078 "x" */
    0xb4, 0x2c, 0xe8, 0xaa, 0x84, 0x24, 0x12, 0x80,
    0x61, 0x80, 0x61, 0x80, 0x26, 0xc1, 0x98, 0xe,
    0x40, 0xd0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 31, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 52, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 4, .adv_w = 77, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 17, .adv_w = 77, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 25, .adv_w = 77, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 78, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 61, .adv_w = 66, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 73, .adv_w = 78, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0xd, 0x10, 0x11, 0x12, 0x22, 0x26, 0x58
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 89, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 8, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    2, 8,
    6, 8,
    7, 2,
    8, 2
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -7, -1, -1, -7
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 4,
    .glyph_ids_size = 0
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
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 3,
    .kern_classes = 0,
    .bitmap_format = 1,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_montserrat_12;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t bebas_neu_12_3bpp = {
#else
lv_font_t bebas_neu_12_3bpp = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 9,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_12,
#endif
    .user_data = NULL,
};



#endif /*#if BEBAS_NEU_12_3BPP*/

