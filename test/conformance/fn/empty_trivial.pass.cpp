#include "test_function.hpp"

#include <cstdint>

namespace {
struct F {
    uintptr_t operator()() {
        return reinterpret_cast<uintptr_t>(this);
    }
};
uintptr_t func(ebd::fn<uintptr_t()>& f) {
    volatile int u = 42;
    auto* volatile d = &u;
    (void)d;
    return f();
}
}

TEST(Conformance_fn, empty_trivial_pass) {
    ebd::fn<uintptr_t()> f = F{};
    volatile uintptr_t a = f();
    volatile uintptr_t b = func(f);
#ifdef EMBED_FN_CONFIG_EMPTY_TRIVIAL_STATEFUL
    ASSERT_EQ(a, b);
#else
    ASSERT_NE(a, b);
#endif
}