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
#include "font_parser.hpp"
#include <string>
#include <exception>
#include <sstream>
#include <fstream>


/****************************** Test Fixtures ***********************************/
class font_parser_tests : public ::testing::Test {
    public:
    void SetUp() override {
        input_stream = std::fstream{"4x6.bdf"};
    };

    void TearDown() override {

    };

    std::fstream input_stream;
    
};

/****************************** Unit Tests ***********************************/
TEST_F(font_parser_tests, test_empty_stream_returns_error) {    
    auto font = fonts::parse(input_stream);
}
