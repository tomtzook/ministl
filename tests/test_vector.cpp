
#include <gtest/gtest.h>
#include <vector.h>

#include "expect.h"

struct trivial_dummy_struct {
    int i;
    float f;
};

struct nontrivial_dummy_struct {
    nontrivial_dummy_struct()
        : i(1)
        , f(2.0f)
    {}
    nontrivial_dummy_struct(const nontrivial_dummy_struct& other)
        : i(other.i + 1)
        , f(other.f + 1)
    {}
    nontrivial_dummy_struct(nontrivial_dummy_struct&& other) noexcept
        : i(other.i + 2)
        , f(other.f + 2)
    {}

    int i;
    float f;
};

TEST(TestVector, test_trivial_add_back_copy) {
    framework::vector<trivial_dummy_struct> v;

    trivial_dummy_struct s{};
    s.i = 1;
    s.f = 2.0f;
    v.push_back(s);

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 1);
    EXPECT_EQ(val.f, 2.0f);
}

TEST(TestVector, test_trivial_add_back_move) {
    framework::vector<trivial_dummy_struct> v;

    trivial_dummy_struct s{};
    s.i = 1;
    s.f = 2.0f;
    v.push_back(framework::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 1);
    EXPECT_EQ(val.f, 2.0f);
}

TEST(TestVector, test_trivial_remove_back_one_element) {
    framework::vector<trivial_dummy_struct> v;

    trivial_dummy_struct s{};
    s.i = 1;
    s.f = 2.0f;
    v.push_back(framework::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 1);
    EXPECT_EQ(val.f, 2.0f);

    v.pop_back();

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);
}

TEST(TestVector, test_trivial_remove_front_two_elements) {
    framework::vector<trivial_dummy_struct> v;

    trivial_dummy_struct s{};
    s.i = 1;
    s.f = 2.0f;
    v.push_back(framework::move(s));

    trivial_dummy_struct s2{};
    s2.i = 5;
    s2.f = 8.0f;
    v.push_back(framework::move(s2));

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 1);
    EXPECT_EQ(val.f, 2.0f);
    auto& val2 = v[1];
    EXPECT_EQ(val2.i, 5);
    EXPECT_EQ(val2.f, 8.0f);

    v.pop_front();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val3 = v[0];
    EXPECT_EQ(val3.i, 5);
    EXPECT_EQ(val3.f, 8.0f);
}

TEST(TestVector, test_trivial_add_front_alread_has) {
    framework::vector<trivial_dummy_struct> v;

    trivial_dummy_struct s{};
    s.i = 1;
    s.f = 2.0f;
    v.push_back(framework::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 1);
    EXPECT_EQ(val.f, 2.0f);

    trivial_dummy_struct s2{};
    s2.i = 5;
    s2.f = 8.0f;
    v.push_front(framework::move(s2));

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.capacity(), framework::vector<trivial_dummy_struct>::default_capacity);

    auto& val2 = v[0];
    EXPECT_EQ(val2.i, 5);
    EXPECT_EQ(val2.f, 8.0f);
    auto& val3 = v[1];
    EXPECT_EQ(val3.i, 1);
    EXPECT_EQ(val3.f, 2.0f);
}

TEST(TestVector, test_trivial_increase_capacity) {
    framework::vector<trivial_dummy_struct> v(1);

    trivial_dummy_struct s{};
    s.i = 1;
    s.f = 2.0f;
    v.push_back(framework::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 1);

    auto& val = v[0];
    EXPECT_EQ(val.i, 1);
    EXPECT_EQ(val.f, 2.0f);

    v.reserve(2);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 2);

    auto& val2 = v[0];
    EXPECT_EQ(val2.i, 1);
    EXPECT_EQ(val2.f, 2.0f);
}

TEST(TestVector, test_nontrivial_add_back_copy) {
    framework::vector<nontrivial_dummy_struct> v;

    nontrivial_dummy_struct s;
    v.push_back(s);

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<nontrivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 2);
    EXPECT_EQ(val.f, 3.0f);
}

TEST(TestVector, test_nontrivial_add_back_move) {
    framework::vector<nontrivial_dummy_struct> v;

    nontrivial_dummy_struct s;
    v.push_back(framework::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), framework::vector<nontrivial_dummy_struct>::default_capacity);

    auto& val = v[0];
    EXPECT_EQ(val.i, 3);
    EXPECT_EQ(val.f, 4.0f);
}

TEST(TestVector, test_nontrivial_increase_capacity) {
    framework::vector<nontrivial_dummy_struct> v(1);

    nontrivial_dummy_struct s;
    v.push_back(framework::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 1);

    auto& val = v[0];
    EXPECT_EQ(val.i, 3);
    EXPECT_EQ(val.f, 4.0f);

    v.reserve(2);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 2);

    auto& val2 = v[0];
    EXPECT_EQ(val2.i, 5);
    EXPECT_EQ(val2.f, 6.0f);
}
