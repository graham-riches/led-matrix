/**
 * \file frame_renderer.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief class for rendering an entire frame which may be comprised of a collection of shape primitives 
 *        that each know how to draw themselves.
 * \version 0.1
 * \date 2021-02-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "frame.hpp"
#include "primatives.hpp"
#include <numeric>

namespace graphics
{
/********************************** Function Definitions *******************************************/
/**
 * \brief function that will render a series of shapes and will return the filled frame.
 * \note  shapes are ordered from bottom to top (last elements are on the "top" of the image)
 *        TODO: revisit the above statement and use a reverse if required 
 * 
 * \param height height of the frame to create
 * \param width of the frame to create
 * \param shapes vector of shapes to draw into the frame
 * \retval frame rendered frame object
 */
inline frame render(int height, int width, std::vector<shape*> shapes) {
    return std::accumulate(shapes.cbegin(), shapes.cend(), frame{height, width}, [](const frame& frame, shape* shape) { return shape->draw(frame); });
}
};  // namespace graphics
