#pragma once

#include <util/ArrayView.h>
#include <cstdint>

// oh lord

struct Font
{
    const uint8_t char_height;
    const uint8_t char_width;
    const uint8_t char_spacing;
    const ArrayView<uint8_t> char_bitmap;

    const uint8_t ascii_begin;
    const uint8_t ascii_end;

    consteval Font(uint8_t char_height, uint8_t char_width, uint8_t char_spacing, uint8_t ascii_begin, uint8_t ascii_end, const uint8_t* char_bmp_data, size_t bmp_data_size)
        : char_height(char_height), char_width(char_width), char_spacing(char_spacing), ascii_begin(ascii_begin), ascii_end(ascii_end), char_bitmap(make_array_view(char_bmp_data, bmp_data_size))
    {
    }

    // ASCII 32 = (space) character
    // ASCII 126 = (~) character
    template<uint8_t char_height, uint8_t char_width, uint8_t char_spacing, uint8_t ascii_begin = 32, uint8_t ascii_end = 126>
    static consteval Font BuildFont(const uint8_t data[char_width * (ascii_end - ascii_begin)])
    {
        return Font(char_height, char_width, char_spacing, ascii_begin, ascii_end, data, char_width * (ascii_end - ascii_begin));
    }

    template<uint8_t char_height, uint8_t char_width, uint8_t char_spacing, uint8_t ascii_begin = 32, uint8_t ascii_end = 126>
    static consteval Font BuildFont(const uint8_t data[ascii_end - ascii_begin][char_width])
    {
        return Font(char_height, char_width, char_spacing, ascii_begin, ascii_end, data, char_width * (ascii_end - ascii_begin));
    }
};