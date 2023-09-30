/**
 * \file simple_clock_task.hpp 
 * \brief Task for displaying the simple clock
 * \version 0.1
 * \date 2023-09-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "cancellable_task.hpp"
#include "font.hpp"
#include "graphics.hpp"
#include "simple_clock.hpp"

/********************************** Types *******************************************/

class simple_clock_task : public tasks::cancellable_task {
  public:
    simple_clock_task(fonts::font& font, graphics::frame& frame)
        : tasks::cancellable_task([&]() {
            clock->draw(frame);
            return true;
        })
        , clock(std::make_unique<graphics::clocks::simple_clock>(graphics::origin{0, 0}, font))
        , frame(frame) { }

  private:
    // Private members
    std::unique_ptr<graphics::clocks::simple_clock> clock;
    graphics::frame& frame;
};
