/**
 * \file primatives.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief contains definitions for some graphics primatives
 * \version 0.1
 * \date 2021-02-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "primatives.hpp"

namespace graphics
{
/********************************** Function Definitions *******************************************/
/**
 * \brief Construct a new frame object
 * 
 * \param height height of the frame in pixels
 * \param width width of the frame in pixels
 */
frame::frame(int height, int width)
: _width(width)
, _height(height)
, _pixels(std::vector<std::vector<pixel>>(height, std::vector<pixel>(width))) { }

/**
 * \brief Construct a new frame::frame object from a copy
 * 
 * \param other to copy from
 */
frame::frame(const frame& other) {
    _pixels = other._pixels;
}

/**
 * \brief set the value of a pixel in the frame from a reference pixel
 * 
 * \param row row to set at
 * \param column column to set at
 * \param pixel the pixel to set
 */
void frame::set_pixel(int row, int column, const pixel& pixel) {
    if ((row < _height) && (column < _width)) {
        _pixels[row][column] = pixel;
    }
}

/**
 * \brief getter for the width of the frame
 * 
 * \retval integer width
 */
int frame::get_width(void) const {
    return _width;
}

/**
 * \brief getter for the height of the frame
 * 
 * \retval integer height
 */
int frame::get_height(void) const {
    return _height;
}


};