#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

#include <iostream>

TEST(UnaryDereference, fn_freeFunction) {
    auto f1 = ebd::make_fn(ebd_test_free_func_v);
    ASSERT_EQ(*f1, &ebd_test_free_func_v);

    auto f2 = ebd::make_fn(ebd_test_free_func_vii);
    ASSERT_EQ(*f2, &ebd_test_free_func_vii);

    auto f3 = ebd::make_fn(ebd_test_free_func_iii_add);
    ASSERT_EQ(*f3, &ebd_test_free_func_iii_add);

    auto f4 = ebd::make_fn([]{ return 1; });
    ASSERT_EQ(*f4 == NULL, true);

    auto f5 = ebd::make_fn(+[]{ return 1; });
    ASSERT_EQ(*f5 == NULL, false);

    auto f6 = ebd::make_fn(&ebd_test_member_fn::mem_fn_ii_add);
#ifdef _MSC_VER
    // Strange behaviour in MSVC.
    ASSERT_EQ(*f6 == NULL, false);
    auto* f6_fn_ptr = *f6;
    ebd_test_member_fn obj;
    ASSERT_EQ(f6_fn_ptr(obj, 1123, 3345), 1123 + 3345);
#else
    ASSERT_EQ(*f6 == NULL, true);
#endif
}

TEST(UnaryDereference, fn_view_freeFunction) {
    ebd::fn_view<void()> f1 = ebd_test_free_func_v;
    ASSERT_EQ(*f1, &ebd_test_free_func_v);
}
