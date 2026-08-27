#include "../__constant_wrapper.hpp"
#include "test_function.hpp"

struct AmbiguousWithConstantWrapper {
    /*[constexpr]*/ int operator()(std::integral_constant<int, 10>) const { return 10; }
    constexpr int operator()(int) const { return 42; }
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    // ambiguous
    ebd::fn_ref<int(std::integral_constant<int, 10>)> f =
        std::cw<AmbiguousWithConstantWrapper{}>; // FAIL
#else
    static_assert(false, "");
#endif
}
