/**
 * \file tasks.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief Some helper tasks classes for running different led matrix tasks based on server input
 * \version 0.1
 * \date 2021-05-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <atomic>
#include "font.hpp"
#include "graphics.hpp"
#include "interval_timer.hpp"

/********************************** Types *******************************************/
/**
 * \brief Interface for a running task that can be cancelled
 * 
 */
class ICancellableTask {
    public:
    virtual void cancel() = 0;
    virtual void start() = 0;
    virtual ~ICancellableTask(){} 
};


/**
 * \brief interval timer drawing task. Draws timers to the screen
 * 
 */
class IntervalTimerTask : public ICancellableTask {
    public:
    /**
     * \brief Construct a new Interval Timer Task object
     * 
     * \param config interval timer configuraion
     * \param time_font font to draw the time in
     * \param stats_font fonr to draw the stats in
     * \param frame frame to draw on
     */
    IntervalTimerTask(const graphics::clocks::interval_timer::configuration& config, 
                      fonts::font& time_font, 
                      fonts::font& stats_font, 
                      graphics::frame& frame)     
    : timer(std::make_unique<graphics::clocks::interval_timer>(graphics::origin{0, 0}, time_font, stats_font, config ))
    , cancellation_token(false)
    , frame(frame)    
    { }

    /**
     * \brief interface method to cancel the task if it's running     
     */
    void cancel(void) {
        try{
            cancellation_token = true;
            task_thread.join();
        } catch (...) 
        {
            //!< TODO: improve this error logging?
            std::cout << "Cancelling thread failed" << std::endl;
        }
    }

    /**
     * \brief interface method to start the task
     */
    void start(void) {
        auto task = [&]() {
            while ( !timer->is_complete() && !cancellation_token ) {
                timer->draw(frame);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        };
        task_thread = std::thread(task);
    }

    private:
    //!< Private members
    std::unique_ptr<graphics::clocks::interval_timer> timer;
    std::atomic<bool> cancellation_token;            
    graphics::frame& frame;
    std::thread task_thread;    
};
