#include <functional>
#include <utility>
#include <type_traits>

#include "__constant_wrapper.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

TEST(Conformance_fn_ref, conformance_addition_pass) {

  {
    // free function (both ref and ptr)
    ebd::fn_ref<int(int, int)> f1 = ebd_test_free_func_iii_add;
    ASSERT_EQ(f1(123, 42), 123 + 42);

    ebd::fn_ref<int(int, int)> f2 = &ebd_test_free_func_iii_add;
    ASSERT_EQ(f2(123, 42), 123 + 42);

    ebd::fn_ref<int(int, int) const> f3 = ebd_test_free_func_iii_add;
    ASSERT_EQ(f3(123, 42), 123 + 42);

    ebd::fn_ref<int(int, int) const> f4 = &ebd_test_free_func_iii_add;
    ASSERT_EQ(f4(123, 42), 123 + 42);
  }

  {
    // make_fn from free function (both ref and ptr)
    auto f1 = ebd::make_fn<ebd::fn_ref>(ebd_test_free_func_iii_add);
    ASSERT_EQ(f1(123, 42), 123 + 42);

    auto f2 = ebd::make_fn<ebd::fn_ref>(&ebd_test_free_func_iii_add);
    ASSERT_EQ(f2(123, 42), 123 + 42);

    static_assert(std::is_same<decltype(f1), ebd::fn_ref<int(int,int) const>>::value, "BUG");
    static_assert(std::is_same<decltype(f2), ebd::fn_ref<int(int,int) const>>::value, "BUG");
  }

#if __cpp_noexcept_function_type >= 201510L
  {
    // Auto deduce the noexcept qualifier
    auto f1 = ebd::make_fn<ebd::fn_ref>(ebd_test_free_func_noexcept);
    static_assert(std::is_same<decltype(f1), ebd::fn_ref<int() const noexcept>>::value, "BUG");
  }
#endif

}
