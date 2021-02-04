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
#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>
#include <iostream>
#include <string>

/********************************** Function Definitions *******************************************/
namespace fonts
{

expected<font, std::string> parse(const std::istream& stream) {
    std::string data(std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{});
    std::cout << data;
    return expected<fonts::font, std::string>::error("blargh");
}

};  // namespace fonts
