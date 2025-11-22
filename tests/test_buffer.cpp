
#include <gtest/gtest.h>
#include <buffer.h>

#include "expect.h"

TEST(TestBuffer, test_create) {
    static constexpr auto size = 16;

    auto buffer = EXPECT_VERIFY(framework::buffer::create(size));
    EXPECT_EQ(buffer.size(), size);
}

TEST(TestBuffer, test_copy) {
    static constexpr auto size = 16;

    auto buffer = EXPECT_VERIFY(framework::buffer::create(size));
    buffer.fill('A');

    auto buffer2 = EXPECT_VERIFY(buffer.copy());
    EXPECT_EQ(memcmp(buffer.data(), buffer2.data(), buffer.size()), 0);
}

TEST(TestBuffer, test_sub_buffer) {
    static constexpr auto size = 16;

    auto buffer = EXPECT_VERIFY(framework::buffer::create(size));
    buffer.fill('A');
    buffer[2] = 'b';
    buffer[4] = 'c';

    auto sub_buffer = EXPECT_VERIFY(buffer.sub(2, 3));
    EXPECT_EQ(sub_buffer.size(), 3);
    EXPECT_EQ(sub_buffer[0], 'b');
    EXPECT_EQ(sub_buffer[2], 'c');
}

TEST(TestBuffer, test_view) {
    static constexpr auto size = 16;

    auto buffer = EXPECT_VERIFY(framework::buffer::create(size));

    auto view = buffer.view<>();
    EXPECT_EQ(view.data(), buffer.data());
    EXPECT_EQ(view.size(), buffer.size());
    EXPECT_EQ(view.size_bytes(), buffer.size());

    auto view2 = buffer.view<uint16_t>();
    EXPECT_EQ(view2.data(), buffer.data<uint16_t>());
    EXPECT_EQ(view2.size() * sizeof(uint16_t), buffer.size());
    EXPECT_EQ(view2.size_bytes(), buffer.size());
}
