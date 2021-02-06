/**
 * \file character.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief definitions of a character object that is part of a font
 * \version 0.1
 * \date 2021-02-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "expected.hpp"
#include <cstdint>
#include <istream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace fonts
{
/********************************** Types *******************************************/
/**
 * \brief structure to store a key value pair type
 * 
 * \tparam T type of the stored values
 */
template <typename T>
struct key_value_pair {
    std::string_view key;
    std::vector<T> values;
};


/**
 * \brief structure that contains the bounding box information for a font or glyph     
 */
struct bounding_box {
    int8_t width;     //!< width of the box
    int8_t height;    //!< height of the box
    int8_t x_origin;  //!< x-coordinate of the box origin
    int8_t y_origin;  //!< y-coordinate of the box origin

    /**
     * \brief bounding box constructor from raw integer values
     */
    bounding_box(int8_t width, int8_t height, int8_t x_origin, int8_t y_origin)
        : width(width)
        , height(height)
        , x_origin(x_origin)
        , y_origin(y_origin) { }

    /**
     * \brief factor method to create a bounding box object from a string view
     * 
     * \param view the string view to create the box from
     * \retval expected<bounding_box, std::string> bounding box object if no errors, otherwise error string
     */
    static expected<bounding_box, std::string> from_stringview(const std::string_view& view);

    /**
     * \brief factory method to create a bounding box from a map containing property keys and associated objects
     * 
     * \param map the 
     * \retval expected<bounding_box, std::string> bounding box object if no errors, otherwise error string
     */
    static expected<bounding_box, std::string> from_key_value_pair(const key_value_pair<int>& kv_pair);
};


/**
 * \brief structure of character properties
 */
struct character_properties {
    uint16_t encoding;                             //!< ASCII decimal code value of the character
    std::pair<uint16_t, uint16_t> scalable_width;  //!< scalable width for DPI scaling
    std::pair<uint8_t, uint8_t> device_width;      //!< offset to the start of the next character in X
    bounding_box b_box;                            //!< the bounding box for the character

    /**
     * \brief Construct a new character properties object
     * 
     * \param encoding the ASCII encoding of the character
     * \param s_width scaling width pair
     * \param d_width device width pair (x offset, and y offset)
     * \param b_box bounding box
     */
    character_properties(const uint16_t& encoding, const std::pair<uint16_t, uint16_t>& s_width, const std::pair<uint8_t, uint8_t>& d_width, const bounding_box& b_box )
    : encoding(encoding)
    , scalable_width(s_width)
    , device_width(d_width)
    , b_box(b_box) {}

    /**
     * \brief factory method to create a character properties structure from a map
     * 
     * \param map the map containing the property tags as keys and the values as fields
     * \retval expected<character_properties, std::string> 
     */
    static expected<character_properties, std::string> from_map(const std::map<std::string_view, std::vector<int>>& map);
};


/**
 * \brief structure representing a bdf encoded font character
 */
struct character {
    character_properties properties; //!< character properties structure 
    std::vector<uint32_t> bitmap;    //!< bitmap of character pixel encodings stored as right aligned hex numbers

    /**
     * \brief Construct a new character object from a properties struct and a bitmap
     * 
     * \param properties the properties structure
     * \param bitmap bitmap of character encodings
     */
    character(character_properties&& properties, std::vector<uint32_t>&& bitmap)
    : properties(std::move(properties))
    , bitmap(std::move(bitmap)) {};

    /**
     * \brief convert a vector of lines into a character structure
     * 
     * \param encoding character encoded as a string
     * \retval expected<character, std::string> expected of character or an error
     */
    static expected<character, std::string> from_string(const std::string& encoding);    
};
};  // namespace fonts
