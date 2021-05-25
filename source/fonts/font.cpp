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
#include <algorithm>
#include <exception>


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
 * \brief Get a character by its encoding value
 * 
 * \param encoding the ASCII encoding of the character
 * \retval expected<character, std::string> maybe character
 */
expected<character, std::string> font::get_character(const uint16_t encoding) {
    try {
        return expected<character, std::string>::success(_characters.at(encoding));
    } catch (...) {
        return expected<character, std::string>::error("Could not find character encoding");
    }
}


/**
 * \brief Get a character from its char equivalent encoding
 * 
 * \param encoding the char equivalent encoding
 * \retval expected<character, std::string> maybe character
 */
expected<character, std::string> font::get_character(const char encoding) {
    return this->get_character(static_cast<uint16_t>(encoding));
}


/**
 * \brief parse a stream of data that is stored as a stream
 * 
 * \param stream the stream containing the data
 * \retval excpected<font, std::string> 
 */
expected<font, std::string> font::from_stream(std::istream&& stream) {
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
    return (characters.size() > 0) ? expected<fonts::font, std::string>::success(font(characters))
         : expected<fonts::font, std::string>::error("No characters found for font");    
}

/**
 * \brief parse a stream of data that is stored as a stream. Overload for value references
 * 
 * \param stream the stream containing the data
 * \retval excpected<font, std::string> 
 */
expected<font, std::string> font::from_stream(std::istream& stream) {
    return from_stream(std::move(stream));
}


/**
 * \brief encode a string as a vector of bitmapped characters
 * 
 * \param message the message string
 * \retval maybe of vector of characters or error
 */
expected<std::vector<character>, std::string> font::encode(const std::string& message) {
    auto characters = message | ranges::views::transform([this](auto&& c){return get_character(c);})
                              | ranges::views::filter([](auto&& exp){return (exp) ? true : false;})
                              | ranges::views::transform([](auto&& exp){return exp.get_value();})
                              | ranges::to_vector;
    
    return (characters.size() != message.size()) ? expected<std::vector<character>, std::string>::error("Encoding one or more tokens failed")
         : expected<std::vector<character>, std::string>::success(characters);
}


/**
 * \brief lookup a string and encode it as character objects. Replace any failed lookups with a default character
 * 
 * \param message the message to encode
 * \param default_character the default character to replace any failed lookups with
 * \retval std::vector<font::character> 
 */
std::vector<character> font::encode_with_default(const std::string& message, const character default_character) {
    /* perform the lookup, swap any invalid expected types with the default and return as a vector */
    return message | ranges::views::transform([this](auto&& c){return get_character(c);})
                   | ranges::views::transform([this, default_character](auto&& exp){ return (exp) ? exp.get_value() : default_character; })
                   | ranges::to_vector;
}


/**
 * \brief lookup a string and replace any missing characters with the character passed as default provided it exists
 * \note this function cannot guarantee that the default character passed in exists in the character set, so it must throw
 *       an exception if that character does not exist.
 * 
 * \param message the string to encode
 * \param default_character default character to replace missing characters with
 * \retval std::vector<character> 
 */
std::vector<character> font::encode_with_default(const std::string& message, const char default_character) {
    auto maybe_character = get_character(default_character);
    if (maybe_character) {
        auto character = maybe_character.get_value();
        return encode_with_default(message, character);
    } else {
        throw std::runtime_error("default character does not exist in the selected font");
    }
}


/**
 * \brief Get the bbox object for the font
 * 
 * \retval optional<bounding_box> 
 */
std::optional<bounding_box> font::get_bbox() {
    auto maybe_character_a = get_character('a');
    if (maybe_character_a) {
        auto character_a = maybe_character_a.get_value();
        return character_a.properties.b_box;
    } else {
        return {};
    }
}

};  // namespace fonts
