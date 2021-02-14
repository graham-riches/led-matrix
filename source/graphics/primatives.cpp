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
 * \brief Construct a new frame object from a led matrix canvas pointer
 * 
 * \param canvas pointer to the canvas
 */
frame::frame(rgb_matrix::Canvas* canvas)
: _canvas(canvas) {
    _height = canvas->height();
    _width = canvas->width();
}

/**
 * \brief set the value of a pixel in the frame from a reference pixel
 * 
 * \param x coordinate to set at
 * \param y coordinate to set at
 * \param pixel the pixel to set
 */
void frame::set_pixel(int x, int y, const pixel& pixel) {
    if ((x < _width) && (y < _height)) {
        _canvas->SetPixel(x, y, pixel.red, pixel.green, pixel.blue);
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