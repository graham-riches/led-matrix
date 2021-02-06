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


/****************************** Test Fixtures ***********************************/
class font_tests : public ::testing::Test {
    public:
    void SetUp() override {
        input_stream = std::ifstream{"../font_parser/4x6.bdf"};
    };

    void TearDown() override {

    };

    std::ifstream input_stream;
    
};

/****************************** Unit Tests ***********************************/
/* test that parsing an invalid font file stream returns an error */
TEST_F(font_tests, test_empty_stream_returns_error) {    
    auto font = fonts::parse(input_stream);
}

/* test creating a font bounding box from a stringview object works */
TEST_F(font_tests, test_bounding_box_from_stringview_works) {
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
TEST_F(font_tests, test_bounding_box_not_enough_arguments_fails) {
    std::string_view view{"BBX 4 6 0"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_FALSE(maybe_box);
}

/* test creating a bounding box without the BBX tag fails */
TEST_F(font_tests, test_bounding_box_missing_key_fails) {
    std::string_view view{"4 6 0 -1"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_FALSE(maybe_box);
}

/* test creating a bounding box with an invalid tag fails */
TEST_F(font_tests, test_create_bounding_box_invalid_key_fails) {
    std::string_view view{"WRONG_TAG 4 6 0 -1"};
    auto maybe_box = fonts::bounding_box::from_stringview(view);
    ASSERT_FALSE(maybe_box);
}

/* test parsing a string character encoding into a character object works if the string is valid */
TEST_F(font_tests, test_character_parser_returns_character) {
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

    auto maybe_character = fonts::to_character(encoding);
    if (!maybe_character) {
        std::cout << maybe_character.get_error() << std::endl;
    }
}