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
     * \retval expected<font, std::string> 
     */
    static expected<font, std::string> from_stream(std::istream& stream);

    /**
     * \brief factory method to parse a stream of data that is stored as a stream. Overload for rvalue references
     * 
     * \param stream the stream containing the data
     * \retval expected<font, std::string> 
     */
    static expected<font, std::string> from_stream(std::istream&& stream);

    /**
     * \brief Get a character by its encoding value
     * 
     * \param encoding the ASCII encoding of the character
     * \retval expected<character, std::string> maybe character
     */
    expected<character, std::string> get_character(const uint16_t encoding);

    /**
     * \brief Get a character from its char equivalent encoding
     * 
     * \param encoding the char equivalent encoding
     * \retval expected<character, std::string> maybe character
     */
    expected<character, std::string> get_character(const char encoding);

    /**
     * \brief encode a string as a vector of bitmapped characters
     * 
     * \param message the message string
     * \retval maybe of vector of characters or error
     */
    expected<std::vector<character>, std::string> encode(const std::string& message);

    /**
     * \brief lookup a string and encode it as character objects. Replace any failed lookups with a default character
     * 
     * \param message the message to encode
     * \param default_character the default character to replace any failed lookups with
     * \retval std::vector<font::character> 
     */
    std::vector<character> encode_with_default(const std::string& message, const character default_character);

  private:
    std::map<int, character> _characters;  //!< maps ascii character values to the character object
};
};  // namespace fonts
