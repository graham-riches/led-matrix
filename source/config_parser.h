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


/********************************** Function Declarations *******************************************/
/**
 * \brief parse configuration options from JSON into matrix options struct. Returns an RGB matrix object if the
 *        options are valid.
 * 
 * \param config the json container to parse
 * \retval expected of options or a string if configuration is invalid
 */
expected<std::unique_ptr<rgb_matrix::RGBMatrix>, std::string> parse_setup_options(json& config);



#endif /* __CONFIG_PARSER_H__ */
