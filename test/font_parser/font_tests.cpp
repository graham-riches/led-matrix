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
TEST_F(font_tests, test_empty_stream_returns_error) {    
    auto font = fonts::parse(input_stream);
}


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

    auto character = fonts::to_character(encoding);
}