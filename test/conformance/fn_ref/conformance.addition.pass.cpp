#include <functional>
#include <utility>
#include <type_traits>

#include "__constant_wrapper.hpp"

#define EMBED_FN_HOOK_DEBUG(msg) // puts(msg)

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

template <class Sig, std::size_t Buf>
using nothrow_fn = ebd::basic_fn<Sig, Buf, true, false, false, false>;

static std::reference_wrapper<ebd_test_member_fn> get_ref() {
  static ebd_test_member_fn x;
  x.member_var = 0;
  return x;
}

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
    static_assert(std::is_same<decltype(f1), ebd::fn_ref<int() const noexcept, sizeof(void(*)())>>::value, "BUG");
    ASSERT_EQ(f1(), 0);

    auto f2 = ebd::make_fn<nothrow_fn>(ebd_test_free_func_noexcept);
    static_assert(std::is_same<decltype(f2), nothrow_fn<int() const noexcept, sizeof(void(*)())>>::value, "BUG");
    ASSERT_EQ(f2(), 0);
  }

  {
    // https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3961r1.html
    ebd::fn_ref<int() noexcept> f_noexcept = ebd_test_free_func_noexcept;
    ebd::fn_ref<int() noexcept(false)> f_maythrow = ebd_test_free_func_maythrow;

    ASSERT_EQ(f_noexcept(), 0);
    ASSERT_EQ(f_maythrow(), 1);

    f_maythrow = f_noexcept;
    ASSERT_EQ(f_maythrow(), 0); // call ebd_test_free_func_noexcept
    // f_noexcept = f_maythrow; // ERROR

    f_noexcept = ebd_test_free_func_noexcept_2;
    ASSERT_EQ(f_noexcept(), 2);
    ASSERT_EQ(f_maythrow(), 0); // call ebd_test_free_func_noexcept
                                // It should not be affected by f_noexcept.
  }
#endif

  {
    // volatile is still supported.
    volatile ebd::fn_ref<void() volatile> f1 = +[] { volatile int a = 1; (void)a; };
    f1();

    volatile ebd_test_operator_qualifier obj{};
    ebd::fn_ref<int(int) volatile> f2 = obj;
    ASSERT_EQ(f2(0), OVL_VOLATILE);
  }

  {
    static_cast<void>(&get_ref); // unused
  }

#if __cpp_lib_constant_wrapper >= 202603L

  {
    auto rx = get_ref();
    ebd::fn_ref<int(int)> f1(std::cw<&ebd_test_member_fn::get_var_and_increase>, rx);

    // Should this be allowed? `get_var_and_increase` is a non-const member function,
    // but `f2` expects a const-qualified function type `int(int) const`.
    // See P3961r1 (Why stop at function_ref as opposed to extending to reference_wrapper).
    // <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3961r1.html#Why-stop-at-function_ref-as-opposed-to-extending-to-reference_wrapper>
    //
    // In embed_function.hpp line 1568: `C V auto& obj = *erased->template access<Obj*>()`.
    // Here `Obj` is `std::reference_wrapper<ebd_test_member_fn>`, so the underlying
    // `ebd_test_member_fn` is not const-qualified. This would allow calling a non-const
    // member function through a const-qualified function_ref, which violates const-correctness.
    // Therefore, P3961r1 recommends that such construction be disallowed.
    //
    // TODO: C++26 draft has not adopted this recommendation because they want to see
    // evidence in real-world code to justify this change. (2026.5.9)
    ebd::fn_ref<int(int) const> f2(std::cw<&ebd_test_member_fn::get_var_and_increase>, rx);
  }

  {
#if !defined(NDEBUG)
    ASSERT_DEATH({ 
      ebd::fn_ref<int(int)> f1(
        std::cw<&ebd_test_member_fn::get_var_and_increase>, 
        static_cast<ebd_test_member_fn*>(nullptr));
    }, "");
#endif
  }

#endif
}
