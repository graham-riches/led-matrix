// RGB LED Matrix Graphics Library

#pragma once

#include "nlohmann/json.hpp"
#include "config_parser.hpp"
#include "led-matrix.h"
#include "canvas.hpp"
#include <memory>
#include <string>
#include <fstream>

// Alias the json namespace for convenience
using json = nlohmann::json;

namespace graphics
{

// Wrapper class for RGBMatrix
class matrix
{
    public:
    // Create the matrix from config data
    matrix(configuration_options& options)
        : m_matrix(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options)) {}

    // Create with path to config data
    static matrix from_config(const std::string& config_path) {
        // Open the json configuration file and parse it
        json config = json::parse(std::ifstream{config_path});    
        auto options = graphics::create_options_from_json(config).get_value();
        return matrix(options);
    }

    // Start drawing
    void start() {
        // Starts a pthread in the background, but we don't get the handle for it :(
        m_matrix->StartRefresh();
    }

    canvas create_canvas() {
        return canvas(m_matrix.get());
    }

private:
    std::unique_ptr<rgb_matrix::RGBMatrix> m_matrix;

};

}  // namespace graphics