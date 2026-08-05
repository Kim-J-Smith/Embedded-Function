#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
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