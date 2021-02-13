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
struct set_pixel_t {
    uint8_t x;
    uint8_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

//!< alias for an expected of set pixel
using expected_set_pixel = expected<set_pixel_t, std::exception_ptr>;

/****************************** Function Definitions ***********************************/
/**
 * \brief convert json into an instruction struct to set a pixel value
 * 
 * \param data json data
 * \retval expected_set_pixel expected 
 */
expected_set_pixel pixel_instruction_from_json(const json& data) {
    return reactive::mtry([&] { return set_pixel_t{data.at("x"), data.at("y"), data.at("r"), data.at("g"), data.at("b")}; });
}

/**
 * \brief main application entry point.
 * 
 * \param argc number of command line arguments
 * \param argv space delimited command line arguments
 * \retval int process return value
 */
int main(int argc, char* argv[]) {
    /* open the json configuration file and parse it into an expected ADT which contains either valid JSON or an error message */
    json config = json::parse(std::ifstream{"config.json"});

    auto maybe_options = create_options_from_json(config);
    if ( !maybe_options ) {
        std::cout << "Error: matrix configuration is invalid\n";
        std::cout << maybe_options.get_error() << std::endl;
        return -1;
    }

    /* create the RGB Matrix object from the validated options. */
    auto options = maybe_options.get_value();
    auto matrix = std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options));
    matrix->StartRefresh();  //!< unfortunately this manages it's own pthread :(

    /* create a vector of threads to hold the threads that will do the other application work */
    std::vector<std::thread> threads;

    /* start up a TCP server to receive control messages */
    using namespace reactive::operators;
    boost::asio::io_service service;
    
    auto io_pipeline = io_service(service) 
                     | transform( [](const std::string& message) { return reactive::mtry([&] { return json::parse(message); }); })
                     | transform( [](const auto& exp) { return mbind(exp, pixel_instruction_from_json); } )
                     | filter( [](const auto& exp){ return (exp); } )
                     | transform( [](const auto& exp){ return exp.get_value(); } )
                     | sink([&matrix](const auto& instruction) { matrix->SetPixel(instruction.x, instruction.y, instruction.r, instruction.g, instruction.b); });

    /* start the IO service */
    threads.push_back(std::thread{[&service](){service.run();}});

    /* hack a character onto the screen using a font */
    auto input_stream = std::ifstream{"graphics/fonts/5x8.bdf"};    
    auto maybe_font = fonts::font::from_stream(input_stream);    
    auto font = maybe_font.get_value();
    auto maybe_characters = font.encode("HELLO WORLD");
    auto characters = maybe_characters.get_value();
    uint8_t draw_iterator{0};
    for (const auto character : characters) {
        for (int i = 0; i < character.properties.b_box.height; i++) {
            for (int j = 0; j < character.properties.b_box.width; j++){            
                if (character.bitmap[i] & (0x80 >> j)){
                    auto offset = character.properties.b_box.width * draw_iterator;
                    matrix->SetPixel(offset + j, i, 255, 0, 0);    
                }                        
            }
        }
        draw_iterator++;
    }        
    



    /* wait for threads to be done their work */
    for ( auto& thread : threads ) {
        thread.join();
    }

    return 0;
}
