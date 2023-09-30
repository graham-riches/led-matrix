

#pragma once

/********************************** Includes *******************************************/
#include "cancellable_task.hpp"
#include "font.hpp"
#include "graphics.hpp"
#include "simple_clock.hpp"

/********************************** Types *******************************************/

class simple_clock_task : public tasks::cancellable_task {
  public:
    simple_clock_task(graphics::fonts::font& font, graphics::canvas& canvas)
        : tasks::cancellable_task([&]() {
            clock->draw(canvas);
            return true;
        })
        , clock(std::make_unique<graphics::clocks::simple_clock>(graphics::origin{0, 0}, font))
        , canvas(canvas) { }

  private:
    // Private members
    std::unique_ptr<graphics::clocks::simple_clock> clock;
    graphics::canvas& canvas;
};
