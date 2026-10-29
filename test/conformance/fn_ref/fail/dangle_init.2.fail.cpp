#include "test_function.hpp"

int main() {
#if ((defined(__clang__) && __clang_major__ >= 7) || (defined(_MSC_VER) && _MSC_VER >= 1937)) \
        && !defined(EBD_TEST_USE_FALLBACK)
    // FAIL: dangling reference
    ebd::fn_ref<bool(int, int)> f = std::less<int>{};
    (void)f(1, 2);
#else
    static_assert(false, "");
#endif
}
