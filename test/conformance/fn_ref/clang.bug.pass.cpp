#include <functional>
#include <utility>
#include <type_traits>

#include "__constant_wrapper.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

/// @todo experimental @bug Clang 20 has a bug here.
/// In Clang 20, when a user creates two static free functions that have the
/// same name in two compile units and wraps them into two `std::cw<&free_fn>`
/// objects, a segmentation fault ( @e SIGSEGV ) occurs if one of the
/// `std::cw<&free_fn>` is called.

// if defined EBD_TEST_TRY_BUG__Clang_SameNameStaticFunction and use Clang20
// FAIL is expected:
// [ RUN      ] Conformance_fn_ref.copy_pass
// ninja: build stopped: subcommand failed.

// In copy.pass.cpp, there exists an identical static function 
// `this_name_should_not_be_changed` (with the same parameters and return type).
static double this_name_should_not_be_changed(int x, double y) noexcept { return x + y; }

TEST(Conformance_fn_ref, clang_bug_pass) {

  static_cast<void>(&this_name_should_not_be_changed);

#if __cpp_lib_constant_wrapper >= 202603L

# if defined(__clang__) && !defined(EBD_TEST_TRY_BUG__Clang_SameNameStaticFunction)
  ebd::fn_ref<double(int,double)> f = std::cw<+[](int x, double y) { return x + y; }>;
  ASSERT_DOUBLE_EQ(f(1, 2.0), 3.0);
# else
  ebd::fn_ref<double(int,double)> f = std::cw<&this_name_should_not_be_changed>;
  ASSERT_DOUBLE_EQ(f(1, 2.0), 3.0);
# endif

#endif

}

