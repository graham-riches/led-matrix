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