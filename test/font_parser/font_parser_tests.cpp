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


/****************************** Unit Tests ***********************************/
TEST(font_parser_tests, test_empty_stream_returns_error) {
    std::stringstream input;
    auto font = fonts::parse(input);
    ASSERT_FALSE( font );
}
