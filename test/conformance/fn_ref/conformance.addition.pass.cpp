#include "test_fallback_macros.hpp"

#include <functional>
#include <utility>
#include <type_traits>

#include "__constant_wrapper.hpp"

#define EMBED_FN_HOOK_DEBUG(msg) // puts(msg)

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

template <class Sig, std::size_t Buf>
using nothrow_fn = ebd::basic_fn<Sig, ebd::detail::get_aligned_size(Buf), true, false, false, false>;

struct empty_trivial_but_state {
  std::uintptr_t operator()() const { return reinterpret_cast<std::uintptr_t>(this); }
};

#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)

struct Static_call_operator_test {
  int operator()(int) const { return 0; }
  static int operator()(long) { return 1; }
};

#endif

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

    ebd_test_operator_unambiguous obj{};
    auto f3 = ebd::make_fn<nothrow_fn>(obj);
    static_assert(std::is_same<decltype(f3), nothrow_fn<int(int) & noexcept, sizeof(obj)>>::value, "BUG");
    ASSERT_EQ(f3(10), 10);
    ASSERT_EQ(f3(10), 20);

    auto f4 = ebd::make_fn<ebd::fn_ref>(obj);
    static_assert(std::is_same<decltype(f4), ebd::fn_ref<int(int) noexcept, sizeof(obj)>>::value, "BUG");
    ASSERT_EQ(obj(10), 10);
    ASSERT_EQ(f4(10), 20);
    ASSERT_EQ(f4(10), 30);

    auto ptm = &ebd_test_member_fn::get_var_and_increase;
    auto f5 = ebd::make_fn<nothrow_fn>(ptm);
    static_assert(std::is_same<
      decltype(f5), 
      nothrow_fn<int(ebd_test_member_fn &, int) const noexcept, sizeof(ptm)>
    >::value, "BUG");
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

#if __cpp_lib_constant_wrapper >= 202603L

  {
    ebd_test_member_fn obj;
    obj.member_var = 0;
    auto rx = std::ref(obj);
    ebd::fn_ref<int(int)> f1(std::cw<&ebd_test_member_fn::get_var_and_increase>, rx);

    // Should this be allowed? `get_var_and_increase` is a non-const member function,
    // but `f2` expects a const-qualified function type `int(int) const`.
    // See P3961r1 (Why stop at function_ref as opposed to extending to reference_wrapper).
    // <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3961r1.html#Why-stop-at-function_ref-as-opposed-to-extending-to-reference_wrapper>
    //
    // In embed_function.hpp namespace `invocation`:
    //    `auto& obj = *static_cast<Obj C V*>(base.val.fill_ptr);`.
    // Here `Obj` is `std::reference_wrapper<ebd_test_member_fn>`, so the underlying
    // `ebd_test_member_fn` will not be qualified with const. This would allow calling a non-const
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

# if defined(__GNUC__) && !defined(__clang__)
    ASSERT_DEATH({
      ebd::fn_ref<void()> f2(static_cast<void (*)()>(nullptr));
    }, "");
# endif // GCC

#endif
  }

#endif

  {
    ebd_test_operator_unambiguous lref_callable{};
    ebd::fn_ref<int(int)> f = lref_callable;
    ASSERT_EQ(f(1), 1);
    ASSERT_EQ(f(1), 2);
  }

  {
    ebd::fn_ref<int(int,int)> f1 = ebd_test_free_func_iii_add;
    static constexpr auto size_ = sizeof(ebd::fn_ref<int(int,int)>);
    ebd::fn<int(int,int), size_> f2 = f1;
    ebd::unique_fn<int(int,int), size_> f3 = f1;
    ebd::classic_fn<int(int,int), size_> f4 = f1;
    ebd::__safe_fn<int(int,int), size_> f8 = f1;
    ASSERT_EQ(f1(1, 42), 43);
    ASSERT_EQ(f2(1, 42), 43);
    ASSERT_EQ(f3(1, 42), 43);
    ASSERT_EQ(f4(1, 42), 43);
    ASSERT_EQ(f8(1, 42), 43);

    auto f5 = ebd::make_fn<ebd::fn>(f1);
    auto f6 = ebd::make_fn<ebd::unique_fn>(f1);
    auto f7 = ebd::make_fn<ebd::classic_fn>(f1);
    auto f9 = ebd::make_fn<ebd::__safe_fn>(f1);
    ASSERT_EQ(f5(2, 42), 44);
    ASSERT_EQ(f6(2, 42), 44);
    ASSERT_EQ(f7(2, 42), 44);
    ASSERT_EQ(f9(2, 42), 44);
  }

  {
    auto f = ebd::make_fn<ebd::fn_ref>(std::less<int>{});
    ASSERT_EQ(f(3, 4), true);
    ASSERT_EQ(f(4, 3), false);
  }

# if (EMBED_CXX_VERSION >= 202002L && __cpp_constexpr >= 202002L)
  {
    static constexpr auto l = [] { return 42; };
    constexpr ebd::fn_ref<int()> f1 = l;
    ASSERT_EQ(f1(), 42);

    constexpr ebd::fn_ref<int(int, int)> f2 = std::plus{};
    ASSERT_EQ(f2(1, 2), 3);
  }
#endif

#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)
  {
    ebd::fn_ref<int(int, int)> f1 = ebd_test_static_call_operator{};
    ASSERT_EQ(f1(0, 42), 42);

    auto f2 = ebd::make_fn<ebd::fn_ref>(ebd_test_static_call_operator{});
    static_assert(std::is_same_v<decltype(f2), ebd::fn_ref<int(int, int) const noexcept>>, "BUG");
    ASSERT_EQ(f2(1, 42), 43);
  }
  {
    // static call operator
    Static_call_operator_test obj{};
    ebd::fn_ref<int(long)> f = obj;
    ASSERT_EQ(f(0), 1);
    ebd::fn_ref<int(long) const> f1 = obj;
    ASSERT_EQ(f1(0), 1);
  }
#endif

  {
#if EMBED_CXX_VERSION >= 202002L && __cpp_lib_ranges >= 202110L

    ebd::fn_ref<bool(int, int) const> f3 = std::ranges::less{}; // stateless
    ASSERT_EQ(f3(1, 2), true);

    ebd::fn_ref<bool(int, int) const> f4 = std::ranges::greater_equal{}; // stateless
    ASSERT_EQ(f4(4, 4), true);
    ASSERT_EQ(f4(5, 4), true);

#endif // __cpp_lib_ranges >= 202110L
  }

  {
    empty_trivial_but_state obj;
    auto f1 = ebd::make_fn<ebd::fn_ref>(obj);
    ASSERT_EQ(f1(), obj());
  }

  {
    int result = 42;
    ASSERT_EQ(ebd_test_safe_tmp_fn([result]{ return result; }), result);
  }
}
