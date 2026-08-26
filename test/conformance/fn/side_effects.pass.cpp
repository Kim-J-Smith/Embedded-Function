#include "test_function.hpp"

namespace {
    static int copies = 0;

    struct NonTriviallyCopyable {
        NonTriviallyCopyable() = default;
        NonTriviallyCopyable(const NonTriviallyCopyable&) noexcept { copies++; }
        int operator()(int) { return 42; }
#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)
        static int operator()(long) { return 43; }
#endif
    };
}

// See <https://lists.isocpp.org/sg14/2026/08/1346.php>.
TEST(Conformance_fn, side_effects_pass) {
    int number = 1;
    ebd::fn<int()> f1 = [number]() mutable { return number++; };
    ASSERT_EQ(f1(), 1);
    ASSERT_EQ(f1(), 2);
    ASSERT_EQ(f1(), 3);

    ebd::fn<int(int)> f2 = NonTriviallyCopyable{};
    copies = 0;
    {
        auto f_ = f2;
    }
    ASSERT_EQ(copies, 1);
    ASSERT_EQ(f2(0), 42);
    ASSERT_EQ(f2(0L), 42);

#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)
    ebd::fn<int(long)> f3 = NonTriviallyCopyable{};
    copies = 0;
    {
        auto f_ = f3;
    }
    ASSERT_EQ(copies, 1);
    ASSERT_EQ(f3(0), 43);
    ASSERT_EQ(f3(0L), 43);
#endif
}
