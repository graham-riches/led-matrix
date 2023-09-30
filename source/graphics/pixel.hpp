// RGB LED Matrix Graphics Library

#pragma once

#include <cstdint>

namespace graphics
{

// RGB pixel. Values range from 0 (off) to 255 (max brightness)
struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

}  // namespace graphics