#include "graphics.hpp"
#include "simple_clock_task.hpp"
#include <fstream>
#include <memory>
#include <string>


// Main program entry point
int main(int argc, char* argv[]) {
    auto matrix = graphics::matrix::from_config("/home/pi/led-matrix/config.json");
    auto time_font = graphics::fonts::font::load_from_path("/home/pi/led-matrix/graphics/fonts/9x18B.bdf").get_value();
    
    matrix.start();    
    auto canvas = matrix.create_canvas();
    auto task = std::make_unique<simple_clock_task>(time_font, canvas);
    task->start();
    task->await_complete();
    return 0;
}
