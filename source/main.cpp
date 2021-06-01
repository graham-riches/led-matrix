    /*! \file main.cpp
*
*  \brief main application entry point
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "canvas.h"
#include "config_parser.hpp"
#include "expected.hpp"
#include "io_service.hpp"
#include "led-matrix.h"
#include "nlohmann/json.hpp"
#include "reactive.hpp"
#include "font.hpp"
#include "graphics.hpp"
#include "interval_timer.hpp"
#include "tasks.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <thread>



/********************************** Types and Aliases *******************************************/
//!< alias the json namespace for convenience
using json = nlohmann::json;

//!< alias for an expected of set pixel
using expected_timer_config = expected<graphics::clocks::interval_timer::configuration, std::exception_ptr>;


/********************************** Local Variables *******************************************/
static std::unique_ptr<IntervalTimerTask> task;


/****************************** Function Definitions ***********************************/
/**
 * \brief convert json into an instruction struct to start an interval timer
 * 
 * \param data json data
 * \retval expected type containing an interval timer
 */
expected_timer_config timer_from_json(const json& data) {
    return reactive::mtry([&] { 
        graphics::clocks::interval_timer::configuration config;
        config.total_intervals = data.at("total_intervals");
        config.high_interval_ms = data.at("high_interval_ms");
        config.low_interval_ms = data.at("low_interval_ms");
        config.warmup_interval_ms = data.at("warmup_interval_ms");
        config.cooldown_interval_ms = data.at("cooldown_interval_ms");
        config.repeat_times = data.at("repeat_times");
        return config; });
}

/**
 * \brief sink function to spawn a new timer from a TCP instruction
 * 
 * \param config configuration for the timer 
 * \param time_font font to draw the time counter with
 * \param stats_font font to draw the stats with
 * \param frame graphics frame to draw on
 */
void spawn_timer(const graphics::clocks::interval_timer::configuration& config, fonts::font& time_font, fonts::font& stats_font, graphics::frame& frame) {          
    if (task) {        
        task->cancel();
    }    
    task = std::make_unique<IntervalTimerTask>(config, time_font, stats_font, frame);
    task->start();
}

/**
 * \brief main application entry point.
 * 
 * \param argc number of command line arguments
 * \param argv space delimited command line arguments
 * \retval int process return value
 */
int main(int argc, char* argv[]) {
    //!< open the json configuration file and parse it into an expected ADT which contains either valid JSON or an error message
    json config = json::parse(std::ifstream{"/home/pi/led-matrix/config.json"});

    //!< read the matrix configuration from json
    auto maybe_options = create_options_from_json(config);
    auto options = maybe_options.get_value();
    
    //!< load the fonts
    auto maybe_time_font = fonts::font::from_stream(std::ifstream{std::string{"/home/pi/led-matrix/graphics/fonts/7x13B.bdf"}});
    auto time_font = maybe_time_font.get_value();
    auto maybe_stats_font = fonts::font::from_stream(std::ifstream{std::string{"/home/pi/led-matrix/graphics/fonts/5x8.bdf"}});
    auto stats_font = maybe_stats_font.get_value();

    //!< create the RGB Matrix object from the validated options.    
    auto matrix = std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options));
    matrix->StartRefresh();  //!< unfortunately this manages it's own pthread :(

    //!< create a graphics frame object to render text messages recevied over TCP    
    auto frame = graphics::frame(matrix.get());

    //!< quick hack to render a boot screen
    for (int i = 0; i < options.options.rows; i++) {
        for (int j = 0; j < options.options.cols; j++) {
            matrix->SetPixel(j, i, 255, 255, 255);
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    matrix->Clear();

    //!< create a vector of threads to hold the threads that will do the other application work
    std::vector<std::thread> threads;

    //!< start up a TCP server to receive control messages
    using namespace reactive::operators;
    boost::asio::io_service service;
    
    auto io_pipeline = io_service(service) 
                     | transform( [](const std::string& message){ std::cout << message << std::endl; return message; } )
                     | transform( [](const std::string& message) { return reactive::mtry([&] { return json::parse(message); }); })
                     | transform( [](const auto& exp) { return mbind(exp, timer_from_json); } )
                     | filter( [](const auto& exp){ return (exp); } )
                     | transform( [](const auto& exp){ return exp.get_value(); } )
                     | sink([&](const auto& timer) { spawn_timer(timer, time_font, stats_font, frame); });

    //!< add the TCP io service to the threads list and start the thread
    threads.push_back(std::thread{[&service](){service.run();}});

    //!< wait for threads to be done their work
    for ( auto& thread : threads ) {
        thread.join();
    }

    return 0;
}
