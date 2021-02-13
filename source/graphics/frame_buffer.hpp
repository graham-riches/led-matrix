/**
 * \file frame_buffer.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief 
 * \version 0.1
 * \date 2021-02-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cstdint>
#include <vector>
#include "primatives.hpp"

namespace graphics
{


class frame_buffer {
  public:
    frame_buffer(int width, int height, int depth)
        : _frames(std::vector<frame>(depth, frame(width, height))) { }

  private:
    std::vector<frame> _frames;
};

};  // namespace graphics