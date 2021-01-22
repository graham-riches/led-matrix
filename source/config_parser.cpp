/*! \file config_parser.cpp
*
*  \brief parsing and validation for application configuration stored as JSON
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "config_parser.h"
#include "nlohmann/json.hpp"
#include <iostream>

/****************************** Function Definitions ***********************************/
using json = nlohmann::json;


std::optional<rgb_matrix::RGBMatrix::Options>> parse_matrix_options(const std::string& data)
{
    std::cout << "blargh";
}