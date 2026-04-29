//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Additional modifications:
// Copyright (c) 2026 Kim-J-Smith
// SPDX-License-Identifier: MIT
// 
//===----------------------------------------------------------------------===//

// REQUIRES: C++11 ~ C++26

// template<class F> constexpr fn_ref(F&&) noexcept;

#include <functional>
#include <utility>
#include <type_traits>

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

#define STATIC_ASSERT_(...) static_assert((__VA_ARGS__), "BUG")
#define ASSERT_(...) ASSERT_EQ((__VA_ARGS__) == true, true)
#ifndef TEST_IS_CONSTANT_EVALUATED
# define TEST_IS_CONSTANT_EVALUATED 0
#endif

// Constraints:
// - remove_cvref_t<F> is not the same type as function_ref,
// - is_member_pointer_v<T> is false, and
// - is-invocable-using<cv T&> is true.

struct A {
  int i;
  void f() {}
#if EMBED_CXX_VERSION >= 202002L
  void operator()(auto...) const {}
#else
  template <typename... Args>
  void operator()(Args...) const {}
#endif
};

template <typename T>
using remove_cvref_t = typename std::remove_cv<
  typename std::remove_reference<T>::type
>::type;

const auto l1 = [] {};
using L1          = remove_cvref_t<decltype(l1)>;

const auto l2 = [] { return A{5}; };
using L2          = remove_cvref_t<decltype(l2)>;

#if __cpp_constexpr >= 201603L
  constexpr auto l2_noexcept = []() noexcept { return A{5}; };
  using L2Noexcept           = remove_cvref_t<decltype(l2_noexcept)>;
#endif

const auto l3 = [](int x, double d) { return x + d; };
using L3          = remove_cvref_t<decltype(l3)>;

#if __cpp_constexpr >= 201603L
  constexpr auto l3_noexcept = [](int x, double d) noexcept { return x + d; };
  using L3Noexcept           = remove_cvref_t<decltype(l3_noexcept)>;
#endif

struct NonConstInvocable {
  int i;

  int operator()(int x, double y) {
    ++i;
    return static_cast<int>(x + y + i);
  }
};

// non-const noexcept(false)
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, L1&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, L1 const&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, L1&&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, L1 const&&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, L2&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, ebd::fn_ref<int()>&>::value);

STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>, L3&>::value);
// STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>>::value); // CANNOT IMPLEMENT
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>, void (A::*)()>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*)>, void (A::*)()>::value);

// the constructor is noexcept
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, L1&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, L1 const&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, L1&&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, L1 const&&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, ebd::fn_ref<int()>&>::value);

#if __cpp_noexcept_function_type >= 201510L && __cpp_constexpr >= 201603L
  // non-const noexcept(true)
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept const&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept&&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept const&&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<A() noexcept>, ebd::fn_ref<A&() noexcept>>::value);

  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<A() noexcept>, L2&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<A() noexcept>, L2 const&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>, L3&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void() noexcept>, void (A::*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*) noexcept>, void (A::*)() noexcept>::value);

  // the constructor is noexcept
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept const&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept&&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<A() noexcept>, L2Noexcept const&&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<A() noexcept>, ebd::fn_ref<A&() noexcept>>::value);
#endif

// const noexcept(false)
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const>, L3&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const>, const L3&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const>, L3&&>::value);
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const>, const L3&&>::value);
STATIC_ASSERT_(
    std::is_constructible<ebd::fn_ref<void(int, double) const>, ebd::fn_ref<int(int, double) const>>::value);

STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double)>, NonConstInvocable&>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double)>, const NonConstInvocable&>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const>, NonConstInvocable&>::value);
// STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const>>::value); // CANNOT IMPLEMENT
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const>, void (A::*)()>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*) const>, void (A::*)()>::value);

// the constructor is noexcept
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>, L3&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>, const L3&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>, L3&&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>, const L3&&>::value);
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>,
                                              ebd::fn_ref<int(int, double) const>>::value);

#if __cpp_noexcept_function_type >= 201510L && __cpp_constexpr >= 201603L
  // const noexcept(true)
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, L3Noexcept&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, const L3Noexcept&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, L3Noexcept&&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, const L3Noexcept&&>::value);
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>,
                                        ebd::fn_ref<int(int, double) const noexcept>>::value);

  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, L3&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, const L3&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, L3&&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, const L3&&>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void*>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void (A::*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*) const>, void (A::*)() noexcept>::value);

  // the constructor is noexcept
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const noexcept>, L3Noexcept&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const noexcept>, const L3Noexcept&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const noexcept>, L3Noexcept&&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const noexcept>, const L3Noexcept&&>::value);
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const noexcept>,
                                                ebd::fn_ref<int(int, double) const noexcept>>::value);
#endif

struct F {
  int i;
#if EMBED_CXX_VERSION >= 202002L
  int operator()(auto&&...) { return 5 + i; }

  int operator()(auto&&...) const { return 6 + i; }
#else
  template <typename... Args>
  int operator()(Args&&...) { return 5 + i; }

  template <typename... Args>
  int operator()(Args&&...) const { return 6 + i; }
#endif
};

struct Int {
  int i;
  constexpr Int(int ii) noexcept : i(ii) {}
};

struct NeedsConversion {
  int operator()(Int x, Int y, Int z) const noexcept { return x.i + y.i + z.i; }
};

int fn() { return 5; }
#if __cpp_noexcept_function_type >= 201510L && __cpp_constexpr >= 201603L
  constexpr auto fn_noexcept = []() noexcept { return 6; };

# if 0 && __cpp_lib_function_ref >= 202603L
  const auto one = []() noexcept { return 1; };
  const auto two = []() noexcept { return 2; };
# endif
#endif

struct VolatileCallable {
  int operator()() volatile const { return 42; }
};

TEST(Conformance_fn_ref, ref_pass) {
  static_cast<void>(l2);

  {
    ebd::fn_ref<void()> f(l1);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      f();
    }
  }
  {
    // explicit(false)
    ebd::fn_ref<void()> f = l1;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      f();
    }
  }
#if __cpp_noexcept_function_type >= 201510L && __cpp_constexpr >= 201603L
  {
    // noexcept
    ebd::fn_ref<A() noexcept> f(l2_noexcept);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      auto a = f();
      ASSERT_(a.i == 5);
    }
  }
#endif
  {
    // const
    ebd::fn_ref<double(int, double) const> f(l3);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2.0) == 3.0);
    }
  }
#if __cpp_noexcept_function_type >= 201510L && __cpp_constexpr >= 201603L
  {
    // const noexcept
    ebd::fn_ref<double(int, double) const noexcept> f(l3_noexcept);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2.0) == 3.0);
    }
  }
#endif
  {
    // no copies of original callable
    int i = 5;
    auto local = [i]() mutable { return i++; };
    ebd::fn_ref<int()> f(local);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f() == 5);
      ASSERT_(local() == 6);
      ASSERT_(f() == 7);
    }
  }
#if __cpp_lib_as_const >= 201510L
  {
    // const correctness
    F f{5};

    ebd::fn_ref<int()> f1(f);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f1() == 10);
      ASSERT_(std::as_const(f1)() == 10);
    }

    ebd::fn_ref<int() const> f2(f);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 11);
      ASSERT_(std::as_const(f2)() == 11);
    }
  }
#endif
  {
    // with conversions
    NeedsConversion c{};

    ebd::fn_ref<Int(int, int, int)> f(c);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f2(c);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2(1, 2, 3).i == 6);
    }
#if __cpp_noexcept_function_type >= 201510L
    ebd::fn_ref<Int(int, int, int) noexcept> f3(c);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f3(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f4(c);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f4(1, 2, 3).i == 6);
    }
#endif
  }
#if __cpp_noexcept_function_type >= 201510L && __cpp_constexpr >= 201603L
  {
    // noexcept conversion
    ebd::fn_ref<int() noexcept> f1(fn_noexcept);
    ebd::fn_ref<int()> f2(f1);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 6);
    }
  }

  {
    // const conversion
    ebd::fn_ref<int() const> f1(fn_noexcept);
    ebd::fn_ref<int()> f2(f1);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 6);
    }
  }

  {
    // const noexcept conversion
    ebd::fn_ref<int() const noexcept> f1(fn_noexcept);
    ebd::fn_ref<int()> f2(f1);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 6);
    }
  }

  {
    // const noexcept to noexcept conversion
    ebd::fn_ref<int() const noexcept> f1(fn_noexcept);
    ebd::fn_ref<int() noexcept> f2(f1);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 6);
    }
  }

  {
    // const noexcept to const conversion
    ebd::fn_ref<int() const noexcept> f1(fn_noexcept);
    ebd::fn_ref<int() const> f2(f1);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 6);
    }
  }
#endif
#if 0 && __cpp_lib_function_ref >= 202603L
  {
    // P3961R1 Less double indirection in function_ref
    // double unwrapping
    ebd::fn_ref<int() const noexcept> f1(std::cw<one>);
    ebd::fn_ref<int()> f2(f1);

    f1 = std::cw<two>;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 1 || f2() == 2);
      ASSERT_(f2() == 1);
    }
  }
#endif
  {
    // volatile objects
    const volatile VolatileCallable vc{};
    ebd::fn_ref<int()> f(vc);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f() == 42);
    }
  }
}
