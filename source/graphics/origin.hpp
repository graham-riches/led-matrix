// RGB LED Matrix Graphics Library

#pragma once

#include <cstdint>

namespace graphics
{

// The origin of graphics object
// NOTE: At this point in time, the origin (0,0) is located in the top-left
// corner of the display with the following coordinate system
//  (0, 0) ----> +X
//  |
//  |
//  V +Y
struct origin {
    uint16_t x;
    uint16_t y;
};

}  // namespace graphics
