#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

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
    ASSERT_EQ(a, b);
}
