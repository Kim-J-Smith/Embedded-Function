#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

TEST(TestSwap, fn_swap) {
    auto f1 = ebd::make_fn(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, fn_ref_swap) {
    auto f1 = ebd::fn_ref<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2 = ebd_test_free_func_iii_add;

    f2.swap(f1);

    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, safe_fn_swap) {
    auto f1 = ebd::safe_fn<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, unique_fn_swap) {
    auto f1 = ebd::unique_fn<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}
