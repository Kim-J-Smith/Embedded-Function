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

// template<class F> fn_ref(F* f) noexcept;

#include <functional>
#include <utility>
#include <type_traits>

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

// Constraints:
// - is_function_v<F> is true, and
// - is-invocable-using<F> is true.

#define STATIC_ASSERT_(...) static_assert((__VA_ARGS__), "BUG")
#define ASSERT_(...) ASSERT_EQ((__VA_ARGS__) == true, true)

struct A {
  int i;
  void f() {}
  // void operator()(auto...) const {}
};

// non-const noexcept(false)
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, void (*)()>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>, void*>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>, void (*)(int)>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void()>, void (A::*)()>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*)>, void (A::*)()>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void()>, void (*)() noexcept>::value);
#endif

// the constructor is noexcept
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, void (*)()>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void()>, void (*)() noexcept>::value);
#endif

#if __cpp_noexcept_function_type >= 201510L
  // non-const noexcept(true)
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<A() noexcept>, A (*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<A() noexcept>, A (*)()>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<A() noexcept>, A*>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void() noexcept>, void (*)(int) noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void() noexcept>, void (A::*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*) noexcept>, void (A::*)() noexcept>::value);

  // the constructor is noexcept
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<A() noexcept>, A (*)() noexcept>::value);
#endif

// const noexcept(false)
STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const>, void (*)(int, double)>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const>, void*>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const>, void (*)(int, A)>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const>, void (A::*)()>::value);
STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*) const>, void (A::*)()>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_constructible<ebd::fn_ref<void(int, double) const>, void (*)(int, double) noexcept>::value);
#endif

// the constructor is noexcept
STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>, void (*)(int, double)>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(
    std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const>, void (*)(int, double) noexcept>::value);
#endif

#if __cpp_noexcept_function_type >= 201510L
  // const noexcept(true)
  STATIC_ASSERT_(
      std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void (*)(int, double) noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void (*)(int, double)>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void*>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void (*)(int, A) noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(int, double) const noexcept>, void (A::*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_constructible<ebd::fn_ref<void(A*) const>, void (A::*)() noexcept>::value);
#endif

// the constructor is noexcept
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_nothrow_constructible<ebd::fn_ref<void(int, double) const noexcept>,
                                               void (*)(int, double) noexcept>::value);
#endif

static int fn() { return 42; }

// static int fn_maythrow(int i, A a) { return i - a.i; }
static int fn_noexcept(int i, A a) noexcept { return i + a.i; }

static void foo(int) {}
static void bar(int) noexcept {}
struct Int {
  int i;
  Int(int ii) noexcept : i(ii) {}
};

static int needs_conversion(Int x, Int y, Int z) noexcept { return x.i + y.i + z.i; }

TEST(Conformance_fn_ref, function_ptr_pass) {
  {
    // simple case
    ebd::fn_ref<int()> f(&fn);
    ASSERT_(f() == 42);
  }
  {
    // explicit(false)
    ebd::fn_ref<int()> f = &fn;
    ASSERT_(f() == 42);
  }
  {
    ebd::fn_ref<int(int, A)> f(&fn_noexcept);
    ASSERT_(f(4, A{5}) == 9);
  }
  {
    // noexcept
    ebd::fn_ref<int(int, A) noexcept> f(&fn_noexcept);
    ASSERT_(f(4, A{5}) == 9);
  }
  {
    // const
    auto l = [](int x, int y, int z) { return x + y - z; };
    ebd::fn_ref<int(int, int, int) const> f(+l);
    ASSERT_(f(2, 3, 4) == 1);
  }
  {
    // const noexcept
    ebd::fn_ref<int(int, A) const noexcept> f(&fn_noexcept);
    ASSERT_(f(4, A{5}) == 9);
  }

  {
    ebd::fn_ref<Int(int, int, int)> f(&needs_conversion);
    ASSERT_(f(1, 2, 3).i == 6);

    ebd::fn_ref<Int(int, int, int) const> f2(&needs_conversion);
    ASSERT_(f2(1, 2, 3).i == 6);

    ebd::fn_ref<Int(int, int, int) noexcept> f3(&needs_conversion);
    ASSERT_(f3(1, 2, 3).i == 6);

    ebd::fn_ref<Int(int, int, int) const noexcept> f4(&needs_conversion);
    ASSERT_(f4(1, 2, 3).i == 6);

    {
      auto r1 = ebd::make_fn(foo);
      auto r2 = ebd::make_fn(bar);
      r1                   = r2; // ok
    }
  }
}
