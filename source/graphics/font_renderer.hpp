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


namespace graphics
{

class font_renderer : public shape {
  public:
    /**
     * \brief Construct a new font renderer object
     * 
     * \param font the font object that contains the character encodings
     * \param envelope envelope object that provides the graphics framework with an idea of how much space each
     *                 font character will occupy
     */
    font_renderer(const fonts::font& font, const envelope& envelope);

    /**
     * \brief draw function that satisfies the shape interface (bad naming alert)
     * 
     * \param origin origin to draw the object at
     */
    void draw(origin origin);

  private:
    fonts::font _font;
};

};  // namespace graphics