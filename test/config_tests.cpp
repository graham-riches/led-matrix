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

/****************************** Function Definitions ***********************************/

TEST(matrix_options_tests, see_if_it_runs) {
    auto result = parse_matrix_options("hello");
    ASSERT_TRUE(true);
}