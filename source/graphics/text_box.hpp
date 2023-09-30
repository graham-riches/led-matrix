// RGB LED Matrix Graphics Library

#pragma once

#include "alignment.hpp"
#include "font.hpp"
#include "shape.hpp"
#include <vector>

namespace graphics
{
struct text_box : protected shape {

    text_box(const std::vector<fonts::character>& characters,
             graphics::origin origin,
             pixel& color,
             uint8_t width,
             uint8_t height,
             horizontal_alignment h_align = horizontal_alignment::left,
             vertical_alignment v_align = vertical_alignment::top);

    // Draw on the canvas
    void draw(canvas& canvas);

    std::vector<fonts::character> characters;
    pixel& color;
    uint8_t width;
    uint8_t height;
    horizontal_alignment h_align;
    vertical_alignment v_align;
};

};  // namespace graphics