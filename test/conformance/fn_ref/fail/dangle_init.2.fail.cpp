#include "test_function.hpp"

int main() {
#if (defined(__clang__) && __clang_major__ >= 7)
    // FAIL: dangling reference
    ebd::fn_ref<bool(int, int)> f = std::less<int>{};
    (void)f(1, 2);
#else
    static_assert(false, "");
#endif
}
