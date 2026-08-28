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

// REQUIRES: std-at-least-c++26

// template<auto f> constexpr fn_ref(constant_wrapper<f>) noexcept;

#include <functional>
#include <utility>
#include <type_traits>

#include "__constant_wrapper.hpp"

#include "test_function.hpp"

#if __cpp_lib_constant_wrapper >= 202603L

#define ASSERT_(...) ASSERT_EQ((__VA_ARGS__) == true, true)
#ifndef TEST_IS_CONSTANT_EVALUATED
# define TEST_IS_CONSTANT_EVALUATED 0
#endif

// Constraints: is-invocable-using<const F&> is true.

static auto l1          = [] {};
static auto l1_noexcept = [] noexcept {};
static auto l2          = [](int) {};
static auto l2_noexcept = [](int) noexcept {};

struct NonConstInvocable {
  void operator()() noexcept {}
};

// non-const noexcept(false)
static_assert(std::is_constructible_v<ebd::fn_ref<void()>, std::constant_wrapper<l1>>);
// LWG issue 4256
// https://cplusplus.github.io/LWG/issue4256
static_assert(!std::is_constructible_v<ebd::fn_ref<void()>, std::constant_wrapper<NonConstInvocable{}>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void()>, std::constant_wrapper<l2>>);
static_assert(std::is_constructible_v<ebd::fn_ref<void(int)>, std::constant_wrapper<l2>>);

static_assert(std::is_nothrow_constructible_v<ebd::fn_ref<void()>, std::constant_wrapper<l1>>);
static_assert(std::is_nothrow_constructible_v<ebd::fn_ref<void(int)>, std::constant_wrapper<l2>>);

// non-const noexcept
static_assert(std::is_constructible_v<ebd::fn_ref<void() noexcept>, std::constant_wrapper<l1_noexcept>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() noexcept>, std::constant_wrapper<l1>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() noexcept>, std::constant_wrapper<NonConstInvocable{}>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() noexcept>, std::constant_wrapper<l2_noexcept>>);
static_assert(std::is_constructible_v<ebd::fn_ref<void(int) noexcept>, std::constant_wrapper<l2_noexcept>>);

static_assert(std::is_nothrow_constructible_v<ebd::fn_ref<void() noexcept>, std::constant_wrapper<l1_noexcept>>);
static_assert(
    std::is_nothrow_constructible_v<ebd::fn_ref<void(int) noexcept>, std::constant_wrapper<l2_noexcept>>);

// const noexcept(false)
static_assert(std::is_constructible_v<ebd::fn_ref<void() const>, std::constant_wrapper<l1>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() const>, std::constant_wrapper<NonConstInvocable{}>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() const>, std::constant_wrapper<l2>>);
static_assert(std::is_constructible_v<ebd::fn_ref<void(int) const>, std::constant_wrapper<l2>>);

static_assert(std::is_nothrow_constructible_v<ebd::fn_ref<void() const>, std::constant_wrapper<l1>>);
static_assert(std::is_nothrow_constructible_v<ebd::fn_ref<void(int) const>, std::constant_wrapper<l2>>);

// const noexcept
static_assert(std::is_constructible_v<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<l1_noexcept>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<l1>>);
static_assert(
    !std::is_constructible_v<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<NonConstInvocable{}>>);
static_assert(!std::is_constructible_v<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<l2_noexcept>>);
static_assert(std::is_constructible_v<ebd::fn_ref<void(int) const noexcept>, std::constant_wrapper<l2_noexcept>>);

static_assert(
    std::is_nothrow_constructible_v<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<l1_noexcept>>);
static_assert(
    std::is_nothrow_constructible_v<ebd::fn_ref<void(int) const noexcept>, std::constant_wrapper<l2_noexcept>>);

double f1(int x, double y) noexcept { return x + y; }

struct Int {
  int i;
  constexpr Int(int ii) noexcept : i(ii) {}
};

struct NeedsConversion {
  int operator()(Int x, Int y, Int z) const noexcept { return x.i + y.i + z.i; }
};

static int needs_conversion_314(Int x, Int y, Int z) noexcept { return x.i + y.i + z.i; }

TEST(Conformance_fn_ref, constant_wrapper_pass) {
  {
    ebd::fn_ref<void()> f(std::cw<[] {}>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      f();
    }
  }
  {
    // explicit
    ebd::fn_ref<void()> f = std::cw<[] {}>;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      f();
    }
  }
  {
    // const
    ebd::fn_ref<int() const> f(std::cw<[] { return 42; }>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f() == 42);
    }
  }
  {
    // noexcept
    ebd::fn_ref<double(int, double) noexcept> f(std::cw<&f1>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2.0) == 3.0);
    }
  }
  {
    // const noexcept
    ebd::fn_ref<double(int, double) const noexcept> f(std::cw<&f1>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2.0) == 3.0);
    }
  }
  {
    // with conversions
    ebd::fn_ref<Int(int, int, int)> f(std::cw<NeedsConversion{}>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f2(std::cw<NeedsConversion{}>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) noexcept> f3(std::cw<NeedsConversion{}>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f3(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f4(std::cw<NeedsConversion{}>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f4(1, 2, 3).i == 6);
    }
  }
  {
    // with conversions function pointer
    ebd::fn_ref<Int(int, int, int)> f(std::cw<&needs_conversion_314>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f2(std::cw<&needs_conversion_314>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) noexcept> f3(std::cw<&needs_conversion_314>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f3(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f4(std::cw<&needs_conversion_314>);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f4(1, 2, 3).i == 6);
    }
  }

  {
    // constexpr and make_fn
    constexpr auto f = ebd::make_fn<ebd::fn_ref>(std::cw<&ebd_test_free_func_iii_add>);
    static_assert(std::is_same_v<decltype(f), const ebd::fn_ref<int(int, int) const>>);
    ASSERT_EQ(f(42, 42), 42 + 42);
  }

}

#endif // __cpp_lib_constant_wrapper >= 202603L
