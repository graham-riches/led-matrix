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
 * \param characters vector of characters to render
 * \param origin location to draw the 
 */
font_renderer::font_renderer(const std::vector<fonts::character>& characters, graphics::origin origin, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
: shape(origin)
, characters(characters)
, color(pixel{red, green, blue, alpha}) {}

/**
 * \brief render a sequence of characters on the screen
 * 
 * \param canvas existing frame canvas
 * \retval frame new frame
 */
frame& font_renderer::draw(frame& canvas) {        
    //!< get the frame size to make sure we don't draw outside the limits of the frame
    const auto width = canvas.get_width();
    const auto height = canvas.get_height();

    for (int character_count = 0; character_count < characters.size(); character_count++) {
        //!< get the current character
        auto character = characters[character_count];

        for (int i = 0; i < character.properties.b_box.height; i++) {
            //!< get the bit alignment of the font data within the character encoding
            uint32_t pixel_alignment = (character.properties.b_box.width / 8) << 8ul;

            //!< draw the row of characters
            for (int j = 0; j < character.properties.b_box.width; j++){                       
                if (character.bitmap[i] & (pixel_alignment >> j)){
                    auto offset = character.properties.b_box.width * character_count;
                    auto row_coordinate = _origin.x + offset + i;
                    auto column_coordinate = _origin.y + j;
                    if ((i < height) && (j < width)){
                        canvas.set_pixel(row_coordinate, column_coordinate, color);
                    }                    
                }
            }
        }        
    }
    return canvas;
}


};  // namespace graphics
