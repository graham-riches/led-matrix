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
#include "led-matrix.h"
#include "nlohmann/json.hpp"
#include <optional>
#include <string>

using json = nlohmann::json;

/********************************** Types *******************************************/
enum class SetupOptions : unsigned { hardware, dimensions, chaining, pwm, display, refresh };

/**
 * \brief enumeration of different hardware settings that are wrapped up int the CLI options
 *        of the led matrix
 */
enum class HardwareSettings : unsigned { mapping, panel_type, scan_mode, row_address_type, multiplexing };

/**
 * \brief settings for the sizing of the display window
 */
enum class DimensionSettings : unsigned { rows, columns };

/**
 * \brief settings for connecting multiple displays together
 */
enum class ChainingSettings : unsigned { chain_length, parallel_chains };

/**
 * \brief settings for different PWM controls
 */
enum class PWMSettings : unsigned { brightness, invert_colors, rgb_sequence, pixel_mapper };

/**
 * \brief settings for refresh rate limiting and viewing
 * 
 */
enum class RefreshSettings : unsigned { show_rate, limit_rate };

/**
 * \brief settings for runtime control
 */
enum class RuntimeSettings : unsigned { slowdown, daemonize, initialize_gpio };

/**
 * \brief slowdown the display for smoother animations
 */
enum class SlowdownLevel : unsigned { none, one, two, three, four };

/**
 * \brief handles runtime daemon settings
 */
enum class DaemonSettings : unsigned {
    none,  //!< user must manually start the refresh thread
    off,   //!< refresh thread starts but is not daemonized
    on     //!< refresh thread starts and is automatically daemonized
};

/********************************** Function Declarations *******************************************/
/**
 * \brief parse configuration options from JSON into matrix options struct. Returns an options struct if all
 *        parsing was valid.
 * 
 * \param json_data the json container to parse
 * \retval std::optional<rgb_matrix::RGBMatrix::Options> optional of validated struct
 */
std::optional<rgb_matrix::RGBMatrix::Options> parse_matrix_options(json& json_data);

/**
 * \brief parse runtime options from JSON into a runtime options struct. Returns a RuntimeOptions struct
 *        if all parsing was valid
 * 
 * \param json_data the json container to parse
 * \retval std::optional<rgb_matrix::RuntimeOption> optional of validated struct
 */
std::optional<rgb_matrix::RuntimeOptions> parse_runtime_options(json& json_data);

#endif /* __CONFIG_PARSER_H__ */
