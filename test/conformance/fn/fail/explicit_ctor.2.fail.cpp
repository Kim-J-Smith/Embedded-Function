#include "__constant_wrapper.hpp"
#include "test_function.hpp"

struct Normal {
    Normal() = default;
    int add_42(int n) const { return 42 + n; }
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    // FAIL: The Ctor is `explicit`.
    ebd::fn<int(int), sizeof(Normal)> f = {std::cw<&Normal::add_42>, std::in_place_type<Normal>};
    f(0);
#else
    static_assert(false, "");
#endif
}
