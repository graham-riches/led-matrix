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
     * \param characters characters to render
     * \param origin origin to render at
     * \param red R color channel
     * \param green G color channel
     * \param blue B color channel
     * \param alpha A color channel
     */
    font_renderer(const std::vector<fonts::character>& characters, graphics::origin origin, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 1.0);

    /**
     * \brief render a sequence of characters on the screen
     * 
     * \param canvas existing frame canvas
     * \retval the drawing frame
     */
    frame& draw(frame& canvas);

    std::vector<fonts::character> characters;
    pixel color;
};

};  // namespace graphics