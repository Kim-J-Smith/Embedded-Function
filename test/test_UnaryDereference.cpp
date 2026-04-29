#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

#include <iostream>

TEST(UnaryDereference, fn_freeFunction) {
    auto f1 = ebd::make_fn(ebd_test_free_func_v);
    ASSERT_EQ(*f1, &ebd_test_free_func_v);

    auto f2 = ebd::make_fn(ebd_test_free_func_vii);
    ASSERT_EQ(*f2, &ebd_test_free_func_vii);

    const auto f3 = ebd::make_fn(ebd_test_free_func_iii_add);
    ASSERT_EQ(*f3, &ebd_test_free_func_iii_add);

    auto f4 = ebd::make_fn([]{ return 1; });
    ASSERT_EQ(*f4 == nullptr, true);

    auto f5 = ebd::make_fn(+[]{ return 1; });
    ASSERT_EQ(*f5 == nullptr, false);

    auto f6 = ebd::make_fn(&ebd_test_member_fn::mem_fn_ii_add);
    ASSERT_EQ(*f6 == nullptr, true);

    auto f7 = ebd::unique_fn<void()>(ebd_test_free_func_v);
    ASSERT_EQ(*f7 == nullptr, false);
}

TEST(UnaryDereference, fn_ref_freeFunction) {
    ebd::fn_ref<void()> f1 = ebd_test_free_func_v;
    ASSERT_EQ(*f1, &ebd_test_free_func_v);
}
