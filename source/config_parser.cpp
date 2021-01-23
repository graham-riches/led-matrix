/*! \file config_parser.cpp
*
*  \brief parsing and validation for application configuration stored as JSON
*
*
*  \author Graham Riches
*/
/********************************** Includes *******************************************/
#include "config_parser.h"
#include <iostream>
#include <map>


/********************************** Local Function Declarations *******************************************/

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
std::optional<ReturnType> get_value(const std::map<Key, ReturnType> &map, Key key) {    
    for (const auto& [k, v] : map) {
        if (k == key) {
            return v;
        }
    }
    return {};
}


/********************************** Local Variables *******************************************/
static const std::map<SetupOptions, std::string> option_keys = { 
    {SetupOptions::hardware, "hardware"}, {SetupOptions::dimensions, "dimensions"}, {SetupOptions::chaining, "chaining"},
    {SetupOptions::pwm, "pwm"}, {SetupOptions::display, "display"}
};



/********************************** Public Function Definitions *******************************************/
std::optional<rgb_matrix::RGBMatrix::Options> parse_matrix_options(json &json_data) {
    //rgb_matrix::RGBMatrix::Options options;

    return {};
}


std::optional<rgb_matrix::RuntimeOptions> parse_runtime_options(json &json_data) {

    return {};
}


