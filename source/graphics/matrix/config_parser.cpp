// RGB LED Matrix Graphics Library

#include "config_parser.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>

namespace graphics {

// All option types
enum class options : unsigned {
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
    daemonize,
    font
};


// Helper to convert std::map get into optional
template <typename ReturnType, typename Key>
std::optional<ReturnType> get_value(const std::map<Key, ReturnType>& map, Key key) {
    for ( const auto& [k, v] : map ) {
        if ( k == key ) {
            return v;
        }
    }
    return {};
}


// Maps options struct to RGB led matrix library CLI arguments
const std::map<std::string, options> flag_options = {{"hardware_mapping", options::hardware_mapping},
                                                     {"panel_type", options::panel_type},
                                                     {"scan_mode", options::scan_mode},
                                                     {"row_address_type", options::row_address_type},
                                                     {"multiplexing", options::multiplexing},
                                                     {"rows", options::rows},
                                                     {"columns", options::columns},
                                                     {"chain_length", options::chain_length},
                                                     {"parallel_chains", options::parallel_chains},
                                                     {"pwm_bits", options::pwm_bits},
                                                     {"pwm_lsb_nanoseconds", options::pwm_lsb_nanoseconds},
                                                     {"pwm_dither_bits", options::pwm_dither_bits},
                                                     {"disable_hardware_pulsing", options::disable_hardware_pulsing},
                                                     {"brightness", options::brightness},
                                                     {"invert_colors", options::invert_colors},
                                                     {"rgb_sequence", options::rgb_sequence},
                                                     {"pixel_mapper", options::pixel_mapper},
                                                     {"show_refresh_rate", options::show_refresh_rate},
                                                     {"limit_refresh_rate", options::limit_refresh_rate},
                                                     {"slowdown", options::slowdown},
                                                     {"daemonize", options::daemonize},
                                                     {"font", options::font}};

static const std::map<std::string, int> daemon_settings = {{"manual", -1}, {"on", 1}, {"off", 0}};


// Copy construct configuration options - requires deep copying some items
configuration_options::configuration_options(const configuration_options& other) {
    string_options = other.string_options;
    app_options = other.app_options;
    runtime_options = other.runtime_options;
    options = other.options;

    // Point the led matrix string settings to the correct pointers since it uses char pointers instead of strings..
    options.hardware_mapping = string_options.hardware_mapping.c_str();
    options.panel_type = string_options.panel_type.c_str();
    options.led_rgb_sequence = string_options.led_rgb_sequence.c_str();
    options.pixel_mapper_config = string_options.pixel_mapper_config.c_str();
}


// Parse config options from JSON
expected<configuration_options, std::string> create_options_from_json(json& config) {
    configuration_options options;

    for ( const auto& [key, value] : config.items() ) {
        auto option = get_value(flag_options, key);
        if ( option ) {
            switch ( option.value() ) {
                case options::hardware_mapping:
                    options.string_options.hardware_mapping = std::string{value};
                    options.options.hardware_mapping = options.string_options.hardware_mapping.c_str();
                    break;

                case options::panel_type:
                    options.string_options.panel_type = std::string{value};
                    options.options.panel_type = options.string_options.panel_type.c_str();
                    break;

                case options::scan_mode:
                    options.options.scan_mode = int{value};
                    break;

                case options::row_address_type:
                    options.options.row_address_type = int{value};
                    break;

                case options::multiplexing:
                    options.options.multiplexing = int{value};
                    break;

                case options::rows:
                    options.options.rows = int{value};
                    break;

                case options::columns:
                    options.options.cols = int{value};
                    break;

                case options::chain_length:
                    options.options.chain_length = int{value};
                    break;

                case options::parallel_chains:
                    options.options.parallel = int{value};
                    break;

                case options::pwm_bits:
                    options.options.pwm_bits = int{value};
                    break;

                case options::pwm_lsb_nanoseconds:
                    options.options.pwm_lsb_nanoseconds = int{value};
                    break;

                case options::pwm_dither_bits:
                    options.options.pwm_dither_bits = int{value};
                    break;

                case options::disable_hardware_pulsing:
                    options.options.disable_hardware_pulsing = bool{value};
                    break;

                case options::brightness:
                    options.options.brightness = int{value};
                    break;

                case options::invert_colors:
                    options.options.inverse_colors = bool{value};
                    break;

                case options::rgb_sequence:
                    options.string_options.led_rgb_sequence = std::string{value};
                    options.options.led_rgb_sequence = options.string_options.led_rgb_sequence.c_str();
                    break;

                case options::pixel_mapper:
                    options.string_options.pixel_mapper_config = std::string{value};
                    options.options.pixel_mapper_config = options.string_options.pixel_mapper_config.c_str();
                    break;

                case options::show_refresh_rate:
                    options.options.show_refresh_rate = bool{value};
                    break;

                case options::limit_refresh_rate:
                    options.options.limit_refresh_rate_hz = int{value};
                    break;

                case options::slowdown:
                    options.runtime_options.gpio_slowdown = int{value};
                    break;

                case options::daemonize: {
                    auto daemon_setting = get_value(daemon_settings, std::string{value});
                    options.runtime_options.daemon = (daemon_setting) ? daemon_setting.value() : 0;
                    break;
                }

                case options::font:
                    options.app_options.font = std::string{value};
                    break;

                default:
                    break;
            }
        }
    }

    std::string validation_results;
    if ( options.options.Validate(&validation_results) ) {
        return expected<configuration_options, std::string>::success(options);
    } else {
        return expected<configuration_options, std::string>::error(validation_results);
    }
}

}