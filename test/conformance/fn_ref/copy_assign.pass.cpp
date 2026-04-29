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

// constexpr fn_ref& operator=(const fn_ref&) noexcept = default;

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

STATIC_ASSERT_(std::is_copy_assignable<ebd::fn_ref<void()>>::value);
STATIC_ASSERT_(std::is_copy_assignable<ebd::fn_ref<void() const>>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_copy_assignable<ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_copy_assignable<ebd::fn_ref<void() const noexcept>>::value);
#endif

double plus(int x, double y) noexcept { return x + y; }
double minus(int x, double y) noexcept { return x - y; }

struct Int {
  int i;
  constexpr Int(int ii) noexcept : i(ii) {}
};

struct NeedsConversion {
  int operator()(Int x, Int y, Int z) const noexcept { return x.i + y.i + z.i; }
};

static int needs_conversion(Int x, Int y, Int z) noexcept { return x.i + y.i + z.i; }
static int zero(Int, Int, Int) noexcept { return 0; }

TEST(Conformance_fn_ref, copy_assign_pass) {
#if 0 && __cpp_lib_function_ref >= 202603L
  {
    ebd::fn_ref<void()> f(std::cw<[] {}>);
    ebd::fn_ref<void()> f2(std::cw<[] {}>);
    f2 = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      f();
      f2();
    }
  }
  {
    // const
    ebd::fn_ref<int() const> f(std::cw<[] { return 42; }>);
    ebd::fn_ref<int() const> f2(std::cw<[] { return 41; }>);
    f2 = f;

    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f() == 42);
      ASSERT_(f2() == 42);
    }
  }
  {
    // noexcept
    ebd::fn_ref<double(int, double) noexcept> f(std::cw<&plus>);
    ebd::fn_ref<double(int, double) noexcept> f2(std::cw<&minus>);
    f2 = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2.0) == 3.0);
      ASSERT_(f2(1, 2.0) == 3.0);
    }
  }
  {
    // const noexcept
    ebd::fn_ref<double(int, double) const noexcept> f(std::cw<&plus>);
    ebd::fn_ref<double(int, double) const noexcept> f2(std::cw<&minus>);
    f2 = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2.0) == 3.0);
      ASSERT_(f2(1, 2.0) == 3.0);
    }
  }
  {
    // with conversions
    ebd::fn_ref<Int(int, int, int)> f(std::cw<[](int, int, int) { return Int{1}; }>);
    ebd::fn_ref<Int(int, int, int)> f2(std::cw<NeedsConversion{}>);
    f = f2;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2, 3).i == 6);
      ASSERT_(f2(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f_const(std::cw<[](int, int, int) { return Int{1}; }>);
    ebd::fn_ref<Int(int, int, int) const> f2_const(std::cw<NeedsConversion{}>);
    f_const = f2_const;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_const(1, 2, 3).i == 6);
      ASSERT_(f2_const(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) noexcept> f_noexcept(std::cw<[](int, int, int) noexcept { return Int{1}; }>);
    ebd::fn_ref<Int(int, int, int) noexcept> f2_noexcept(std::cw<NeedsConversion{}>);
    f_noexcept = f2_noexcept;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_noexcept(1, 2, 3).i == 6);
      ASSERT_(f2_noexcept(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f_const_noexcept(
        std::cw<[](int, int, int) noexcept { return Int{1}; }>);
    ebd::fn_ref<Int(int, int, int) const noexcept> f2_const_noexcept(std::cw<NeedsConversion{}>);
    f_const_noexcept = f2_const_noexcept;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_const_noexcept(1, 2, 3).i == 6);
      ASSERT_(f2_const_noexcept(1, 2, 3).i == 6);
    }
  }
  {
    // with conversions function pointer
    ebd::fn_ref<Int(int, int, int)> f(std::cw<&zero>);
    ebd::fn_ref<Int(int, int, int)> f2(std::cw<&needs_conversion>);
    f = f2;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f(1, 2, 3).i == 6);
      ASSERT_(f2(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f_const(std::cw<&zero>);
    ebd::fn_ref<Int(int, int, int) const> f2_const(std::cw<&needs_conversion>);
    f_const = f2_const;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_const(1, 2, 3).i == 6);
      ASSERT_(f2_const(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) noexcept> f_noexcept(std::cw<&zero>);
    ebd::fn_ref<Int(int, int, int) noexcept> f2_noexcept(std::cw<&needs_conversion>);
    f_noexcept = f2_noexcept;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_noexcept(1, 2, 3).i == 6);
      ASSERT_(f2_noexcept(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f_const_noexcept(std::cw<&zero>);
    ebd::fn_ref<Int(int, int, int) const noexcept> f2_const_noexcept(std::cw<&needs_conversion>);
    f_const_noexcept = f2_const_noexcept;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_const_noexcept(1, 2, 3).i == 6);
      ASSERT_(f2_const_noexcept(1, 2, 3).i == 6);
    }
  }
#endif
  SUCCEED();
}
