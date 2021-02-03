/**
 * \file font_parser.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief 
 * \version 0.1
 * \date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "font_parser.hpp"


/********************************** Function Definitions *******************************************/
namespace fonts
{

expected<font, std::string> parse(const std::stringstream& stream) {
    return expected<fonts::font, std::string>::error("blargh");
}

};

