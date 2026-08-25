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

#include <functional>
#include <utility>
#include <type_traits>

#include "__constant_wrapper.hpp"

#include "test_function.hpp"

#define STATIC_ASSERT_(...) static_assert((__VA_ARGS__), "BUG")
#define ASSERT_(...) ASSERT_EQ((__VA_ARGS__) == true, true)
#ifndef TEST_IS_CONSTANT_EVALUATED
# define TEST_IS_CONSTANT_EVALUATED 0
#endif

STATIC_ASSERT_(std::is_move_assignable<ebd::fn_ref<void()>>::value);
STATIC_ASSERT_(std::is_move_assignable<ebd::fn_ref<void() const>>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_move_assignable<ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_move_assignable<ebd::fn_ref<void() const noexcept>>::value);
#endif

static double plus_2(int x, double y) noexcept { return x + y; }
static double minus_2(int x, double y) noexcept { return x - y; }

struct Int {
  int i;
  constexpr Int(int ii) noexcept : i(ii) {}
};

struct NeedsConversion {
  int operator()(Int x, Int y, Int z) const noexcept { return x.i + y.i + z.i; }
};

/// @bug In Clang 20, when a user creates two static free functions that have the
/// same name in two compile units and wraps them into two `std::cw<&free_fn>`
/// objects, a segmentation fault ( @e SIGSEGV ) occurs if one of the
/// `std::cw<&free_fn>` is called.
/// Add _4 to avoid the same name.
static int needs_conversion_4(Int x, Int y, Int z) noexcept { return x.i + y.i + z.i; }
static int zero(Int, Int, Int) noexcept { return 0; }

TEST(Conformance_fn_ref, move_assign_pass) {
  static_cast<void>(&plus_2);
  static_cast<void>(&minus_2);
  static_cast<void>(&needs_conversion_4);
  static_cast<void>(&zero);
  SUCCEED();
}

#if __cpp_lib_constant_wrapper >= 202603L

TEST(Conformance_fn_ref, move_assign_pass_0) {
  ebd::fn_ref<void()> f(std::cw<[] {}>);
  ebd::fn_ref<void()> f2(std::cw<[] {}>);
  f2 = std::move(f);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    f();
    f2();
  }
}

TEST(Conformance_fn_ref, move_assign_pass_1) {
  // const
  ebd::fn_ref<int() const> f(std::cw<[] { return 42; }>);
  ebd::fn_ref<int() const> f2(std::cw<[] { return 41; }>);
  f2 = std::move(f);

  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f() == 42);
    ASSERT_(f2() == 42);
  }
}

TEST(Conformance_fn_ref, move_assign_pass_2) {
  // noexcept
  ebd::fn_ref<double(int, double) noexcept> f(std::cw<&plus_2>);
  ebd::fn_ref<double(int, double) noexcept> f2(std::cw<&minus_2>);
  f2 = std::move(f);
  auto ret1 = f(1, 2.0);
  auto ret2 = f2(1, 2.0);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_DOUBLE_EQ(ret1, 3.0);
    ASSERT_DOUBLE_EQ(ret2, 3.0);
  }
}

TEST(Conformance_fn_ref, move_assign_pass_3) {
  // const noexcept
  ebd::fn_ref<double(int, double) const noexcept> f(std::cw<&plus_2>);
  ebd::fn_ref<double(int, double) const noexcept> f2(std::cw<&minus_2>);
  f2 = std::move(f);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_DOUBLE_EQ(f(1, 2.0), 3.0);
    ASSERT_DOUBLE_EQ(f2(1, 2.0), 3.0);
  }
}

TEST(Conformance_fn_ref, move_assign_pass_4) {
  // with conversions
  ebd::fn_ref<Int(int, int, int)> f(std::cw<[](int, int, int) { return Int{1}; }>);
  ebd::fn_ref<Int(int, int, int)> f2(std::cw<NeedsConversion{}>);
  f = std::move(f2);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f(1, 2, 3).i == 6);
    ASSERT_(f2(1, 2, 3).i == 6);
  }

  ebd::fn_ref<Int(int, int, int) const> f_const(std::cw<[](int, int, int) { return Int{1}; }>);
  ebd::fn_ref<Int(int, int, int) const> f2_const(std::cw<NeedsConversion{}>);
  f_const = std::move(f2_const);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f_const(1, 2, 3).i == 6);
    ASSERT_(f2_const(1, 2, 3).i == 6);
  }

  ebd::fn_ref<Int(int, int, int) noexcept> f_noexcept(std::cw<[](int, int, int) noexcept { return Int{1}; }>);
  ebd::fn_ref<Int(int, int, int) noexcept> f2_noexcept(std::cw<NeedsConversion{}>);
  f_noexcept = std::move(f2_noexcept);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f_noexcept(1, 2, 3).i == 6);
    ASSERT_(f2_noexcept(1, 2, 3).i == 6);
  }

  ebd::fn_ref<Int(int, int, int) const noexcept> f_const_noexcept(
      std::cw<[](int, int, int) noexcept { return Int{1}; }>);
  ebd::fn_ref<Int(int, int, int) const noexcept> f2_const_noexcept(std::cw<NeedsConversion{}>);
  f_const_noexcept = std::move(f2_const_noexcept);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f_const_noexcept(1, 2, 3).i == 6);
    ASSERT_(f2_const_noexcept(1, 2, 3).i == 6);
  }
}

TEST(Conformance_fn_ref, move_assign_pass_5) {
  // with conversions function pointer
  ebd::fn_ref<Int(int, int, int)> f(std::cw<&zero>);
  ebd::fn_ref<Int(int, int, int)> f2(std::cw<&needs_conversion_4>);
  f = std::move(f2);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f(1, 2, 3).i == 6);
    ASSERT_(f2(1, 2, 3).i == 6);
  }

  ebd::fn_ref<Int(int, int, int) const> f_const(std::cw<&zero>);
  ebd::fn_ref<Int(int, int, int) const> f2_const(std::cw<&needs_conversion_4>);
  f_const = std::move(f2_const);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f_const(1, 2, 3).i == 6);
    ASSERT_(f2_const(1, 2, 3).i == 6);
  }

  ebd::fn_ref<Int(int, int, int) noexcept> f_noexcept(std::cw<&zero>);
  ebd::fn_ref<Int(int, int, int) noexcept> f2_noexcept(std::cw<&needs_conversion_4>);
  f_noexcept = std::move(f2_noexcept);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f_noexcept(1, 2, 3).i == 6);
    ASSERT_(f2_noexcept(1, 2, 3).i == 6);
  }

  ebd::fn_ref<Int(int, int, int) const noexcept> f_const_noexcept(std::cw<&zero>);
  ebd::fn_ref<Int(int, int, int) const noexcept> f2_const_noexcept(std::cw<&needs_conversion_4>);
  f_const_noexcept = std::move(f2_const_noexcept);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    ASSERT_(f_const_noexcept(1, 2, 3).i == 6);
    ASSERT_(f2_const_noexcept(1, 2, 3).i == 6);
  }
}

#endif
