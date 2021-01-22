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



std::optional<rgb_matrix::RGBMatrix::Options> parse_matrix_options(const std::string& data);








#endif /* __CONFIG_PARSER_H__ */
