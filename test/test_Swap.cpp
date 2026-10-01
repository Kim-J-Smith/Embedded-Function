#include "test_function.hpp"

TEST(TestSwap, fn_swap) {
    auto f1 = ebd::make_fn(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, fn_ref_swap) {
    auto f1 = ebd::fn_ref<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2 = ebd_test_free_func_iii_add;

    f2.swap(f1);

    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, unique_fn_swap) {
    auto f1 = ebd::unique_fn<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, classic_fn_swap) {
    auto f1 = ebd::classic_fn<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}

TEST(TestSwap, __safe_fn_swap) {
    auto f1 = ebd::__safe_fn<int(int, int)>(ebd_test_free_func_iii_add);
    decltype(f1) f2;

    f2.swap(f1);

    ASSERT_EQ(f1 == nullptr, true);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f2(3, 4), 3 + 4);
}

static int TestSwap_NonTrivialSwap_Flag = 0;

struct TestSwap_NonTrivialSwap_Functor {
    int m_var;
    TestSwap_NonTrivialSwap_Functor() noexcept : m_var(0) {}
    TestSwap_NonTrivialSwap_Functor(const TestSwap_NonTrivialSwap_Functor& other) noexcept {
        TestSwap_NonTrivialSwap_Flag++;
        m_var = other.m_var;
    }
    int operator()() const { return m_var; }
};

TEST(TestSwap, NonTrivialSwap) {
    using Test_t = TestSwap_NonTrivialSwap_Functor;
    {
        Test_t t{};
        t.m_var = 42;
        ebd::fn<int() const> f1 = t;
        t.m_var = 43;
        ebd::fn<int() const> f2 = t;

        TestSwap_NonTrivialSwap_Flag = 0;
        f1.swap(f2);

        ASSERT_EQ(TestSwap_NonTrivialSwap_Flag, 3);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
    {
        Test_t t{};
        t.m_var = 42;
        ebd::unique_fn<int() const> f1 = t;
        t.m_var = 43;
        ebd::unique_fn<int() const> f2 = t;

        TestSwap_NonTrivialSwap_Flag = 0;
        f1.swap(f2);

        ASSERT_EQ(TestSwap_NonTrivialSwap_Flag, 3);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
    {
        Test_t t{};
        t.m_var = 42;
        ebd::classic_fn<int() const> f1 = t;
        t.m_var = 43;
        ebd::classic_fn<int() const> f2 = t;

        TestSwap_NonTrivialSwap_Flag = 0;
        f1.swap(f2);

        ASSERT_EQ(TestSwap_NonTrivialSwap_Flag, 3);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
}

TEST(TestSwap, ADL_Swap) {
    {
        auto f1 = ebd::make_fn<ebd::fn>(+[] { return 42; });
        auto f2 = ebd::make_fn<ebd::fn>(+[] { return 43; });
        swap(f1, f2);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
    {
        auto f1 = ebd::make_fn<ebd::unique_fn>(+[] { return 42; });
        auto f2 = ebd::make_fn<ebd::unique_fn>(+[] { return 43; });
        swap(f1, f2);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
    {
        auto f1 = ebd::make_fn<ebd::__safe_fn>(+[] { return 42; });
        auto f2 = ebd::make_fn<ebd::__safe_fn>(+[] { return 43; });
        swap(f1, f2);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
    {
        auto f1 = ebd::make_fn<ebd::classic_fn>(+[] { return 42; });
        auto f2 = ebd::make_fn<ebd::classic_fn>(+[] { return 43; });
        swap(f1, f2);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
    {
        auto f1 = ebd::make_fn<ebd::fn_ref>(+[] { return 42; });
        auto f2 = ebd::make_fn<ebd::fn_ref>(+[] { return 43; });
        swap(f1, f2);
        ASSERT_EQ(f1(), 43);
        ASSERT_EQ(f2(), 42);
    }
}

#if EMBED_CXX_ENABLE_EXCEPTION && EMBED_CXX_VERSION >= 201703L
namespace {
int count = 0;

struct ThrowInCopy {
    ThrowInCopy() = default;
    ThrowInCopy(const ThrowInCopy&) noexcept(false) { throw 7; }
    ThrowInCopy(ThrowInCopy&&) noexcept(false) { throw 8; }
    int operator()(int) const { return 0; }
};

struct CountInDestroy {
    CountInDestroy() = default;
    ~CountInDestroy() { count++; }
    int operator()(int) const { return 0; }
};
}

TEST(TestSwap, ThrowInSwap) {
    ebd::fn<int(int)> f1(std::in_place_type<ThrowInCopy>);
    ebd::fn<int(int)> f2(std::in_place_type<CountInDestroy>);
    int c = 0;
    count = 0;
    try {
        f2.swap(f1);
    } catch (...) {
        c = count;
    }

    f2.clear();
    ASSERT_EQ(count, c);
}

#endif // EMBED_CXX_ENABLE_EXCEPTION && EMBED_CXX_VERSION >= 201703L

namespace {
int free_count = 0;
struct CountInFree {
    ~CountInFree() { free_count++; }
    int operator()() const noexcept { return 42; }
};
}

TEST(TestSwap, NonDoubleFree) {
    ebd::fn<int()> f1 = CountInFree{};
    ebd::fn<int()> f2 = CountInFree{};

    free_count = 0;
    f1.swap(f2);
    ASSERT_EQ(free_count, 3);
}

