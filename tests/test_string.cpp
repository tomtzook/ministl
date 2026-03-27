
#include <gtest/gtest.h>
#include <str.h>

#include "expect.h"


TEST(TestString, test_empty_string) {
    framework::string str;
    EXPECT_FALSE(str);
    EXPECT_EQ(str.length(), 0);
}

TEST(TestString, test_empty_string_set_raw) {
    framework::string str;

    const auto expr = "hello world";
    EXPECT_TRUE(str.set(expr));
    EXPECT_STREQ(str.c_str(), expr);
    EXPECT_TRUE(str);
    EXPECT_EQ(str.length(), strlen(expr));
}

TEST(TestString, test_empty_string_set_other) {
    framework::string str;
    framework::string str2;

    const auto expr = "hello world";
    EXPECT_TRUE(str2.set(expr));
    EXPECT_TRUE(str.set(str2));
    EXPECT_TRUE(str == str2);
    EXPECT_STREQ(str.c_str(), expr);
    EXPECT_TRUE(str);
    EXPECT_EQ(str.length(), strlen(expr));
}

TEST(TestString, test_find_exiting) {
    framework::string str;

    const auto expr = "hello world";
    EXPECT_TRUE(str.set(expr));
    EXPECT_EQ(str.find(' '), 5);
}

TEST(TestString, test_find_missing) {
    framework::string str;

    const auto expr = "hello world";
    EXPECT_TRUE(str.set(expr));
    EXPECT_EQ(str.find('m'), framework::string::npos);
}

TEST(TestString, test_find_last_exiting) {
    framework::string str;

    const auto expr = "hello world";
    EXPECT_TRUE(str.set(expr));
    EXPECT_EQ(str.find_last(' '), 5);
}

TEST(TestString, test_find_last_missing) {
    framework::string str;

    const auto expr = "hello world";
    EXPECT_TRUE(str.set(expr));
    EXPECT_EQ(str.find_last('m'), framework::string::npos);
}

TEST(TestString, test_substr) {
    framework::string str;

    const auto expr = "hello world";
    EXPECT_TRUE(str.set(expr));

    const auto result = str.substr(2, 5);
    EXPECT_TRUE(result);
    const auto& str2 = result.value();
    EXPECT_EQ(str2.length(), 3);
    EXPECT_STREQ(str2.c_str(), "llo");
}

