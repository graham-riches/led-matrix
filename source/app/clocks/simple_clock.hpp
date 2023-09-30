/**
 * \file simple_clock.hpp 
 * \brief A simple clock for displaying the time
 * \version 0.1
 * \date 2023-09-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "fmt/chrono.h"
#include "fmt/core.h"
#include "font.hpp"
#include "graphics.hpp"
#include "primatives.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

/********************************** Types *******************************************/
namespace graphics::clocks
{

class simple_clock : public shape {
  public:
    using timestamp = std::chrono::time_point<std::chrono::system_clock>;

    simple_clock(graphics::origin origin, fonts::font& font)
        : shape(origin)
        , font(font) { }

    frame& draw(frame& canvas) {
        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::seconds>(now - last_draw_time);
        if ( delta.count() == 0 ) {
            return canvas;
        }
        last_draw_time = now;
        canvas.clear();

        auto time_string = fmt::format("{:%H:%M}", now);
        auto time_characters = font.encode_with_default(time_string, ' ');
        auto time_renderer = graphics::text_box(time_characters,
                                                graphics::origin{_origin.x, _origin.y},
                                                color,
                                                canvas.width(),
                                                canvas.height(),
                                                graphics::horizontal_alignment::center,
                                                graphics::vertical_alignment::center);
        time_renderer.draw(canvas);
        return canvas;
    }

  private:
    fonts::font font;
    timestamp last_draw_time;
    graphics::pixel color{255, 128, 128};
};
};  // namespace graphics::clocks