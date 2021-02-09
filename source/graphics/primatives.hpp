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

#include <cstdint>
#include <utility>
#include <vector>

namespace graphics
{

struct origin {
    uint16_t x;
    uint16_t y;
};

struct envelope {
    uint16_t width;
    uint16_t height;
    origin origin;
};

struct shape {
    envelope envelope;

    shape(const envelope& envelope)
        : envelope(envelope) { }

    shape(envelope&& envelope)
        : envelope(std::move(envelope)) { }

    shape(const shape& other)
        : envelope(other.envelope) { }

    shape(shape&& other)
        : envelope(std::move(other.envelope)) { }

    virtual void draw(origin origin) = 0;
};

};  // namespace graphics
