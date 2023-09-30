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
#include "simple_clock_task.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <thread>

/********************************** Types and Aliases *******************************************/
// alias the json namespace for convenience
using json = nlohmann::json;

// Main program entry point
int main(int argc, char* argv[]) {
    // open the json configuration file and parse it into an expected ADT which contains either valid JSON or an error message
    json config = json::parse(std::ifstream{"/home/pi/led-matrix/config.json"});    
    auto options = create_options_from_json(config).get_value();        

    auto time_font = fonts::font::from_stream(std::ifstream{std::string{"/home/pi/led-matrix/graphics/fonts/9x18B.bdf"}}).get_value();

    // create the RGB Matrix object from the validated options.    
    auto matrix = std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options));
    matrix->StartRefresh();  // unfortunately this manages it's own pthread :(

    // create a graphics frame object to render text messages recevied over TCP    
    auto frame = graphics::frame(matrix.get());

    auto task = std::make_unique<simple_clock_task>(time_font, frame);
    task->start();
    task->await_complete();

    return 0;
}
