/**
 * \file font_parser_tests.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief unit tests for the font parsing module
 * \version 0.1
 * \date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "expected.hpp"
#include "font.hpp"
#include <string>
#include <exception>
#include <sstream>
#include <fstream>
#include <vector>


/****************************** Unit Tests ***********************************/
/* test that parsing an invalid font file stream returns an error */
TEST(font_tests, test_parsing_font_from_file) {    
    std::ifstream input_stream{"../font_parser/4x6.bdf"};
    auto font = fonts::font::from_stream(input_stream);
    ASSERT_TRUE(font);
}

/* test that the rvalue reference overload also functions */
TEST(font_tests, test_font_from_file_rvalue_ref) {
    auto font = fonts::font::from_stream(std::ifstream{"../font_parser/4x6.bdf"});
    ASSERT_TRUE(font);
}

/* test creating a font bounding box from a stringview object works */
TEST(font_tests, test_bounding_box_from_stringview_works) {
    std::string_view view{"BBX 4 6 0 -1"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_TRUE(maybe_box);
    auto box = maybe_box.get_value();
    ASSERT_EQ(4, box.width);
    ASSERT_EQ(6, box.height);
    ASSERT_EQ(0, box.x_origin);
    ASSERT_EQ(-1, box.y_origin);
}

/* test bounding box missing an argument fails */
TEST(font_tests, test_bounding_box_not_enough_arguments_fails) {
    std::string_view view{"BBX 4 6 0"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_FALSE(maybe_box);
}

/* test creating a bounding box without the BBX tag fails */
TEST(font_tests, test_bounding_box_missing_key_fails) {
    std::string_view view{"4 6 0 -1"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_FALSE(maybe_box);
}

/* test creating a bounding box with an invalid tag fails */
TEST(font_tests, test_create_bounding_box_invalid_key_fails) {
    std::string_view view{"WRONG_TAG 4 6 0 -1"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_FALSE(maybe_box);
}

/* test parsing a string character encoding into a character object works if the string is valid */
TEST(font_tests, test_character_parser_returns_character) {
    const char* encoding = 
        "STARTCHAR space\n"
        "ENCODING 32\n"
        "SWIDTH 640 0\n"
        "DWIDTH 4 0\n"
        "BBX 4 6 0 -1\n"
        "BITMAP\n"
        "00\n"
        "00\n"
        "00\n"
        "00\n"
        "00\n"
        "00\n"
        "ENDCHAR\n";

    auto maybe_character = fonts::character::from_string(encoding);
    ASSERT_TRUE(maybe_character);
}

/* test parsing a character that is missing a bitmap line fails */
TEST(font_tests, test_character_parser_bitmap_too_short_fails) {
    const char* encoding = 
        "STARTCHAR space\n"
        "ENCODING 32\n"
        "SWIDTH 640 0\n"
        "DWIDTH 4 0\n"
        "BBX 4 6 0 -1\n"
        "BITMAP\n"
        "00\n"
        "00\n"
        "00\n"
        "00\n"        
        "00\n"
        "ENDCHAR\n";

    auto maybe_character = fonts::character::from_string(encoding);
    ASSERT_FALSE(maybe_character);
}

/* a character encoding with no bounding box should fail */
TEST(font_tests, test_character_parser_missing_bbox_fails) {
    const char* encoding = 
        "STARTCHAR space\n"
        "ENCODING 32\n"
        "SWIDTH 640 0\n"
        "DWIDTH 4 0\n"        
        "BITMAP\n"
        "00\n"
        "00\n"
        "00\n"
        "00\n"        
        "00\n"
        "00\n"
        "ENDCHAR\n";

    auto maybe_character = fonts::character::from_string(encoding);
    ASSERT_FALSE(maybe_character);
}

/* a character with no ascii encoding should fail */
TEST(font_tests, test_character_parser_missing_encoding_fails) {
    const char* encoding = 
        "STARTCHAR space\n"        
        "SWIDTH 640 0\n"
        "DWIDTH 4 0\n" 
        "BBX 4 6 0 -1\n"       
        "BITMAP\n"
        "00\n"
        "00\n"
        "00\n"
        "00\n"        
        "00\n"
        "00\n"
        "ENDCHAR\n";

    auto maybe_character = fonts::character::from_string(encoding);
    ASSERT_FALSE(maybe_character);
}

/* test converting a string to a vector of characters */
TEST(font_tests, test_string_to_vector_of_characters_works) {
    std::string message{"Hello World"};
    std::ifstream input_stream{"../font_parser/4x6.bdf"};
    auto maybe_font = fonts::font::from_stream(input_stream);    
    auto font = maybe_font.get_value();
    auto maybe_characters = font.encode(message);
    auto characters = maybe_characters.get_value();
    char *message_ptr = message.data();
    for (const auto& character : characters) {
        ASSERT_EQ(character.properties.encoding, static_cast<uint16_t>(*message_ptr++));
    }
}

/* string to vector fails if there are missing characters in the font */
TEST(font_tests, test_string_to_vector_of_characters_missing_character_fails) {
    std::vector<fonts::character> characters;
    fonts::character_properties properties{32, {0, 0}, {0, 0}, fonts::bounding_box{0, 0, 0, 0}};    
    characters.push_back(fonts::character{std::move(properties), std::vector<uint32_t>{}});
    fonts::font font{characters};
    auto maybe_characters = font.encode("Hello World"); //!< should fail since we only have one character in the font
    ASSERT_FALSE(maybe_characters);
}

/* string to vector works with missing characters provided a suitable default is given */
TEST(font_tests, test_string_to_vector_of_characters_with_default) {
    std::vector<fonts::character> characters;
    fonts::character_properties properties{32, {0, 0}, {0, 0}, fonts::bounding_box{0, 0, 0, 0}};
    fonts::character default_character{std::move(properties), std::vector<uint32_t>{}};
    characters.push_back(default_character);
    fonts::font font{characters};
    auto encoded = font.encode_with_default("Hello World", default_character); //!< all characters should return as default
    for (const auto& character : encoded) {
        ASSERT_EQ(32, character.properties.encoding);
    }
}
