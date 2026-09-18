#include "__constant_wrapper.hpp"
#include "test_function.hpp"

struct RightCallable {
    int add_42_right(int n) && noexcept { return n + 42; };
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    // FAIL: && -> &
    ebd::fn<int(int)&> f(std::cw<&RightCallable::add_42_right>, RightCallable{});
    f(1);
#else
    static_assert(false, "");
#endif
}
