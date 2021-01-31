/*! \file config_parser.cpp
*
*  \brief parsing and validation for application configuration stored as JSON
*
*
*  \author Graham Riches
*/
/********************************** Includes *******************************************/
#include "config_parser.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>

/********************************** Types and Aliases *******************************************/
/**
 * \brief enumeration of all command line options
 */
enum class Options : unsigned {
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
    limit_refresh_rate,
    slowdown,
    daemonize
};

/********************************** Local Function Declarations *******************************************/
template <typename ReturnType, typename Key>
std::optional<ReturnType> get_value(const std::map<Key, ReturnType>& map, Key key);

/********************************** Local Variables *******************************************/
const std::map<std::string, Options> flag_options = {{"hardware_mapping", Options::hardware_mapping},
                                                     {"panel_type", Options::panel_type},
                                                     {"scan_mode", Options::scan_mode},
                                                     {"row_address_type", Options::row_address_type},
                                                     {"multiplexing", Options::multiplexing},
                                                     {"rows", Options::rows},
                                                     {"columns", Options::columns},
                                                     {"chain_length", Options::chain_length},
                                                     {"parallel_chains", Options::parallel_chains},
                                                     {"pwm_bits", Options::pwm_bits},
                                                     {"pwm_lsb_nanoseconds", Options::pwm_lsb_nanoseconds},
                                                     {"pwm_dither_bits", Options::pwm_dither_bits},
                                                     {"disable_hardware_pulsing", Options::disable_hardware_pulsing},
                                                     {"brightness", Options::brightness},
                                                     {"invert_colors", Options::invert_colors},
                                                     {"rgb_sequence", Options::rgb_sequence},
                                                     {"pixel_mapper", Options::pixel_mapper},
                                                     {"show_refresh_rate", Options::show_refresh_rate},
                                                     {"limit_refresh_rate", Options::limit_refresh_rate},
                                                     {"slowdown", Options::slowdown},
                                                     {"daemonize", Options::daemonize}};

static const std::map<std::string, int> daemon_settings = {{"manual", -1}, {"on", 1}, {"off", 0}};

/********************************** Public Function Definitions *******************************************/
expected<ConfigurationOptions, std::string> create_options_from_json(json& config) {
    ConfigurationOptions options;

    for ( const auto& [key, value] : config.items() ) {
        auto option = get_value(flag_options, key);
        if ( option ) {
            switch ( option.value() ) {
                case Options::hardware_mapping:                    
                    options.string_options.hardware_mapping = std::string{value};
                    options.options.hardware_mapping = options.string_options.hardware_mapping.c_str();
                    break;

                case Options::panel_type:
                    options.string_options.panel_type = std::string{value};
                    options.options.panel_type = options.string_options.panel_type.c_str();
                    break;

                case Options::scan_mode:
                    options.options.scan_mode = int{value};
                    break;

                case Options::row_address_type:
                    options.options.row_address_type = int{value};
                    break;

                case Options::multiplexing:
                    options.options.multiplexing = int{value};
                    break;

                case Options::rows:
                    options.options.rows = int{value};
                    break;

                case Options::columns:
                    options.options.cols = int{value};
                    break;

                case Options::chain_length:
                    options.options.chain_length = int{value};
                    break;

                case Options::parallel_chains:
                    options.options.parallel = int{value};
                    break;

                case Options::pwm_bits:
                    options.options.pwm_bits = int{value};
                    break;

                case Options::pwm_lsb_nanoseconds:
                    options.options.pwm_lsb_nanoseconds = int{value};
                    break;

                case Options::pwm_dither_bits:
                    options.options.pwm_dither_bits = int{value};
                    break;

                case Options::disable_hardware_pulsing:
                    options.options.disable_hardware_pulsing = bool{value};
                    break;

                case Options::brightness:
                    options.options.brightness = int{value};
                    break;

                case Options::invert_colors:
                    options.options.inverse_colors = bool{value};
                    break;

                case Options::rgb_sequence:
                    options.string_options.led_rgb_sequence = std::string{value};
                    options.options.led_rgb_sequence = options.string_options.led_rgb_sequence.c_str();
                    break;

                case Options::pixel_mapper:
                    options.string_options.pixel_mapper_config = std::string{value};
                    options.options.pixel_mapper_config = options.string_options.pixel_mapper_config.c_str();
                    break;

                case Options::show_refresh_rate:
                    options.options.show_refresh_rate = bool{value};
                    break;

                case Options::limit_refresh_rate:
                    options.options.limit_refresh_rate_hz = int{value};
                    break;

                case Options::slowdown:
                    options.runtime_options.gpio_slowdown = int{value};
                    break;

                case Options::daemonize: {
                    auto daemon_setting = get_value(daemon_settings, std::string{value});
                    options.runtime_options.daemon = (daemon_setting) ? daemon_setting.value() : 0;
                    break;
                }

                default:
                    break;
            }
        }
    }

    std::string validation_results;
    if ( options.options.Validate(&validation_results) ) {
        return expected<ConfigurationOptions, std::string>::success(options);
    } else {
        return expected<ConfigurationOptions, std::string>::error(validation_results);
    }
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
