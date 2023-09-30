// RGB LED Matrix Graphics Library

#pragma once

#include "canvas.hpp"
#include "origin.hpp"

namespace graphics
{

// Base class for canvass/widgets that provides an easy
// interface for creating collections of shapes that make up
// a display and can be easily drawn onto a canvas
class shape {
  public:
    // Create a new shape
    shape(const origin& origin)
        : m_origin(origin) { }

    // Draw a shape on the canvas
    virtual void draw(canvas& canvas) = 0;

  protected:
    origin m_origin;
};

};  // namespace graphics