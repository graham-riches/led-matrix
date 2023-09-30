/**
 * \file primatives.cpp 
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
    : _canvas(canvas) { }

/**
 * \brief set the value of a pixel in the frame from a reference pixel
 * 
 * \param x coordinate to set at
 * \param y coordinate to set at
 * \param pixel the pixel to set
 */
void frame::set_pixel(int x, int y, const pixel& pixel) {
    if ( (x < _canvas->width()) && (y < _canvas->height()) ) {
        _canvas->SetPixel(x, y, pixel.red, pixel.green, pixel.blue);
    }
}

/**
 * \brief Set a pixel. Overload to meet the canvas interface
 * 
 * \param x location
 * \param y location
 * \param red pixel R value
 * \param green pixel G value
 * \param blue pixel B value
 */
void frame::set_pixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    if ( (x < _canvas->width()) && (y < _canvas->height()) ) {
        _canvas->SetPixel(x, y, red, green, blue);
    }
}

/**
 * \brief clear the canvas
 */
void frame::clear(void) {
    _canvas->Clear();
}

/**
 * \brief fill the canvas all with a specific color
 * 
 * \param red R value
 * \param green G value
 * \param blue B value
 */
void frame::fill(uint8_t red, uint8_t green, uint8_t blue) {
    _canvas->Fill(red, green, blue);
}

/**
 * \brief getter for the width of the frame
 * 
 * \retval integer width
 */
int frame::width(void) const {
    return _canvas->width();
}

/**
 * \brief getter for the height of the frame
 * 
 * \retval integer height
 */
int frame::height(void) const {
    return _canvas->height();
}

};  // namespace graphics