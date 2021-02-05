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
#include "range/v3/view/transform.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/split.hpp"
#include "range/v3/range/conversion.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


/********************************** Function Definitions *******************************************/
namespace fonts
{

expected<font, std::string> parse(std::istream& stream) {        
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string font_data = buffer.str();    

    

    return expected<fonts::font, std::string>::error("blargh");
}


expected<character, std::string> to_character(const std::string& encoding) {
    std::string temp{"THIS IS AN ALL CAPS STRING"};
    auto s = temp | ranges::views::transform(tolower) | ranges::to<std::string>;
    std::cout << s;
    return expected<character, std::string>::error("error");
}

};  // namespace fonts
