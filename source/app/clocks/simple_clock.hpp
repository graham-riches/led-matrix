// A simple clock display

#pragma once

#include "fmt/chrono.h"
#include "fmt/core.h"
#include "graphics.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

namespace graphics::clocks
{

class simple_clock : protected shape {
  public:
    using timestamp = std::chrono::time_point<std::chrono::system_clock>;

    simple_clock(graphics::origin origin, fonts::font& font)
        : shape(origin)
        , font(font) { }

    void draw(canvas& canvas) {
        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::seconds>(now - last_draw_time);
        if ( delta.count() == 0 ) {
            return;
        }

        last_draw_time = now;
        canvas.clear();

        auto local = std::chrono::system_clock::to_time_t(now);
        auto time_string = fmt::format("{:%H:%M}", fmt::localtime(local));
        auto time_characters = font.encode_with_default(time_string, ' ');
        auto time_renderer = graphics::text_box(time_characters,
                                                graphics::origin{m_origin.x, m_origin.y},
                                                color,
                                                canvas.width(),
                                                canvas.height(),
                                                graphics::horizontal_alignment::center,
                                                graphics::vertical_alignment::center);
        time_renderer.draw(canvas);
    }

  private:
    fonts::font font;
    timestamp last_draw_time;
    graphics::pixel color{255, 128, 128};
};
};  // namespace graphics::clocks