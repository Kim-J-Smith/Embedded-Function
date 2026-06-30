#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

static int free_count = 0;

struct no_double_free_checker {
    int m_var = 123;
    ~no_double_free_checker() { free_count++; }
};

// https://github.com/Kim-J-Smith/Embedded-Function/issues/75
TEST(LifetimeTest, no_double_free_in_assignment) {
    free_count = 0;
    {
        no_double_free_checker checker; // 1
        ebd::fn<int()> f1 = [checker]() { return checker.m_var; }; // 2
        ebd::fn<int()> f2;
        f2 = f1; // 1
    }
    ASSERT_EQ(free_count, 4);
}


