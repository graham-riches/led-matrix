/**
 * \file font_parser.hpp
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
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "expected.hpp"

namespace fonts
{
/********************************** Types *******************************************/
/**
 * \brief structure that contains the bounding box information for a font or glyph     
 */
struct bounding_box {
    uint8_t width;     //!< width of the box
    uint8_t height;    //!< height of the box
    uint8_t x_origin;  //!< x-coordinate of the box origin
    uint8_t y_origin;  //!< y-coordinate of the box origin
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
    uint8_t encoding;              //!< ASCII decimal code value of the character
    uint16_t scalable_width;       //!< scalable width for DPI scaling
    uint16_t device_width;         //!< offset to the start of the next character in X
    bounding_box b_box;            //!< the bounding box for the character
    std::vector<uint32_t> bitmap;  //!< bitmap of character pixel encodings stored as right aligned hex numbers
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
    std::map<char, character> _characters;
};


/********************************** Function Definitions *******************************************/
/**
 * \brief parse a stream of data that is stored as a stream
 * 
 * \param stream the stream containing the data
 * \retval excpected<font, std::string> 
 */
expected<font, std::string> parse(const std::stringstream& stream);




};  // namespace fonts
