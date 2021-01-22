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
#include <optional>
#include <string>

/************************************ Types ********************************************/

namespace Configuration {

enum class HardwareSettings : unsigned {
    mapping,
    panel_type,
    scan_mode,
    row_address_type,
    multiplexing
};


enum class DimensionSettings : unsigned {
    rows,
    columns
};


enum class ChainingSettings : unsigned {
    chain_length,
    parallel_chains
};


enum class PWMSesttings : unsigned {
    brightness,
    invert_colors,
    rgb_sequence,
    pixel_mapper
};


enum class RefreshSettings : unsigned {
    show_rate,
    limit_rate
};


enum class RuntimeSettings : unsigned {
    slowdown,
    daemonize,
    initialize_gpio
};


enum class SlowdownLevel : unsigned {
    none,
    one,
    two,
    three,
    four
};


std::optional<rgb_matrix::RGBMatrix::Options> parse_matrix_options(const std::string& data);


};

#endif /* __CONFIG_PARSER_H__ */
