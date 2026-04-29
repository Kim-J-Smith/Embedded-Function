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

// constexpr fn_ref(const fn_ref&) noexcept = default;

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

STATIC_ASSERT_(std::is_copy_constructible<ebd::fn_ref<void()>>::value);
STATIC_ASSERT_(std::is_copy_constructible<ebd::fn_ref<void() const>>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_copy_constructible<ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_copy_constructible<ebd::fn_ref<void() const noexcept>>::value);
#endif

static double f1(int x, double y) noexcept { return x + y; }

struct Int {
  int i;
  constexpr Int(int ii) noexcept : i(ii) {}
};

struct NeedsConversion {
  int operator()(Int x, Int y, Int z) const noexcept { return x.i + y.i + z.i; }
};

static int needs_conversion(Int x, Int y, Int z) noexcept { return x.i + y.i + z.i; }

TEST(Conformance_fn_ref, copy_pass) {
  static_cast<void>(&f1);
  static_cast<void>(&needs_conversion);

#if 0 && __cpp_lib_function_ref >= 202603L
  {
    ebd::fn_ref<void()> f(std::cw<[] {}>);
    auto f2 = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      f2();
    }
  }
  {
    const
    ebd::fn_ref<int() const> f(std::cw<[] { return 42; }>);
    auto f2 = f;

    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2() == 42);
    }
  }
  {
    // noexcept
    ebd::fn_ref<double(int, double) noexcept> f(std::cw<&f1>);
    auto f2 = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2(1, 2.0) == 3.0);
    }
  }
  {
    // const noexcept
    ebd::fn_ref<double(int, double) const noexcept> f(std::cw<&f1>);
    auto f2 = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2(1, 2.0) == 3.0);
    }
  }
  {
    // with conversions
    ebd::fn_ref<Int(int, int, int)> f(std::cw<NeedsConversion{}>);
    auto f_copy = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_copy(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f2(std::cw<NeedsConversion{}>);
    auto f2_copy = f2;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2_copy(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) noexcept> f3(std::cw<NeedsConversion{}>);
    auto f3_copy = f3;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f3_copy(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f4(std::cw<NeedsConversion{}>);
    auto f4_copy = f4;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f4_copy(1, 2, 3).i == 6);
    }
  }
  {
    // with conversions function pointer
    ebd::fn_ref<Int(int, int, int)> f(std::cw<&needs_conversion>);
    auto f_copy = f;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f_copy(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const> f2(std::cw<&needs_conversion>);
    auto f2_copy = f2;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f2_copy(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) noexcept> f3(std::cw<&needs_conversion>);
    auto f3_copy = f3;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f3_copy(1, 2, 3).i == 6);
    }

    ebd::fn_ref<Int(int, int, int) const noexcept> f4(std::cw<&needs_conversion>);
    auto f4_copy = f4;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      ASSERT_(f4_copy(1, 2, 3).i == 6);
    }
  }
#endif
  SUCCEED();
}
