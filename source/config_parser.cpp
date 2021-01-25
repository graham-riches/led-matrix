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

/********************************** Local Function Declarations *******************************************/
template <typename ReturnType, typename Key>
std::optional<ReturnType> get_value(const std::map<Key, ReturnType>& map, Key key);


/********************************** Local Variables *******************************************/
const std::map<std::string, std::string> flag_options = {{"hardware_mapping", "--led-gpio-mapping"},
                                                        {"panel_type", "--led-panel-type"},
                                                        {"scan_mode", "--led-scan-mode"},
                                                        {"row_address_type", "--led-row-addr-type"},
                                                        {"multiplexing", "--led-multiplexing"},
                                                        {"rows", "--led-rows"},
                                                        {"columns", "--led-cols"},
                                                        {"chain_length", "--led-chain"},
                                                        {"parallel_chains", "--led-parallel"},
                                                        {"pwm_bits", "--led-pwm-bits"},
                                                        {"pwm_lsb_nanoseconds", "--led-pwm-lsb-nanoseconds"},
                                                        {"pwm_dither_bits", "--led-pwm-dither-bits"},
                                                        {"disable_hardware_pulsing", "--led-hardware-pulse"},
                                                        {"brightness", "--led-brightness"},
                                                        {"invert_colors", "--led-inverse"},
                                                        {"rgb_sequence", "--led-rgb-sequence"},
                                                        {"pixel_mapper", "--led-pixel-mapper"},
                                                        {"show_refresh_rate", "--led-show-refresh"},
                                                        {"limit_refresh_rate", "--led-limit-refresh"},
                                                        {"slowdown", "--led-slowdown-gpio"},
                                                        {"daemonize", "--led-daemon"}};

/********************************** Public Function Definitions *******************************************/
expected<ConfigurationOptions, std::string> create_options_from_json(json& config) {
    ConfigurationOptions options;

    std::vector<char*> option_fields;
    for (const auto& element : config) {
        auto cli_flag = get_value(flag_options, std::string{element});
        if (cli_flag) {
            option_fields.push_back(cli_flag.value().data());            
            option_fields.push_back(std::string{element}.data());
        }
    }

    char **fuck_this_library = option_fields.data();

    if (rgb_matrix::ParseOptionsFromFlags(option_fields.size(), &fuck_this_library, &options.options, &options.runtime_options)) {
        
        return expected<ConfigurationOptions, std::string>::error("Invalid configuration options");
    }
    return expected<ConfigurationOptions, std::string>::success(options);
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
