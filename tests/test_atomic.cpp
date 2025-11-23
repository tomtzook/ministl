
#include <gtest/gtest.h>
#include <atomic.h>


TEST(TestAtomicBool, test_set) {
    framework::atomic<bool> val;

    val.store(true);
    EXPECT_TRUE(val.load());

    val.store(false);
    EXPECT_FALSE(val.load());

    EXPECT_FALSE(val.exchange(true));
    EXPECT_TRUE(val.load());

    bool set_expect = true;
    EXPECT_TRUE(val.compare_exchange(set_expect, false));
    EXPECT_FALSE(val.load());

    set_expect = true;
    EXPECT_FALSE(val.compare_exchange(set_expect, false));
    EXPECT_FALSE(val.load());
    EXPECT_FALSE(set_expect);
}

TEST(TestAtomicInteger, test_set) {
    using type = int;

    framework::atomic<type> val;

    constexpr type value1 = 5;
    val.store(value1);
    EXPECT_EQ(val.load(), value1);

    constexpr type value2 = 10;
    val.store(value2);
    EXPECT_EQ(val.load(), value2);

    EXPECT_EQ(val.exchange(value1), value2);
    EXPECT_EQ(val.load(), value1);

    type set_expect = value1;
    EXPECT_TRUE(val.compare_exchange(set_expect, value2));
    EXPECT_EQ(val.load(), value2);

    set_expect = value1;
    EXPECT_FALSE(val.compare_exchange(set_expect, value2));
    EXPECT_EQ(val.load(), value2);
    EXPECT_EQ(set_expect, value2);

    val += 1;
    EXPECT_EQ(val.load(), value2 + 1);
    val -= 1;
    EXPECT_EQ(val.load(), value2);
    EXPECT_EQ(val++, value2);
    EXPECT_EQ(val.load(), value2 + 1);
    EXPECT_EQ(val--, value2 + 1);
    EXPECT_EQ(val.load(), value2);
}
