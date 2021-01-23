/*! \file config_parser.h
*
*  \brief config_parser module for parsing a configuration JSON file into a validated
*         configuration structure for the matrix and other application components.
*
*
*  \author Graham Riches
*/
#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

/********************************** Includes *******************************************/
#include "expected.h"
#include "led-matrix.h"
#include "nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

/********************************** Types *******************************************/
enum class SetupOptions : unsigned {
    hardware_mapping,
    panel_type,
    scan_mode,
    row_address_type,
    multiplexing,
    rows,
    columns,
    chain_length,
    parallel_chains,
    pwm_bits,
    pwm_lsb_nanoseconds,
    pwm_dither_bits,
    disable_hardware_pulsing,
    brightness,
    invert_colors,
    rgb_sequence,
    pixel_mapper,
    show_refresh_rate,
    limit_refresh_rate
};

/**
 * \brief settings for runtime control
 */
enum class RuntimeSettings : unsigned { slowdown, daemonize, initialize_gpio };

/********************************** Function Declarations *******************************************/
/**
 * \brief parse configuration options from JSON into matrix options struct. Returns an options struct if all
 *        parsing was valid.
 * 
 * \param json_data the json container to parse
 * \retval expected of options or a string if configuration is invalid
 */
expected<rgb_matrix::RGBMatrix::Options, std::string> parse_matrix_options(json& json_data);

/**
 * \brief parse runtime options from JSON into a runtime options struct. Returns a RuntimeOptions struct
 *        if all parsing was valid
 * 
 * \param json_data the json container to parse
 * \retval expected of RuntimeOptions or an error string if configuration is invalid
 */
expected<rgb_matrix::RuntimeOptions, std::string> parse_runtime_options(json& json_data);

#endif /* __CONFIG_PARSER_H__ */
