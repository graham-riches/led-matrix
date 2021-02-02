/*! \file config_parser.hpp
*
*  \brief config_parser module for parsing a configuration JSON file into a validated
*         configuration structure for the matrix and other application components.
*
*
*  \author Graham Riches
*/
#pragma once

/********************************** Includes *******************************************/
#include "expected.hpp"
#include "led-matrix.h"
#include "nlohmann/json.hpp"
#include <string>
#include <vector>
#include <memory>

using json = nlohmann::json;

/********************************** Types *******************************************/

/**
 * \brief set of string options to manage the memory requirements for the matrix configuration structure
 */
struct StringOptions {
    std::string hardware_mapping;
    std::string panel_type;
    std::string led_rgb_sequence;
    std::string pixel_mapper_config;
};

/**
 * \brief structure to hold the complete set of matrix options
 */
struct ConfigurationOptions {
    rgb_matrix::RGBMatrix::Options options;
    rgb_matrix::RuntimeOptions runtime_options;
    StringOptions string_options;

    /**
     * \brief Construct a new Configuration Options object with the default constructor
     */
    ConfigurationOptions() = default;

    /**
     * \brief Construct a new Configuration Options object by copy, which requires
     *        a deep copy to reset the string pointers
     * 
     * \param other the other to copy from
     */
    ConfigurationOptions(const ConfigurationOptions& other) {
        string_options = other.string_options;
        runtime_options = other.runtime_options;
        options = other.options;
        /* reset option string pointers */
        options.hardware_mapping = string_options.hardware_mapping.c_str();
        options.panel_type = string_options.panel_type.c_str();
        options.led_rgb_sequence = string_options.led_rgb_sequence.c_str();
        options.pixel_mapper_config = string_options.pixel_mapper_config.c_str();
    }
};

/********************************** Function Declarations *******************************************/
/**
 * \brief parse configuration options from JSON into matrix options struct. Returns an RGB matrix object if the
 *        options are valid.
 * 
 * \param config the json container to parse
 * \retval expected of options or a string if configuration is invalid
 */
expected<ConfigurationOptions, std::string> create_options_from_json(json& config);
