/**
 * \file character.cpp
 * \brief method declarations for character objects
 * \version 0.1
 * \date 2021-02-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "font.hpp"
#include "range/v3/all.hpp"
#include "string_utilities.hpp"
#include <charconv>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/********************************** Constants *******************************************/
#define INTEGER_HEX_BASE (16ul)

namespace fonts
{
/********************************** Local Function Definitions *******************************************/
/**
 * \brief convert a stringview into an int representation
 * 
 * \param view the string view
 * \param base the integer base of the number. Defaults to a base 10 int
 * \retval int integer value of the string
 */
template <typename T>
T stringview_to_int(const std::string_view& view, int base = 10) {
    T value;
    std::from_chars(view.data(), view.data() + view.size(), value, base);
    return value;
}

/**
 * \brief convert a stringview into a keyvalue pair containing integers
 * 
 * \param view the string view
 * \retval key_value_pair<int> key-value pair object 
 */
key_value_pair<int> to_property_kv_pair(const std::string_view& view) {
    auto tokens = view | ranges::views::split(' ') |
                  ranges::views::transform([](auto&& range) { return std::string_view(&*range.begin(), ranges::distance(range)); }) | ranges::to_vector;

    auto values = tokens | ranges::views::drop(1) | ranges::views::transform([](auto&& item) { return stringview_to_int<int>(item); }) | ranges::to_vector;
    return key_value_pair<int>{tokens[0], values};
}

/**
 * \brief converts a vector of keyvalue pair objects into a std::map
 * 
 * \param pairs vector of pairs
 * \retval std::map<std::stringview, std::vector<int>> 
 */
std::map<std::string_view, std::vector<int>> kv_pairs_to_map(const std::vector<key_value_pair<int>>& pairs) {
    std::map<std::string_view, std::vector<int>> map;
    for ( const auto& pair : pairs ) {
        map.insert(std::make_pair(pair.key, pair.values));
    }
    return map;
}

/********************************** Public Function Definitions *******************************************/
/**
 * \brief factor method to create a bounding box object from a string view
 * 
 * \param view the string view to create the box from
 * \retval expected<bounding_box, std::string> containing either the box, or an error message
 */
expected<bounding_box, std::string> bounding_box::from_stringview(const std::string_view& view) {
    key_value_pair<int> kv_pair = to_property_kv_pair(view);
    return bounding_box::from_key_value_pair(kv_pair);
}

/**
 * \brief factory method to create a bounding box from a key_value pair object
 * 
 * \param kv_pair the key_value pair object
 * \retval expected<bounding_box, std::string> bounding box object if no errors, otherwise error string
 */
expected<bounding_box, std::string> bounding_box::from_key_value_pair(const key_value_pair<int>& kv_pair) {
    return (kv_pair.key != "BBX") ? expected<bounding_box, std::string>::error("Invalid key for constructor")
           : (kv_pair.values.size() != 4)
               ? expected<bounding_box, std::string>::error("BoundingBox: not enough arguments supplied")
               : expected<bounding_box, std::string>::success(bounding_box(kv_pair.values[0], kv_pair.values[1], kv_pair.values[2], kv_pair.values[3]));
}

/**
 * \brief factory method to create a character properties structure from a map
 * 
 * \param map the map containing the property tags as keys and the values as fields
 * \retval expected<character_properties, std::string> 
 */
expected<character_properties, std::string> character_properties::from_map(const std::map<std::string_view, std::vector<int>>& map) {
    try {
        auto scalable_width = std::make_pair<uint16_t, uint16_t>(map.at("SWIDTH")[0], map.at("SWIDTH")[1]);
        auto device_width = std::make_pair<uint8_t, uint8_t>(map.at("DWIDTH")[0], map.at("DWIDTH")[1]);
        auto encoding = map.at("ENCODING")[0];
        auto maybe_b_box = bounding_box::from_key_value_pair(key_value_pair<int>{"BBX", map.at("BBX")});

        // check the bounding box parsed successfully
        if ( !maybe_b_box ) {
            return expected<character_properties, std::string>::error(maybe_b_box.get_error());
        }

        character_properties properties(encoding, scalable_width, device_width, maybe_b_box.get_value());
        return expected<character_properties, std::string>::success(std::move(properties));
    } catch ( ... ) {
        return expected<character_properties, std::string>::error("failed to create character properties");
    }
}

/**
 * \brief convert a vector of lines into a character structure
 * 
 * \param encoding character encoded as a string
 * \retval expected<character, std::string> expected of character or an error
 */
expected<character, std::string> character::from_string(const std::string& encoding) {
    // splits the string by endlines, converts to string views, and splits into two ranges containing: properties, character bitmapping
    auto properties = encoding | ranges::views::split('\n') |
                      ranges::views::transform([](auto&& range) { return std::string_view(&*range.begin(), ranges::distance(range)); }) |
                      ranges::views::filter([](auto&& line) { return line != "ENDCHAR"; }) |
                      ranges::views::filter([](auto&& line) { return line.find(std::string_view{"STARTCHAR"}) == line.npos; }) |
                      ranges::views::filter([](auto&& line) { return line != ""; }) | ranges::views::split("BITMAP") |
                      ranges::views::transform([](auto&& range) { return range | ranges::to_vector; }) | ranges::to_vector;

    // check to make sure we got the correct set of values: TODO: not 100% sold on the hard-coded 2 but maybe it makes sense here?
    if ( properties.size() != 2 ) {
        return expected<character, std::string>::error("Invalid character encoding: missing either bit encoding or character properties");
    }

    // parse the properties and return an error if the property parsing failed
    auto property_fields = properties[0] | ranges::views::transform(to_property_kv_pair) | ranges::to_vector;
    auto maybe_properties = character_properties::from_map(kv_pairs_to_map(property_fields));

    if ( !maybe_properties ) {
        return expected<character, std::string>::error(maybe_properties.get_error());
    }

    // extract the properties struct from the expected
    auto c_properties = maybe_properties.get_value();

    // parse the bitmap character encoding
    auto bit_encoding =
        properties[1] | ranges::views::transform([](auto&& view) { return stringview_to_int<uint32_t>(view, INTEGER_HEX_BASE); }) | ranges::to_vector;

    // check to make sure there are enough rows in the bitmap to match the bounding box height
    if ( bit_encoding.size() != c_properties.b_box.height ) {
        return expected<character, std::string>::error("Not enough bitmapped rows for the character type");
    }

    // return the character object
    return expected<character, std::string>::success(character{std::move(c_properties), std::move(bit_encoding)});
}
};  // namespace fonts
