/**
 * \file font_renderer.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief contains functions and methods for drawing font objects
 * \version 0.1
 * \date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "font_renderer.hpp"

namespace graphics
{
/********************************** Function Definitions *******************************************/
/**
 * \brief Construct a new font renderer object
 * 
 * \param characters characters to render
 * \param origin origin to render at
 * \param red R color channel
 * \param green G color channel
 * \param blue B color channel     
 */
font_renderer::font_renderer(const std::vector<fonts::character>& characters,
                             graphics::origin origin,
                             uint8_t red,
                             uint8_t green,
                             uint8_t blue,
                             text_wrap_mode mode)
    : shape(origin)
    , characters(characters)
    , wrap_mode(mode)
    , color(pixel{red, green, blue}) { }

/**
 * \brief render a sequence of characters on the screen
 * 
 * \param canvas existing frame canvas
 * \retval frame new frame
 */
frame& font_renderer::draw(frame& canvas) {
    //!< get the frame size to make sure we don't draw outside the limits of the frame
    const auto width = canvas.width();
    const auto height = canvas.height();

    int y_offset{0};
    int x_offset{0};

    for ( int character_count = 0; character_count < characters.size(); character_count++ ) {
        //!< get the current character
        auto character = characters[character_count];

        //!< get the bit alignment of the font data within the character encoding
        auto bbox = character.properties.b_box;        
        auto alignment = (bbox.width % 8 == 0) ? (bbox.width / 8) : (bbox.width / 8) + 1;
        uint32_t pixel_alignment = alignment << 8ul;

        //!< check for text wrapping and update the row draw position if required
        auto wraps = (x_offset + bbox.width >= width);
        x_offset = (wraps && (wrap_mode == text_wrap_mode::wrap)) ? 0 : x_offset;
        y_offset = (wraps && (wrap_mode == text_wrap_mode::wrap)) ? y_offset + bbox.height : y_offset;

        //!< draw the character
        for ( int j = 0; j < character.properties.b_box.height; j++ ) {
            for ( int i = 0; i < character.properties.b_box.width; i++ ) {
                if ( character.bitmap[j] & (pixel_alignment >> i) ) {                    
                    auto x = _origin.x + x_offset + i;
                    auto y = _origin.y + y_offset + j;                    
                    canvas.set_pixel(x, y, color);                    
                }
            }
        }

        //!< update the x-draw position for the next character
        x_offset += bbox.width;        
    }
    return canvas;
}

};  // namespace graphics
