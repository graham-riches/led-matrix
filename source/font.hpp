/**
 * \file font.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief bdf font file parser
 * \version 0.1
 * \date 2021-02-02
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
     * \retval bounding_box bounding box object
     */
    static expected<bounding_box, std::string> from_stringview(const std::string_view& view);
};

/**
 * \brief structure containing high-level overall font properties
 */
struct properties {
    std::string foundry;
    std::string family;
    std::string weight;
    uint8_t ascent;
    uint8_t descent;
    uint8_t pixel_size;
};

/**
 * \brief structure representing a bdf encoded font character
 */
struct character {
    uint8_t encoding;                              //!< ASCII decimal code value of the character
    std::pair<uint16_t, uint16_t> scalable_width;  //!< scalable width for DPI scaling
    std::pair<uint8_t, uint8_t> device_width;      //!< offset to the start of the next character in X
    bounding_box b_box;                            //!< the bounding box for the character
    std::vector<uint32_t> bitmap;                  //!< bitmap of character pixel encodings stored as right aligned hex numbers
};

/**
 * \brief font object that contains the character encoding for each ascii character in it's binary
 *        encoded equivalent. This provides functions to transform char and string inputs into pixel
 *        mappings.
 */
class font {
  public:
  private:
    properties _properties;
    std::map<int, character> _characters;  //!< maps ascii character values to the character object
};

/********************************** Function Definitions *******************************************/
/**
 * \brief parse a stream of data that is stored as a stream
 * 
 * \param stream the stream containing the data
 * \retval excpected<font, std::string> 
 */
expected<font, std::string> parse(std::istream& stream);

/**
 * \brief convert a vector of lines into a character structure
 * 
 * \param encoding character encoded as a string
 * \retval expected<character, std::string> expected of character or an error
 */
expected<character, std::string> to_character(const std::string& encoding);

};  // namespace fonts
