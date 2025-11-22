
#include <gtest/gtest.h>
#include <optional.h>


// todo: check abort when empty

TEST(TestOptional, test_empty_primitive) {
    const framework::optional<uint32_t> opt;

    EXPECT_FALSE(static_cast<bool>(opt));
    EXPECT_FALSE(opt.has_value());
}

TEST(TestOptional, test_empty_struct) {
    struct some_struct {
        uint32_t value;
    };

    const framework::optional<some_struct> opt;

    EXPECT_FALSE(static_cast<bool>(opt));
    EXPECT_FALSE(opt.has_value());
}

TEST(TestOptional, test_set_value_primitive) {
    framework::optional<uint32_t> opt;

    EXPECT_FALSE(static_cast<bool>(opt));
    EXPECT_FALSE(opt.has_value());

    constexpr uint32_t value = 5;
    opt = value;

    EXPECT_TRUE(static_cast<bool>(opt));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), value);
    EXPECT_EQ(*opt, value);
}

TEST(TestOptional, test_set_value_struct) {
    struct some_struct {
        uint32_t value;
    };

    framework::optional<some_struct> opt;

    EXPECT_FALSE(static_cast<bool>(opt));
    EXPECT_FALSE(opt.has_value());

    const some_struct value{5};
    opt = value;

    EXPECT_TRUE(static_cast<bool>(opt));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value().value, value.value);
    // ReSharper disable once CppRedundantDereferencingAndTakingAddress
    EXPECT_EQ((*opt).value, value.value);
    EXPECT_EQ(opt->value, value.value);
}

TEST(TestOptional, test_set_value_struct_onlymove) {
    struct some_struct {
        uint32_t value = 0;

        some_struct() = default;
        some_struct(some_struct&&) = default;
        some_struct(const some_struct&) = delete;

        some_struct& operator=(some_struct&&) = default;
        some_struct& operator=(const some_struct&) = delete;
    };

    framework::optional<some_struct> opt;

    EXPECT_FALSE(static_cast<bool>(opt));
    EXPECT_FALSE(opt.has_value());

    constexpr uint32_t inner_value = 5;
    some_struct value; value.value = inner_value;
    opt = framework::move(value);

    EXPECT_TRUE(static_cast<bool>(opt));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value().value, inner_value);
    // ReSharper disable once CppRedundantDereferencingAndTakingAddress
    EXPECT_EQ((*opt).value, inner_value);
    EXPECT_EQ(opt->value, inner_value);
}

TEST(TestOptional, test_set_value_struct_onlycopy) {
    struct some_struct {
        uint32_t value = 0;

        some_struct() = default;
        some_struct(some_struct&&) = delete;
        some_struct(const some_struct&) = default;

        some_struct& operator=(some_struct&&) = delete;
        some_struct& operator=(const some_struct&) = default;
    };

    framework::optional<some_struct> opt;

    EXPECT_FALSE(static_cast<bool>(opt));
    EXPECT_FALSE(opt.has_value());

    constexpr uint32_t inner_value = 5;
    some_struct value; value.value = inner_value;
    opt = value;

    EXPECT_TRUE(static_cast<bool>(opt));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value().value, inner_value);
    // ReSharper disable once CppRedundantDereferencingAndTakingAddress
    EXPECT_EQ((*opt).value, inner_value);
    EXPECT_EQ(opt->value, inner_value);
}

TEST(TestOptional, test_clear_primitive) {
    framework::optional<uint32_t> opt;

    constexpr uint32_t value = 5;
    opt = value;

    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), value);

    opt = framework::nullopt;

    EXPECT_FALSE(opt.has_value());
}

TEST(TestOptional, test_clear_struct) {
    struct some_struct {
        uint32_t value = 0;
        bool* flag = nullptr;

        ~some_struct() {
            if (flag != nullptr) {
                *flag = true;
            }
        }
    };

    framework::optional<some_struct> opt;

    constexpr uint32_t inner_value = 5;
    bool flag = false;
    some_struct value;
    value.value = inner_value;
    value.flag = &flag;
    opt = value;

    EXPECT_EQ(opt.value().value, inner_value);
    EXPECT_FALSE(flag);

    opt = framework::nullopt;

    EXPECT_FALSE(opt.has_value());
    EXPECT_TRUE(flag);
}

