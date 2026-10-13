#include "test_function.hpp"

int main() {
#if (defined(__clang__) && __clang_major__ >= 7)
    // FAIL: dangling reference
    ebd::fn_ref<int()> f = []() { return 42; };
    (void)f();
#else
    static_assert(true, "");
#endif
}
