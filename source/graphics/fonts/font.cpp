// RGB LED Matrix Graphics Library

#include "font.hpp"
#include "range/v3/all.hpp"
#include "string_utilities.hpp"
#include <algorithm>
#include <charconv>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define INTEGER_HEX_BASE (16ul)

namespace graphics
{
namespace fonts
{
//-----------------------------------------------------------------------------
font::font(const std::vector<character>& characters) {
    for ( const auto& character : characters ) {
        m_characters.insert(std::make_pair(character.properties.encoding, character));
    }
}

//-----------------------------------------------------------------------------
expected<character, std::string> font::get_character(const uint16_t encoding) {
    try {
        return expected<character, std::string>::success(m_characters.at(encoding));
    } catch ( ... ) {
        return expected<character, std::string>::error("Could not find character encoding");
    }
}

//-----------------------------------------------------------------------------
expected<character, std::string> font::get_character(const char encoding) {
    return this->get_character(static_cast<uint16_t>(encoding));
}

//-----------------------------------------------------------------------------
expected<font, std::string> font::from_stream(std::istream&& stream) {
    // read the input stream into a string buffer and get read the underlying string
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string font_data = buffer.str();

    // split the string by properties and characters
    auto temp = string_helpers::split(font_data, "ENDPROPERTIES\n");
    auto character_strings = string_helpers::split(temp[1], "ENDCHAR\n");

    // apply some range based transformations and parsing
    auto characters = character_strings | ranges::views::transform([](auto&& c) { return character::from_string(c); }) |
                      ranges::views::filter([](auto&& exp) { return (exp) ? true : false; }) |
                      ranges::views::transform([](auto&& exp) { return exp.get_value(); }) | ranges::to_vector;

    // return either the parsed font or an error
    return (characters.size() > 0) ? expected<fonts::font, std::string>::success(font(characters))
                                   : expected<fonts::font, std::string>::error("No characters found for font");
}

//-----------------------------------------------------------------------------
expected<font, std::string> font::from_stream(std::istream& stream) {
    return from_stream(std::move(stream));
}

//-----------------------------------------------------------------------------
expected<font, std::string> font::load_from_path(const std::string& path) {    
    return from_stream(std::ifstream{path});
}

//-----------------------------------------------------------------------------
expected<std::vector<character>, std::string> font::encode(const std::string& message) {
    auto characters = message | ranges::views::transform([this](auto&& c) { return get_character(c); }) |
                      ranges::views::filter([](auto&& exp) { return (exp) ? true : false; }) |
                      ranges::views::transform([](auto&& exp) { return exp.get_value(); }) | ranges::to_vector;

    return (characters.size() != message.size()) ? expected<std::vector<character>, std::string>::error("Encoding one or more tokens failed")
                                                 : expected<std::vector<character>, std::string>::success(characters);
}

//-----------------------------------------------------------------------------
std::vector<character> font::encode_with_default(const std::string& message, const character default_character) {
    /* perform the lookup, swap any invalid expected types with the default and return as a vector */
    return message | ranges::views::transform([this](auto&& c) { return get_character(c); }) |
           ranges::views::transform([this, default_character](auto&& exp) { return (exp) ? exp.get_value() : default_character; }) | ranges::to_vector;
}

//-----------------------------------------------------------------------------
std::vector<character> font::encode_with_default(const std::string& message, const char default_character) {
    auto maybe_character = get_character(default_character);
    if ( maybe_character ) {
        auto character = maybe_character.get_value();
        return encode_with_default(message, character);
    } else {
        throw std::runtime_error("default character does not exist in the selected font");
    }
}

//-----------------------------------------------------------------------------
std::optional<bounding_box> font::get_bbox() {
    auto maybe_character_a = get_character('a');
    if ( maybe_character_a ) {
        auto character_a = maybe_character_a.get_value();
        return character_a.properties.b_box;
    } else {
        return {};
    }
}

};  // namespace fonts
};  // namespace graphics