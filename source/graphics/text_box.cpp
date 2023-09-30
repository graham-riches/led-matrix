/**
 * \file text_box.cpp
 * \brief contains functions and methods for drawing font objects
 * \version 0.1
 * \date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "text_box.hpp"

namespace graphics
{
text_box::text_box(const std::vector<fonts::character>& characters,
                   graphics::origin origin,
                   pixel& color,
                   uint8_t width,
                   uint8_t height,
                   horizontal_alignment h_align,
                   vertical_alignment v_align)
    : shape(origin)
    , characters(characters)
    , color(color)
    , width(width)
    , height(height)
    , h_align(h_align)
    , v_align(v_align) { }

frame& text_box::draw(frame& canvas) {
    auto char_width = characters[0].properties.b_box.width;
    auto char_height = characters[0].properties.b_box.height;
    auto string_width = characters.size() * char_width;
    int x_position = _origin.x;
    int y_position = _origin.y;

    if ( string_width < width ) {
        if ( h_align == horizontal_alignment::center ) {
            x_position += (width - string_width) / 2;
        } else if ( h_align == horizontal_alignment::right ) {
            x_position += (width - string_width);
        }
    }

    if ( char_height < height ) {
        if ( v_align == vertical_alignment::center ) {
            y_position += (height - char_height) / 2;
        } else if ( v_align == vertical_alignment::bottom ) {
            y_position += height - char_height;
        }
    }

    for ( int character_count = 0; character_count < characters.size(); character_count++ ) {        
        auto character = characters[character_count];        
        auto& bbox = character.properties.b_box;

        // fonts larger than 8 bits will be encoded as 16-bit values with padding right-aligned
        unsigned pixel_shift = (bbox.width > 8) ? 15 : 7;

        // draw the character
        for ( int j = 0; j < character.properties.b_box.height; j++ ) {
            auto bitmap = character.bitmap[j];
            for ( int i = 0; i < character.properties.b_box.width; i++ ) {
                if ( bitmap & (0x01 << (pixel_shift - i)) ) {
                    auto x = x_position + i;
                    auto y = y_position + j;
                    canvas.set_pixel(x, y, color);
                }
            }
        }

        // update the x-draw position for the next character
        x_position += bbox.width;
    }
    return canvas;
}

};  // namespace graphics
