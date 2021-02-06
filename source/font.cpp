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
/********************************** Types *******************************************/

/**
 * \brief container to store a stringview key and associated values used when parsing
 *        lines of the form: "KEY val_1 val_2 ... val_N"
 */
template <typename T>
struct key_value_pair {
    std::string_view key;
    std::vector<T> values;
};

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
    auto tokens = view | ranges::views::split(' ')
                       | ranges::views::transform([](auto&& range) { return std::string_view(&*range.begin(), ranges::distance(range)); })
                       | ranges::to_vector;
    
    auto values = tokens | ranges::views::drop(1)
                         | ranges::views::transform([](auto &&item){ return stringview_to_int<int>(item); })
                         | ranges::to_vector;
    return key_value_pair<int>{tokens[0], values};
}


/********************************** Public Function Definitions *******************************************/
/**
 * \brief factor method to create a bounding box object from a string view
 * 
 * \param view the string view to create the box from
 * \retval expected<bounding_box, std::string> containing either the box, or an error message
 */
expected<bounding_box, std::string> bounding_box::from_stringview(const std::string_view& view) {
    auto items = view | ranges::views::split(' ')
                      | ranges::views::transform([](auto&& range) { return std::string_view(&*range.begin(), ranges::distance(range)); })
                      | ranges::views::drop(1)
                      | ranges::views::transform([](auto &&value){return stringview_to_int<int8_t>(value);})
                      | ranges::to_vector;
    if (items.size() != 4) {
        return expected<bounding_box, std::string>::error("BoundingBox: not enough arguments supplied");
    } else{
        return expected<bounding_box, std::string>::success(bounding_box(items[0], items[1], items[2], items[3]));
    }
}

/**
 * \brief parse a stream of data that is stored as a stream
 * 
 * \param stream the stream containing the data
 * \retval excpected<font, std::string> 
 */
expected<font, std::string> parse(std::istream& stream) {
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string font_data = buffer.str();

    return expected<fonts::font, std::string>::error("blargh");
}

/**
 * \brief convert a vector of lines into a character structure
 * 
 * \param encoding character encoded as a string
 * \retval expected<character, std::string> expected of character or an error
 */
expected<character, std::string> to_character(const std::string& encoding) {    
    //!< splits the string by endlines, converts to string views, and splits into two ranges containing: properties, character bitmapping 
    auto properties = encoding | ranges::views::split('\n')                      
                               | ranges::views::transform([](auto&& range) { return std::string_view(&*range.begin(), ranges::distance(range)); })
                               | ranges::views::take_while([](auto &&line){ return line != "ENDCHAR";})
                               | ranges::views::take_while([](auto &&line){ return line.find("STARTCHAR") == line.npos; })
                               | ranges::views::split("BITMAP")
                               | ranges::to_vector;

    //!< check to make sure we got the correct set of values: TODO: not 100% sold on the hard-coded 2 but maybe it makes sense here?
    if (properties.size() != 2) {
        return expected<character, std::string>::error("Invalid character encoding: missing either bit encoding or character properties");
    }

    //!< parse the properties
    auto property_fields = properties[0] | ranges::views::transform(to_property_kv_pair)
                                         | ranges::to_vector;
    
    auto bit_encoding = properties[1] | ranges::views::transform([](auto &&view){return stringview_to_int<uint32_t>(view, INTEGER_HEX_BASE);})                                            
                                      | ranges::to_vector;
    

    return expected<character, std::string>::error("error");
}

};  // namespace fonts
