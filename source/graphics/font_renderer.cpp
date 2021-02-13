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
font_renderer::font_renderer(const std::vector<fonts::character>& characters, graphics::origin origin) 
: shape(origin)
, characters(characters) {}

/**
 * \brief render a sequence of characters on the screen
 * 
 * \param previous_frame existing frame canvas
 * \param red text color red channel
 * \param green text color green channel
 * \param blue text color blue channel
 * \param alpha text alpha
 * \retval frame new frame
 */
frame font_renderer::draw(const frame& previous_frame, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    //!< copy construct a new frame to draw into
    frame new_frame = previous_frame;

    //!< set the text pixels to all be the same
    pixel text_pixel{red, green, blue, alpha};
    
    //!< get the frame size to make sure we don't draw outside the limits of the frame
    const auto width = previous_frame.width;
    const auto height = previous_frame.height;

    for (int character_count = 0; character_count < characters.size(); character_count++) {
        //!< get the current character
        auto character = characters[character_count];

        for (int i = 0; i < character.properties.b_box.height; i++) {
            //!< get the bit alignment of the font data within the character encoding
            uint32_t pixel_alignment = (character.properties.b_box.width / 8) << 8ul;

            for (int j = 0; j < character.properties.b_box.width; j++){                       
                if (character.bitmap[i] & (pixel_alignment >> j)){
                    auto offset = character.properties.b_box.width * character_count;
                    auto row_coordinate = _origin.x + offset + i;
                    auto column_coordinate = _origin.y + j;
                    if 
                    new_frame.set_pixel(row_coordinate, column_coordinate, text_pixel);
                }
            }
        }        
    }
    return new_frame;
}


};  // namespace graphics
