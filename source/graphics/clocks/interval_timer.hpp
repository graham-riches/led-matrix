/**
 * \file interval_timer.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief interval timer object for doing stopwatch based timers
 * \version 0.1
 * \date 2021-05-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <cstdint>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <memory>
#include "primatives.hpp"
#include "font.hpp"
#include "fmt/core.h"


/********************************** Types *******************************************/
namespace graphics::clocks {

/**
 * \brief helper object that manages a down-counting timer
 */
class downcounting_timer {
public:
    using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

    /**
     * \brief Construct a new downcounting timer object
     * 
     * \param time_ms time in ms to count down for
     */
    downcounting_timer( uint32_t time_ms )
    : start_time(std::chrono::system_clock::now())
    , total_time_ms(time_ms) {}

    /**
     * \brief update method to check the timer
     * 
     * \retval uint32_t time remaining
     */
    uint32_t update() {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);              
        time_remaining = ( elapsed.count() > total_time_ms ) ? 0 : total_time_ms - elapsed.count();
        return time_remaining;
    }

    /**
     * \brief check if the timer is expired
     * 
     * \retval true if the targetted time has passed     
     */
    bool is_elapsed() {
        return (time_remaining == 0);
    }

private:
    time_point_t start_time;
    const uint32_t total_time_ms;
    uint32_t time_remaining;   
};


/**
 * \brief interval timer object that manages a high-low interval and displays graphically
 */
class interval_timer : public shape {
public:    
    /**
     * \brief enumeration of the interval timer state     
     */
    enum class timer_state : unsigned {
        warmup = 0,
        high,
        low,
        cooldown,
        complete
    };

    /**
     * \brief configuration options for an interval timer
     */
    struct configuration {
        uint32_t total_intervals;
        uint32_t high_interval_ms;
        uint32_t low_interval_ms;
        uint32_t warmup_interval_ms;
        uint32_t cooldown_interval_ms;
        uint8_t repeat_times;
    };

    /**
     * \brief Construct a new interval timer object
     * 
     * \param origin the XY top left corner origin of the interval timer
     * \param time_font the font to draw the timer text with
     * \param stats_font the font to draw the stats with
     * \param config timer configuration
     */
    interval_timer(graphics::origin origin,                   
                   fonts::font& time_font,
                   fonts::font& stats_font,
                   configuration config)
    : shape(origin)
    , time_font(time_font)
    , stats_font(stats_font)
    , config(config)
    , elapsed_low_intervals(0)
    , elapsed_high_intervals(0)
    , total_repeats_complete(0)
    , current_time_remaining(0)
    , m_state(timer_state::warmup) 
    , m_timer(std::make_unique<downcounting_timer>(config.warmup_interval_ms)) {
        auto maybe_time_font_bbox = time_font.get_bbox();
        auto maybe_stats_font_bbox = stats_font.get_bbox();

        //!< Might throw accessing optional here!
        auto time_font_bbox = maybe_time_font_bbox.value();
        auto stats_font_bbox = maybe_stats_font_bbox.value();

        time_font_vertical_offset = static_cast<uint8_t>(time_font_bbox.height);
        stats_font_vertical_offset = static_cast<uint8_t>(stats_font_bbox.height);
    }

    /**
     * \brief update the timer and draw to the screen
     * 
     * \retval timer_status 
     */
    frame& draw(frame& canvas) {
        canvas.clear();

        //!< update the timer
        update();

        //!< print out the time remaining
        std::string state_string = (m_state == timer_state::warmup) ? "W:"
                                 : (m_state == timer_state::high)   ? "H:"
                                 : (m_state == timer_state::low)    ? "L:"
                                 : "C:";
        std::string time_remaining = fmt::format("{:s} {:.2f}", state_string, current_time_remaining / 1000.0f );
        graphics::pixel color = (m_state == timer_state::warmup) ? graphics::pixel{255, 128, 128}
                              : (m_state == timer_state::high)   ? graphics::pixel{255, 255, 0}
                              : (m_state == timer_state::low)    ? graphics::pixel{0, 255, 0}
                              : graphics::pixel{ 0, 180, 255 };
        auto time_characters = time_font.encode_with_default(time_remaining, ' ');
        auto time_renderer = graphics::font_renderer(time_characters, graphics::origin{_origin.x, _origin.y}, 
                                                          color.red, color.green, color.blue,
                                                          graphics::text_wrap_mode::wrap);

        //!< add the total set stats below
        std::string sets_complete = fmt::format("Reps: {:d}/{:d}", elapsed_low_intervals, config.total_intervals);
        auto sets_characters = stats_font.encode_with_default(sets_complete, ' ');
        auto sets_renderer = graphics::font_renderer(sets_characters, 
                                                     graphics::origin{_origin.x, static_cast<uint16_t>(_origin.y + time_font_vertical_offset)},
                                                     255, 255, 255, graphics::text_wrap_mode::wrap);

        std::string repeats_complete = fmt::format("Sets: {:d}/{:d}", total_repeats_complete, config.repeat_times);
        auto repeats_characters = stats_font.encode_with_default(repeats_complete, ' ');
        auto repeats_renderer = graphics::font_renderer(repeats_characters, 
                                                        graphics::origin{_origin.x, static_cast<uint16_t>(_origin.y + time_font_vertical_offset + stats_font_vertical_offset)},
                                                        255, 255, 255, graphics::text_wrap_mode::wrap);

        time_renderer.draw(canvas);
        sets_renderer.draw(canvas);
        repeats_renderer.draw(canvas);
        return canvas;
    }

    /**
     * \brief helper to check if the timer is done running
     * 
     * \retval true if complete
     */
    bool is_complete() {
        return (m_state == timer_state::complete);
    }

private:
    /**
     * \brief update the timer
     */
    void update() {
        current_time_remaining = m_timer->update();        

        switch ( m_state ) {
            case timer_state::warmup:
                if ( m_timer->is_elapsed() )
                {
                    m_state = timer_state::high;
                    m_timer = std::make_unique<downcounting_timer>(config.high_interval_ms);
                }
                break;

            case timer_state::high:
                if ( m_timer->is_elapsed() )
                {                    
                    elapsed_high_intervals++;
                    m_timer = std::make_unique<downcounting_timer>(config.low_interval_ms);
                    m_state = timer_state::low;
                }
                break;

            case timer_state::low:
                if ( m_timer->is_elapsed() )
                {
                    elapsed_low_intervals++;
                    auto next_duration = (elapsed_low_intervals == config.total_intervals) ? config.cooldown_interval_ms : config.high_interval_ms;
                    m_state = (elapsed_low_intervals == config.total_intervals) ? timer_state::cooldown : timer_state::high;
                    m_timer = std::make_unique<downcounting_timer>(next_duration);
                }
                break;

            case timer_state::cooldown:
                if ( m_timer->is_elapsed() )
                {
                    total_repeats_complete++;
                    if (total_repeats_complete >= config.repeat_times) {
                        m_state = timer_state::complete;
                    } else {
                        elapsed_low_intervals = 0;
                        elapsed_high_intervals = 0;
                        m_state = timer_state::high;
                        m_timer = std::make_unique<downcounting_timer>(config.high_interval_ms);
                    }                    
                }
                break;

            case timer_state::complete:
                break;

            default:
                break;
        }
    }


    //!< Private member variables
    fonts::font time_font;
    fonts::font stats_font;
    configuration config;
    uint32_t elapsed_high_intervals;
    uint32_t elapsed_low_intervals; 
    uint8_t total_repeats_complete;  
    uint32_t current_time_remaining;    
    std::unique_ptr<downcounting_timer> m_timer;
    timer_state m_state;
    uint8_t time_font_vertical_offset;
    uint8_t stats_font_vertical_offset;
};

};

