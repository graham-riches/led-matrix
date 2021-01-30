/*! \file main.cpp
*
*  \brief main application entry point
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "canvas.h"
#include "led-matrix.h"
#include "config_parser.h"
#include "nlohmann/json.hpp"
#include "io_service.h"
#include "io_sinks.h"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>


/********************************** Types *******************************************/

/**
 * \brief super garbo test class just used to write some colors to the current pixel
 * 
 */
class TestFrameGenerator {
  public:
    TestFrameGenerator(rgb_matrix::Canvas* canvas)
        : _canvas(canvas) { }

    void run() {
        using namespace std::chrono_literals;
        while ( true ) {
            for ( int row = 0; row < 32; row++ ) {
                for ( int column = 0; column < 64; column++ ) {
                    _canvas->SetPixel(column, row, 255, 0, 0);
                    std::this_thread::sleep_for(100ms);
                }
                std::this_thread::sleep_for(50ms);
            }
            for ( int row = 31; row >= 0; row-- ) {
                for ( int column = 63; column >= 0; column-- ) {
                    _canvas->SetPixel(column, row, 0, 255, 0);                    
                }
                std::this_thread::sleep_for(50ms);
            }
        }
    }

  private:
    rgb_matrix::Canvas* const _canvas;
};

/****************************** Function Definitions ***********************************/
/**
 * \brief main application entry point.
 * 
 * \param argc number of command line arguments
 * \param argv space delimited command line arguments
 * \retval int process return value
 */
int main(int argc, char* argv[]) {
    /* open the json configuration file and parse it into an expected ADT which contains either valid JSON or an error message */
    using json = nlohmann::json;    
    json config = json::parse(std::ifstream{"config.json"});    

    auto maybe_options = create_options_from_json(config);
    if (!maybe_options) {
        std::cout << "Error: matrix configuration is invalid\n";
        std::cout << maybe_options.get_error() << std::endl;
        return -1;
    }

    /* create the RGB Matrix object from the validated options. Note: wrap the raw pointer returned from the Matrix factory method in a 
       unique pointer for memory management */
    auto options = maybe_options.get_value();
    auto matrix = std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options));
    
    matrix->StartRefresh();

    boost::asio::io_service service;

    auto io_pipeline = IOService(service) | sink([](const auto& message){ std::cout << message << std::endl; });
    service.run();

    /* get a pointer to the base canvas object and passs that to the crappy test frame generator to draw some stuff */
    rgb_matrix::Canvas* canvas = matrix.get();
    TestFrameGenerator generator{canvas};
    generator.run();
    

    return 0;
}