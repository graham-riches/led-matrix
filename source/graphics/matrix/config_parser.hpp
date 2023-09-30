// RGB LED Matrix Graphics Library

#pragma once

#include "expected.hpp"
#include "led-matrix.h"
#include "nlohmann/json.hpp"
#include <memory>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace graphics {

/**
 * \brief set of string options to manage the memory requirements for the led matrix configuration structure
 */
struct matrix_string_options {
    std::string hardware_mapping;
    std::string panel_type;
    std::string led_rgb_sequence;
    std::string pixel_mapper_config;
};

/**
 * \brief custom application options that don't deal with the LED matrix settings 
 */
struct application_options {
    std::string font;
};

/**
 * \brief structure to hold the complete set of matrix options
 */
struct configuration_options {
    rgb_matrix::RGBMatrix::Options options;
    rgb_matrix::RuntimeOptions runtime_options;
    matrix_string_options string_options;
    application_options app_options;

    /**
     * \brief Construct a new Configuration Options object with the default constructor
     */
    configuration_options() = default;

    /**
     * \brief Construct a new Configuration Options object by copy, which requires
     *        a deep copy to reset the string pointers
     * 
     * \param other the other to copy from
     */
    configuration_options(const configuration_options& other);
};



/**
 * \brief parse configuration options from JSON into matrix options struct. Returns an RGB matrix object if the
 *        options are valid.
 * 
 * \param config the json container to parse
 * \retval expected of options or a string if configuration is invalid
 */
expected<configuration_options, std::string> create_options_from_json(json& config);

};