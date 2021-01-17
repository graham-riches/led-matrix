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
#include <algorithm>
#include <assert.h>
#include <getopt.h>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/********************************** Types *******************************************/

class TestFrameGenerator {
  public:
    TestFrameGenerator(rgb_matrix::Canvas* canvas)
        : _canvas(canvas) { }

    void run() {
        while ( true ) {
            for ( int row = 0; row < 64; row++ ) {
                for ( int column = 0; column < 32; column++ ) {
                    _canvas->SetPixel(row, column, 255, 0, 0);                    
                }
                usleep(10000);
            }
            for ( int row = 63; row >= 0; row-- ) {
                for ( int column = 31; column >= 0; column-- ) {
                    _canvas->SetPixel(row, column, 0, 255, 0);                    
                }
                usleep(10000);
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

    rgb_matrix::RGBMatrix* matrix = rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    if ( matrix == NULL )
        return 1;

    printf("Size: %dx%d. Hardware gpio mapping: %s\n", matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    rgb_matrix::Canvas* canvas = matrix;

    TestFrameGenerator generator{canvas};
    generator.run();

    return 0;
}