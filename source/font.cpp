/**
 * \file font.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief 
 * \version 0.1
 * \date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "font.hpp"
#include "range/v3/all.hpp"
#include "string_utilities.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
    //!< splits the string by endlines, converts to string views, and splits into two ranges containing: properties, character bitmapping 
    auto s = encoding | ranges::views::split('\n')                      
                      | ranges::views::transform([](auto&& range) { return std::string_view(&*range.begin(), ranges::distance(range)); })
                      | ranges::views::take_while( [](auto &&line){ return line != "ENDCHAR"; } )
                      | ranges::views::split("BITMAP");
             

    /*
    Here are the things we really care about in this encoding:
        - the ascii encoding integer value: this will be the map lookup of the character
        - the encoding bitfield
        - the bounding box
        - the device width so that we properly move the draw iterator over the next start character
        - scalable width is not required, but we might as well parse it anyways for the sake of completeness
    */

    for ( const auto& str : s ) {
        std::cout << str << std::endl;
    }

    return expected<character, std::string>::error("error");
}

};  // namespace fonts
