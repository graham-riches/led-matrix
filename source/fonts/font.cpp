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
#include <charconv>

/********************************** Constants *******************************************/
#define INTEGER_HEX_BASE (16ul)

namespace fonts
{

/**
 * \brief Construct a new font object from a vector of characters
 * 
 * \param characters vector of character objects
 */
font::font(const std::vector<character>& characters) {
    for (const auto& character : characters) {
        _characters.insert(std::make_pair(character.properties.encoding, character));
    }
}


/**
 * \brief parse a stream of data that is stored as a stream
 * 
 * \param stream the stream containing the data
 * \retval excpected<font, std::string> 
 */
expected<font, std::string> font::from_stream(std::istream& stream) {
    //!< read the input stream into a string buffer and get read the underlying string
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string font_data = buffer.str();    

    //!< split the string by properties and characters
    auto temp = string_helpers::split(font_data, "ENDPROPERTIES\n");    
    auto character_strings = string_helpers::split(temp[1], "ENDCHAR\n");    
    
    //!< apply some range based transformations and parsing
    auto characters = character_strings | ranges::views::transform([](auto&& c){ return character::from_string(c); })
                                        | ranges::views::filter([](auto&& exp){ return (exp) ? true : false; })
                                        | ranges::views::transform([](auto&& exp){return exp.get_value();})
                                        | ranges::to_vector;

    //!< return either the parsed font or an error
    if (characters.size() > 0) {
        return expected<fonts::font, std::string>::success(font(characters));
    } else {
        return expected<fonts::font, std::string>::error("No characters found for font");
    }                                  
}

};  // namespace fonts
