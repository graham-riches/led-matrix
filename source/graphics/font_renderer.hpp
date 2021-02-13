/**
 * \file font_renderer.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief contains rendering functionality for BDF fonts
 * \version 0.1
 * \date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 * \details Note this is probably going to be moved around and restructured once the graphics library
 *          starts to come together in my head. Just putting some thoughts down now to see what shakes loose.
 */

#pragma once

/********************************** Includes *******************************************/
#include "font.hpp"
#include "primatives.hpp"
#include <vector>


namespace graphics
{
/********************************** Types *******************************************/
/**
 * \brief shape type that handles drawing bitmapped fonts
 */
struct font_renderer : public shape {  
    /**
     * \brief Construct a new font renderer object
     * 
     * \param characters vector of characters to render
     * \param origin location to draw the 
     */
    font_renderer(const std::vector<fonts::character>& characters, graphics::origin origin);

    /**
     * \brief render a sequence of characters on the screen
     * 
     * \param previous_frame existing frame canvas
     * \param red text color red channel
     * \param green text color green channel
     * \param blue text color blue channel
     * \param alpha text alpha: default is solid
     * \retval frame new frame
     */
    frame draw(const frame& previous_frame, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha=1.0);


    std::vector<fonts::character> characters;
};

};  // namespace graphics