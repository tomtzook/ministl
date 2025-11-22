
#include <gtest/gtest.h>
#include <result.h>


TEST(TestResult, test_value_primitive) {
    constexpr uint32_t value = 5;
    framework::result<uint32_t> res = framework::ok(value);

    EXPECT_TRUE(static_cast<bool>(res));
    EXPECT_TRUE(res.is_success());
    EXPECT_FALSE(res.is_error());
    EXPECT_EQ(res.value(), value);
    EXPECT_EQ(res.release_value(), value);
}

TEST(TestResult, test_value_struct) {
    constexpr uint32_t inner_value = 5;
    struct some_struct {
        uint32_t value;
    };

    some_struct value{inner_value};
    framework::result<some_struct> res = framework::ok(value);

    EXPECT_TRUE(static_cast<bool>(res));
    EXPECT_TRUE(res.is_success());
    EXPECT_FALSE(res.is_error());
    EXPECT_EQ(res.value().value, inner_value);
    EXPECT_EQ(res.release_value().value, inner_value);
}
