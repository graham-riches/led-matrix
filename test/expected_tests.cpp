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
#include <exception>

/****************************** Unit Tests ***********************************/
TEST(expected_tests, test_construct_success) {    
    auto result = expected<int, std::string>::success(5);
    ASSERT_TRUE(result);
}

TEST(expected_tests, test_construct_error) {
    auto result = expected<int, std::string>::error("this is an error");
    ASSERT_FALSE(result);
}

TEST(expected_tests, test_get_value) {
    auto result = expected<int, std::string>::success(5);
    ASSERT_EQ(5, result.get_value());
}

TEST(expected_tests, test_get_error) {
    auto result = expected<int, std::string>::error("this is an error");
    ASSERT_EQ("this is an error", result.get_error());
}

TEST(expected_tests, test_get_value_from_error_throws) {
    auto result = expected<int, std::string>::error("this is an error");
    ASSERT_THROW(result.get_value(), std::logic_error);
}

TEST(expected_tests, test_copy_construct) {
    auto initial_value = expected<int, std::string>::success(5);
    auto final_value{initial_value};
    ASSERT_EQ(5, final_value.get_value());
    ASSERT_TRUE(final_value);
}

TEST(expected_tests, test_move_construct) {
    auto result{expected<int,std::string>::success(5)};
    ASSERT_TRUE(result);
    ASSERT_EQ(5, result.get_value());
}

TEST(expected_tests, test_assign_expected_both_valid) {
    auto copy = expected<int, std::string>::success(1);
    auto target = expected<int, std::string>::success(2);
    target = copy;
    ASSERT_TRUE(target);
    ASSERT_EQ(1, target.get_value());

}

TEST(expected_tests, test_assign_expected_target_valid) {
    auto copy = expected<int, std::string>::error("error");
    auto target = expected<int, std::string>::success(2);
    target = copy;
    ASSERT_FALSE(target);
    ASSERT_EQ("error", target.get_error());
}

TEST(expected_tests, test_assign_expected_copy_valid) {
    auto copy = expected<int, std::string>::success(1);
    auto target = expected<int, std::string>::error("error");
    target = copy;
    ASSERT_TRUE(target);
    ASSERT_EQ(1, target.get_value());
}

TEST(expected_tests, test_assign_expected_neither_valid) {
    auto copy = expected<int, std::string>::error("one");
    auto target = expected<int, std::string>::error("two");
    target = copy;    
    ASSERT_EQ("one", target.get_error());
}
