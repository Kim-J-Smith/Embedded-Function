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

// Constraints:
// - is-convertible-from-specialization<T> is false,
// - is_pointer_v<T> is false, and
// - T is not a specialization of constant_wrapper. (Unused for `ebd::fn_ref`)

#include <functional>
#include <utility>
#include <type_traits>

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

#define STATIC_ASSERT_(...) static_assert((__VA_ARGS__), "BUG")
#define ASSERT_(...) ASSERT_EQ((__VA_ARGS__) == true, true)

// non const noexcept(false)
STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void()>, ebd::fn_ref<void()>>::value);
STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void()>, ebd::fn_ref<void() const>>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void()>, ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void()>, ebd::fn_ref<void() const noexcept>>::value);
#endif

STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void()>, void (*)()>::value);
STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void()>, void (*)(int)>::value);

// STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void()>, std::constant_wrapper<[] {}>>::value);
// STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void()>, std::constant_wrapper<[](int) {}>>::value);

// const noexcept(false)
STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const>, ebd::fn_ref<void()>>::value);
STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const>, ebd::fn_ref<void() const>>::value);
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const>, ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const>, ebd::fn_ref<void() const noexcept>>::value);
#endif

STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const>, void (*)()>::value);
STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() const>, void (*)(int)>::value);

// STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const>, std::constant_wrapper<[] { return 42; }>>::value);
// STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() const>, std::constant_wrapper<[](int) { return 42; }>>::value);

// non-const noexcept(true)
#if __cpp_noexcept_function_type >= 201510L
  STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() noexcept>, ebd::fn_ref<void()>>::value);
  STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() noexcept>, ebd::fn_ref<void() const>>::value);
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() noexcept>, ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() noexcept>, ebd::fn_ref<void() const noexcept>>::value);

  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() noexcept>, void (*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() noexcept>, void (*)(int) noexcept>::value);

  // STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() noexcept>, std::constant_wrapper<[] noexcept {} >>);
  // STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() noexcept>, std::constant_wrapper<[](int) noexcept {}>>);

  // const noexcept(true)
  STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() const noexcept>, ebd::fn_ref<void()>>::value);
  STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() const noexcept>, ebd::fn_ref<void() const>>::value);
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const noexcept>, ebd::fn_ref<void() noexcept>>::value);
  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const noexcept>, ebd::fn_ref<void() const noexcept>>::value);

  STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const noexcept>, void (*)() noexcept>::value);
  STATIC_ASSERT_(!std::is_assignable<ebd::fn_ref<void() const noexcept>, void (*)(int) noexcept>::value);

  // STATIC_ASSERT_(std::is_assignable<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<[] noexcept {}>>::value);
  // STATIC_ASSERT_(
  //     !std::is_assignable<ebd::fn_ref<void() const noexcept>, std::constant_wrapper<[](int) noexcept {}>>::value);
#endif

static int forty_two() { return 42; }

TEST(Conformance_fn_ref, assign_delete_pass) {
  static_cast<void>(&forty_two);

#if 0 && __cpp_lib_function_ref >= 202603L
  {
    ebd::fn_ref<int()> f(std::cw<[] { return 41; }>);
    f = ebd::fn_ref<int()>(std::cw<[] { return 42; }>);
    ASSERT_(f() == 42);
  }
  {
    ebd::fn_ref<int() > f(std::cw<[] { return 41; }>);
    f = &forty_two;
    ASSERT_(f() == 42);
  }
  {
    ebd::fn_ref<int() > f(std::cw<[] { return 41; }>);
    f = std::cw<[] { return 42; }>;
    ASSERT_(f() == 42);
  }
  {
    // const
    ebd::fn_ref<int() const> f(std::cw<[] { return 41; }>);
    f = std::cw<[] { return 42; }>;
    ASSERT_(f() == 42);
  }
  {
    // noexcept
    ebd::fn_ref<int() noexcept> f(std::cw<[] noexcept { return 41; }>);
    f = std::cw<[] noexcept { return 42; }>;
    ASSERT_(f() == 42);
  }
  {
    // const noexcept
    ebd::fn_ref<int() const noexcept> f(std::cw<[] noexcept { return 41; }>);
    f = std::cw<[] noexcept { return 42; }>;
    ASSERT_(f() == 42);
  }
#endif
  SUCCEED();
}
