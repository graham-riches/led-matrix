/*! \file config_tests.cpp
*
*  \brief unit tests for configuration parsing.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "config_parser.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/****************************** Unit Tests ***********************************/

TEST(matrix_options_tests, see_if_it_runs) {
    json j = "hello";
    auto result = parse_matrix_options(j);
    ASSERT_TRUE(true);
}