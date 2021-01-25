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
                }
                std::this_thread::sleep_for(5000ms);
            }
            for ( int row = 31; row >= 0; row-- ) {
                for ( int column = 63; column >= 0; column-- ) {
                    _canvas->SetPixel(column, row, 0, 255, 0);                    
                }
                std::this_thread::sleep_for(5000ms);
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
    using json = nlohmann::json;
    std::ifstream stream{"../config/config.json"};
    json config;
    stream >> config;

    auto options = create_options_from_json(config);
    if (!options) {
        std::cout << "Error: matrix configuration is invalid\n";
        return -1;
    }
    
    /*
    auto matrix = matrix_expected.get_value();
    rgb_matrix::Canvas* canvas = matrix.get();

    TestFrameGenerator generator{canvas};
    generator.run();
    */

    return 0;
}