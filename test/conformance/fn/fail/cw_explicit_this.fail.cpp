#include "test_function.hpp"

struct ExplicitThis {
    int add(this ExplicitThis&, int a, int b) {
        return a + b;
    }
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    ExplicitThis obj;
    // {std::cw<&Class::method>, &obj} is invalid if method is explicit this function.
    // See <https://wg21.link/P2511#Proposal:~:text=In%20the%20first,would%20stop%20working.>.
    auto f1 = ebd::make_fn(std::cw<&ExplicitThis::add>, &obj); // FAIL
    (void)f1;
#else
    static_assert(false, "");
#endif
}
