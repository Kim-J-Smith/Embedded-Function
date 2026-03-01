#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include "test_function.hpp"

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106067
// This is not a bug of libstdc++. It is actually related to the 
// instantiation sequence of the template function.
// GCC 11.3 ~ 15.2 has this bug.
#if defined(EBD_TEST_TRY_BUG__GCC_106067)
template <void(*T)(ebd::fn<void()>)>
void caller() { T(ebd_test_free_func_v); }
#elif defined(__GNUC__) && !defined(__clang__) \
    && (__GNUC__ > 11 || (__GNUC__ == 11 && __GNUC_MINOR__ >= 3))
    // No Test.
#else
template <void(*T)(ebd::fn<void()>)>
void caller() { T(ebd_test_free_func_v); }
#endif
TEST(CompilerBugs, GCC_Bug_106067) {
    SUCCEED();
}

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51452
// https://en.cppreference.com/w/cpp/types/is_constructible.html#Notes
// GCC 5.1 ~ 15.2 has this "bug".
// Clang 3.3 ~ 20.2 has this "bug".
// MSVC v19.20 ~ v19.50 has this "bug".
// This "bug" will not be solved. See https://cplusplus.github.io/LWG/issue2116
#if 0
# define EBD_RES_EXPECT true
#else
# define EBD_RES_EXPECT false
#endif

TEST(CompilerBugs, GCC_Bug_51452) {
    auto CanConv = std::is_constructible<
        ebd::fn<int(int)>, ebd_test_operator_unambiguous&>::value;
    auto NoThrow = std::is_nothrow_constructible<
        ebd::fn<int(int)>, ebd_test_operator_unambiguous&>::value;

    EXPECT_EQ(CanConv == true, true);
    EXPECT_EQ(NoThrow == true, EBD_RES_EXPECT);
    SUCCEED();
}
