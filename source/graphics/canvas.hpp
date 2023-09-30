// RGB LED Matrix Graphics Library

#pragma once

#include "canvas.h"
#include "pixel.hpp"

namespace graphics {

// Wrapper type to abstract the RBG-LED matrix canvas type underlying
// the graphics library
class canvas {
  public:
    // Create a new canvas from an RGB led matrix library canvas
    canvas(rgb_matrix::Canvas* canvas)
        : m_canvas(canvas) {}

    // Set a pixel to a color value
    void set_pixel(int x, int y, const pixel& color) {
        if ( (x < m_canvas->width()) && (y < m_canvas->height()) ) {
            m_canvas->SetPixel(x, y, color.red, color.green, color.blue);
        }
    }

    // Set a pixel to a color
    void set_pixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    if ( (x < m_canvas->width()) && (y < m_canvas->height()) ) {
        m_canvas->SetPixel(x, y, red, green, blue);
    }
}

    // Get the canvas width
    int width(void) const {
        return m_canvas->width();
    }

    // Get the canvas height
    int height(void) const {
        return m_canvas->height();
    }

    // Clear the display
    void clear(void) {
        m_canvas->Clear();
    }

    // Uniformly fill to a specific color
    void fill(uint8_t red, uint8_t green, uint8_t blue) {
        m_canvas->Fill(red, green, blue);
    }

  private:
    rgb_matrix::Canvas* m_canvas;
};


};

