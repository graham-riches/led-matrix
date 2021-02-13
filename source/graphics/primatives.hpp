/**
 * \file primatives.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief graphics primatives that are used to stitch together more complex scenes from basic objects
 * \version 0.1
 * \date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <cstdint>
#include <utility>
#include <vector>

namespace graphics
{
/********************************** Types *******************************************/
/**
 * \brief structure containing the draw origin for a graphics primative.
 *        This is the top left coordinate of the image as the pixels map as
 *        row major, then column major
 */
struct origin {
    uint16_t x;
    uint16_t y;
};

/**
 * \brief structure containing the color RGBA value of a pixel
 */
struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

/**
 * \brief frame object that contains a 2D array of pixels that are drawn onto
 */
class frame {
  public:
    frame(int height, int width)
        : width(width)
        , height(height)
        , pixels(std::vector<std::vector<pixel>>(height, std::vector<pixel>(width))) { }

    frame(const frame& other);

    void set_pixel(int row, int column, const pixel& pixel);

    int get_width(void) const;

    int get_height(void) const;

  private:
    std::vector<std::vector<pixel>> pixels;
    int width;
    int height;
};

/**
 * \brief shape interface for drawing graphics primatives
 */
struct shape {
    origin _origin;
    
    shape(const origin& origin)
    : origin(origin) {}

    virtual frame draw(const frame& frame, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) = 0;
};

};  // namespace graphics
