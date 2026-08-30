#include "test_function.hpp"

struct ExplicitThis {
    int add(this ExplicitThis&, int a, int b) {
        return a + b;
    }
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    ExplicitThis obj;
    auto f1 = ebd::make_fn(std::cw<&ExplicitThis::add>, &obj);
    (void)f1;
#else
    static_assert(false, "");
#endif
}
