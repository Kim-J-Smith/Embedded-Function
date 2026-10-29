#include "test_function.hpp"

int main() {
#if ((defined(__clang__) && __clang_major__ >= 7) || (defined(_MSC_VER) && _MSC_VER >= 1937)) \
        && !defined(EBD_TEST_USE_FALLBACK)
    // FAIL: dangling reference
    ebd::fn_ref<int()> f = []() { return 42; };
    (void)f();
#else
    static_assert(false, "");
#endif
}
