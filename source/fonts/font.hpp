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
#include "character.hpp"
#include <cstdint>
#include <istream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace fonts
{
/**
 * \brief font object that contains the character encoding for each ascii character in it's binary
 *        encoded equivalent. This provides functions to transform char and string inputs into pixel
 *        mappings.
 */
class font {
  public:
    /**
     * \brief Construct a new font object from a vector of characters
     * 
     * \param characters vector of character objects
     */
    font(const std::vector<character>& characters);

    /**
     * \brief factory method to parse a stream of data that is stored as a stream
     * 
     * \param stream the stream containing the data
     * \retval excpected<font, std::string> 
     */
    static expected<font, std::string> from_stream(std::istream& stream);

  private:
    std::map<int, character> _characters;  //!< maps ascii character values to the character object
};
};  // namespace fonts
