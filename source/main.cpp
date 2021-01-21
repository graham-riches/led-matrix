/*! \file main.cpp
*
*  \brief main application entry point
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "canvas.h"
#include "graphics.h"
#include "led-matrix.h"
#include "pixel-mapper.h"
#include <iostream>
#include <thread>
#include <chrono>


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
    rgb_matrix::RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;

    // These are the defaults when no command-line flags are given.
    matrix_options.rows = 32;
    matrix_options.chain_length = 1;
    matrix_options.parallel = 1;

    // First things first: extract the command line flags that contain
    // relevant matrix options.
    if ( !rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt) ) {
        std::cout << "ERROR: failed to parse some options\n";
    }

    auto matrix =  std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt));
    
    if (!matrix) {
        std::cout << "ERROR: RGB matrix object allocation failed" << std::endl;
        return -1;
    }


    printf("Size: %dx%d. Hardware gpio mapping: %s\n", matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    rgb_matrix::Canvas* canvas = matrix.get();

    TestFrameGenerator generator{canvas};
    generator.run();

    return 0;
}