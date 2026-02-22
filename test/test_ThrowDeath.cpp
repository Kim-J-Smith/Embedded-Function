#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

TEST(ThrowDeath, throw_bad_function_call) {
    ebd::fn<void()> f1;
    ASSERT_EQ(f1.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f1), false);
    EBD_EXPECT_THROW(f1(), std::bad_function_call);

    ebd::unique_fn<void()> f2;
    ASSERT_EQ(f2.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f2), false);
    EBD_EXPECT_THROW(f2(), std::bad_function_call);

    auto f3 = ebd::make_fn<void()>();
    ASSERT_EQ(f3.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f3), false);
    EBD_EXPECT_THROW(f3(), std::bad_function_call);

    auto f4 = ebd::make_fn<void()>(nullptr);
    ASSERT_EQ(f4.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f4), false);
    EBD_EXPECT_THROW(f4(), std::bad_function_call);
}

TEST(ThrowDeath, dead_when_empty_call) {
    ebd::safe_fn<void()> f1;
    ASSERT_EQ(f1.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f1), false);
    EXPECT_DEATH(f1(), "");

    ebd::fn_view<void()> f2;
    ASSERT_EQ(f2.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f2), false);
    EXPECT_DEATH(f2(), "");
}
