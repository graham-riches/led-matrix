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
#include <memory>

using json = nlohmann::json;

/********************************** Types *******************************************/
struct ConfigurationOptions {
    rgb_matrix::RGBMatrix::Options options;
    rgb_matrix::RuntimeOptions runtime_options;
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



#endif /* __CONFIG_PARSER_H__ */
