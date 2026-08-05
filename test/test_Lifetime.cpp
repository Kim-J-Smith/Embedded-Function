#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

static int free_count = 0;

struct no_double_free_checker {
    int m_var = 123;
    ~no_double_free_checker() noexcept { free_count++; }
};

// https://github.com/Kim-J-Smith/Embedded-Function/issues/75
TEST(LifetimeTest, no_double_free_in_assignment) {
    // fn
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::fn<int()> f2;
        f2 = f1; // 1
    }
    ASSERT_EQ(free_count, 4);

    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::fn<int()> f2;
        f2 = std::move(f1); // 1
    }
    ASSERT_EQ(free_count, 4);

    // unique_fn
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::unique_fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::unique_fn<int()> f2;
        f2 = std::move(f1); // 1
    }
    ASSERT_EQ(free_count, 4);

    // classic_fn
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::classic_fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::classic_fn<int()> f2;
        f2 = f1; // 1
    }
    ASSERT_EQ(free_count, 4);

    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::classic_fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::classic_fn<int()> f2;
        f2 = std::move(f1); // 1
    }
    ASSERT_EQ(free_count, 4);

    // __safe_fn
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::__safe_fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::__safe_fn<int()> f2;
        f2 = f1; // 1
    }
    ASSERT_EQ(free_count, 4);

    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::__safe_fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::__safe_fn<int()> f2;
        f2 = std::move(f1); // 1
    }
    ASSERT_EQ(free_count, 4);

    // fn -> unique_fn
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::unique_fn<int()> f2;
        f2 = f1; // 1 + 1 = 2
    }
    ASSERT_EQ(free_count, 5);

    // small buffer -> big buffer
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        auto f1 = ebd::make_fn([checker]() { return checker.m_var; }); // 2
        ebd::fn<int(), 10 * sizeof(void*)> f2;
        f2 = f1; // 1 + 1 = 2
    }
    ASSERT_EQ(free_count, 5);

    // small buffer -> big buffer & fn -> unique_fn
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        auto f1 = ebd::make_fn([checker]() { return checker.m_var; }); // 2
        ebd::unique_fn<int(), 10 * sizeof(void*)> f2;
        f2 = f1; // 1 + 1 = 2
    }
    ASSERT_EQ(free_count, 5);
}


