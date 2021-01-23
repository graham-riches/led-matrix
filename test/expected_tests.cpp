/*! \file expected_tests.cpp
*
*  \brief unit tests for the expected ADT
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "expected.h"
#include <string>

/****************************** Unit Tests ***********************************/
TEST(expected_tests, test_construct_success) {    
    auto result = expected<int, std::string>::success(5);
    ASSERT_TRUE(result);
}

