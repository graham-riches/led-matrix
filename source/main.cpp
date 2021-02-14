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
#include <boost/asio.hpp>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>


/********************************** Types and Aliases *******************************************/
//!< alias the json namespace for convenience
using json = nlohmann::json;

//!< structure to store instructions to set a pixel to a specific color
struct set_pixel {
    uint8_t x;
    uint8_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

//!< alias for an expected of set pixel
using expected_set_pixel = expected<set_pixel, std::exception_ptr>;

/****************************** Function Definitions ***********************************/
/**
 * \brief convert json into an instruction struct to set a pixel value
 * 
 * \param data json data
 * \retval expected_set_pixel expected 
 */
expected_set_pixel pixel_instruction_from_json(const json& data) {
    return reactive::mtry([&] { return set_pixel{data.at("x"), data.at("y"), data.at("r"), data.at("g"), data.at("b")}; });
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
    json config = json::parse(std::ifstream{"config.json"});

    auto maybe_options = create_options_from_json(config);
    if ( !maybe_options ) {        
        std::cout << maybe_options.get_error() << std::endl;
        return -1;
    }    
    auto options = maybe_options.get_value();
    
    //!< load the main application font bdf from the configuration
    auto maybe_font = fonts::font::from_stream(std::ifstream{std::string{"graphics/fonts/"} + options.app_options.font});
    if (!maybe_font) {
        std::cout << maybe_font.get_error() << std::endl;
        return -1;
    }
    auto font = maybe_font.get_value();
    

    //!< create the RGB Matrix object from the validated options.    
    auto matrix = std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options));
    matrix->StartRefresh();  //!< unfortunately this manages it's own pthread :(

    //!< test encode a string to render    
    auto characters = font.encode_with_default("HELLO WORLD", ' ');
    auto frame = graphics::frame(matrix.get());
    auto font_drawer = graphics::font_renderer(characters, graphics::origin{0, 0}, 255, 0, 0);
    font_drawer.draw(frame);


    //!< create a vector of threads to hold the threads that will do the other application work
    std::vector<std::thread> threads;

    //!< start up a TCP server to receive control messages
    using namespace reactive::operators;
    boost::asio::io_service service;
    
    auto io_pipeline = io_service(service) 
                     | transform( [](const std::string& message) { return reactive::mtry([&] { return json::parse(message); }); })
                     | transform( [](const auto& exp) { return mbind(exp, pixel_instruction_from_json); } )
                     | filter( [](const auto& exp){ return (exp); } )
                     | transform( [](const auto& exp){ return exp.get_value(); } )
                     | sink([&matrix](const auto& instruction) { matrix->SetPixel(instruction.x, instruction.y, instruction.r, instruction.g, instruction.b); });

    //!< add the TCP io service to the threads list and start the thread
    threads.push_back(std::thread{[&service](){service.run();}});

    //!< wait for threads to be done their work
    for ( auto& thread : threads ) {
        thread.join();
    }

    return 0;
}
