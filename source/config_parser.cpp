/*! \file config_parser.cpp
*
*  \brief parsing and validation for application configuration stored as JSON
*
*
*  \author Graham Riches
*/
/********************************** Includes *******************************************/
#include "config_parser.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

/********************************** Types and Aliases *******************************************/
using rgb_matrix::RGBMatrix::Options = options;  //!< alias to simplify getting at rgb_matrix options struct

/**
 * \brief function pointer for a parser type that creates a new options struct based on an optional JSON tag
 */
typedef options (*json_field_parser)(const json& data, options options);


/********************************** Local Function Declarations *******************************************/
template <typename ReturnType, typename Key>
std::optional<ReturnType> get_value(const std::map<Key, ReturnType>& map, Key key);

//!< special attribute parsers for JSON setup options
static options hardware_mapping(const json& data, options options);
static options panel_type(const json& data, options options);
static options scan_mode(const json& data, options options);
static options row_address_type(const json& data, options options);
static options multiplexing(const json& data, options options);
static options rows(const json& data, options options);
static options columns(const json& data, options options);
static options chain_length(const json& data, options options);
static options parallel_chains(const json& data, options options);
static options pwm_bits(const json& data, options options);
static options pwm_lsb_nanoseconds(const json& data, options options);
static options pwm_dither_bits(const json& data, options options);
static options disable_hardware_pulsing(const json& data, options options);
static options brightness(const json& data, options options);
static options invert_colors(const json& data, options options);
static options rgb_sequence(const json& data, options options);
static options pixel_mapper(const json& data, options options);
static options show_refresh_rate(const json& data, options options);
static options limit_refresh_rate(const json& data, options options);


/********************************** Local Variables *******************************************/
const std::map<std::string, json_field_parser> parsers = {{"hardware_mapping", hardware_mapping},
                                                          {"panel_type", panel_type},
                                                          {"scan_mode", scan_mode},
                                                          {"row_address_type", row_address_type},
                                                          {"multiplexing", multiplexing},
                                                          {"rows", rows},
                                                          {"columns", columns},
                                                          {"chain_length", chain_length},
                                                          {"parallel_chains", parallel_chains},
                                                          {"pwm_bits", pwm_bits},
                                                          {"pwm_lsb_nanoseconds", pwm_lsb_nanoseconds},
                                                          {"pwm_dither_bits", pwm_dither_bits},
                                                          {"disable_hardware_pulsing", disable_hardware_pulsing},
                                                          {"brightness", brightness},
                                                          {"invert_colors", invert_colors},
                                                          {"rgb_sequence", rgb_sequence},
                                                          {"pixel_mapper", pixel_mapper},
                                                          {"show_refresh_rate", show_refresh_rate},
                                                          {"limit_refresh_rate", limit_refresh_rate}};


/********************************** Public Function Definitions *******************************************/
expected<options, std::string> parse_matrix_options(json& json_data) {
    rgb_matrix::RGBMatrix::Options options();

    auto fields = json_data["setup_options"];
    for ( const auto& field : fields ) {
    }

    std::string validation_errors;
    if ( options.validate(&validation_errors) ) {
        return expected<rgb_matrix::RGBMatrix::Options, std::string>::success(options);
    } else {
        return expected<rgb_matrix::RGBMatrix::Options, std::string>::error(validation_errors);
    }
}

expected<rgb_matrix::RuntimeOptions, std::string> parse_runtime_options(json& json_data) {

    return {};
}


/********************************** Local Function Definitions *******************************************/
/**
 * \brief check if a key is contained in a map and return an optional
 * 
 * \tparam ReturnType the type value contained in the map
 * \tparam Key type of the key to the map
 * \param map std::map object that might contain the requested key
 * \param key the requested key
 * \retval std::optional<ReturnType> returns the value in the map at key if it exists
 */
template <typename ReturnType, typename Key>
std::optional<ReturnType> get_value(const std::map<Key, ReturnType>& map, Key key) {
    for ( const auto& [k, v] : map ) {
        if ( k == key ) {
            return v;
        }
    }
    return {};
}



static options hardware_mapping(const json& data, options options);
static options panel_type(const json& data, options options);
static options scan_mode(const json& data, options options);
static options row_address_type(const json& data, options options);
static options multiplexing(const json& data, options options);
static options rows(const json& data, options options);
static options columns(const json& data, options options);
static options chain_length(const json& data, options options);
static options parallel_chains(const json& data, options options);
static options pwm_bits(const json& data, options options);
static options pwm_lsb_nanoseconds(const json& data, options options);
static options pwm_dither_bits(const json& data, options options);
static options disable_hardware_pulsing(const json& data, options options);
static options brightness(const json& data, options options);
static options invert_colors(const json& data, options options);
static options rgb_sequence(const json& data, options options);
static options pixel_mapper(const json& data, options options);
static options show_refresh_rate(const json& data, options options);
static options limit_refresh_rate(const json& data, options options);
