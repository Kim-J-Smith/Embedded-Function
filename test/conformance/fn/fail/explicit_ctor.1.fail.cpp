#include "__constant_wrapper.hpp"
#include "test_function.hpp"

struct ListInit {
    ListInit() = delete;
    ListInit(std::initializer_list<int>& il) : buf(il) {}
    std::vector<int> buf;
    int operator()() {
        int result = 0;
        for (auto& i : buf) { result += i; }
        return result;
    }
    int sum() { return this->operator()(); }
};

int main() {
#if __cpp_lib_constant_wrapper >= 202603L
    // FAIL: The Ctor is `explicit`.
    ebd::fn<int(), sizeof(ListInit)> f = {std::cw<&ListInit::sum>, std::in_place_type<ListInit>, {1, 42}};
    f();
#else
    static_assert(false, "");
#endif
}
