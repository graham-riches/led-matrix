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

#include "font_renderer.hpp"

namespace graphics
{
    
font_renderer::font_renderer(const fonts::font& font, const envelope& envelope)
    : shape(envelope)
    , _font(font) { }

void draw(origin origin) { }

};  // namespace graphics
