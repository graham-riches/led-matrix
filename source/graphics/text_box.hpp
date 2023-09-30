/**
 * \file text_box.hpp
 * \brief contains rendering functionality for BDF fonts
 * \version 0.1
 * \date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "alignment.hpp"
#include "font.hpp"
#include "primatives.hpp"
#include <vector>

namespace graphics
{
struct text_box : public shape {

    text_box(const std::vector<fonts::character>& characters,
             graphics::origin origin,
             pixel& color,
             uint8_t width,
             uint8_t height,
             horizontal_alignment h_align = horizontal_alignment::left,
             vertical_alignment v_align = vertical_alignment::top);

    // Draw on the canvas and return it afterwards
    frame& draw(frame& canvas);

    std::vector<fonts::character> characters;
    pixel& color;
    uint8_t width;
    uint8_t height;
    horizontal_alignment h_align;
    vertical_alignment v_align;
};

};  // namespace graphics