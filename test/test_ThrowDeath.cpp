#include "test_fallback_macros.hpp"

#define EMBED_NODISCARD

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

// ThrowDeath[0]
TEST(ThrowDeath, throw_bad_function_call) {
    ebd::classic_fn<void()> f1;
    ASSERT_EQ(f1.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f1), false);
    EBD_EXPECT_THROW(f1(), std::bad_function_call);
}

// ThrowDeath[0.5]
TEST(ThrowDeath, terminate_on_empty_call) {
    ebd::fn<void()> f1;
    ASSERT_EQ(f1.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f1), false);
    EXPECT_DEATH(f1(), "");

    ebd::unique_fn<void()> f2;
    ASSERT_EQ(f2.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f2), false);
    EXPECT_DEATH(f2(), "");

    ebd::__safe_fn<void()> sf;
    ASSERT_EQ(sf.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(sf), false);
    EXPECT_DEATH(sf(), "");

    auto f3 = ebd::make_fn<void()>();
    ASSERT_EQ(f3.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f3), false);
    EXPECT_DEATH(f3(), "");

    auto f4 = ebd::make_fn<void()>(nullptr);
    ASSERT_EQ(f4.is_empty(), true);
    ASSERT_EQ(static_cast<bool>(f4), false);
    EXPECT_DEATH(f4(), "");
}

// ThrowDeath[1] — fn/unique_fn terminate on empty; safe_fn (deprecated) also terminates
TEST(ThrowDeath, mix_throw_and_death) {
    auto f1 = ebd::make_fn(ebd_test_free_func_iii_add);
    ASSERT_EQ(f1 != nullptr, true);
    ASSERT_EQ(f1(123, 234), 123 + 234);

    f1.clear();
    ASSERT_EQ(f1 == nullptr, true);
    EXPECT_DEATH(f1(1, 2), "");

    auto f2 = ebd::make_fn(ebd_test_operator_unambiguous{});
    ASSERT_EQ(f2 != nullptr, true);
    ASSERT_EQ(f2(1), 1);

    f2 = ebd_test_operator_unambiguous{};
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(3), 3);

    f2 = nullptr;
    EXPECT_DEATH(f2(2), "");

    auto f3 = ebd::make_fn<ebd::classic_fn>(ebd_test_operator_unambiguous{});
    ASSERT_EQ(f3.is_empty(), false);
    ASSERT_EQ(f3(3), 3);

    f3.clear();
    EBD_EXPECT_THROW(f3(3), std::bad_function_call);
}

#if !defined(_MSC_VER)
// ThrowDeath[3]
struct ThrowDeath_must_fail {
    ThrowDeath_must_fail() = default;
    ~ThrowDeath_must_fail() noexcept(false) {
        EBD_THROW(std::exception{});
    }
    void operator()() noexcept {}
};
void ebd_test_fail() {
    []{ebd::make_fn(ThrowDeath_must_fail{});}();
}
TEST(ThrowDeath, fail_make_fn) {
    EBD_EXPECT_THROW(ebd_test_fail(), std::exception);
}
#endif
