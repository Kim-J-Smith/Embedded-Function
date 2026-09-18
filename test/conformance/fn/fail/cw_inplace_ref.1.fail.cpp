#include "__constant_wrapper.hpp"
#include "test_function.hpp"

struct LeftCallable {
    int add_42_left(int n) & noexcept { return n + 42; };
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    // FAIL: & -> &&
    ebd::fn<int(int) &&> f(std::cw<&LeftCallable::add_42_left>, LeftCallable{});
    std::move(f)(1);
#else
    static_assert(false, "");
#endif
}
