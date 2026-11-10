#include "test_function.hpp"

#include <type_traits>

namespace ed = ebd::detail;

struct DetailTraits_To_class {};
struct DetailTraits_From_class {
    operator DetailTraits_To_class() const { return {}; }
    operator DetailTraits_To_class&() const { static DetailTraits_To_class t; return t; }
};

TEST(DetailTraits, reference_converts_from_temporary_impl) {

#if defined(__GNUC__) && !defined(__clang__)
# define PASS_ true /// TODO: GCC BUG
#else
# define PASS_ false
#endif

    auto ret = ed::reference_converts_from_temporary<DetailTraits_To_class&&, DetailTraits_From_class>::value;
    auto can_convert = std::is_convertible<DetailTraits_From_class, DetailTraits_To_class&&>::value;
    ASSERT_EQ(ret <= can_convert || PASS_, true);
}


namespace {
    struct NonTrivial {
        int n;
        NonTrivial(int num = 0) noexcept : n(num) {}
        NonTrivial(const NonTrivial& o) noexcept { n = o.n; }
    };
}
TEST(DetailTraits, is_convertible_from_specialization) {
    // fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial&&)>, ebd::fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial)>, ebd::fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial&&)>, ebd::fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial)>, ebd::fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial&&) const>, ebd::fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial) const>, ebd::fn<int(NonTrivial&&) const>>::value, "BUG");

    // unique_fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial&&)>, ebd::unique_fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial)>, ebd::unique_fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial&&)>, ebd::unique_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial)>, ebd::unique_fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial&&) const>, ebd::unique_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial) const>, ebd::unique_fn<int(NonTrivial&&) const>>::value, "BUG");

    // classic_fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial&&)>, ebd::classic_fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial)>, ebd::classic_fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial&&)>, ebd::classic_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial)>, ebd::classic_fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial&&) const>, ebd::classic_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial) const>, ebd::classic_fn<int(NonTrivial&&) const>>::value, "BUG");

    // __safe_fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial&&)>, ebd::__safe_fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial)>, ebd::__safe_fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial&&)>, ebd::__safe_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial)>, ebd::__safe_fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial&&) const>, ebd::__safe_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial) const>, ebd::__safe_fn<int(NonTrivial&&) const>>::value, "BUG");

    // fn_ref
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial&&)>, ebd::fn_ref<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial)>, ebd::fn_ref<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial&&)>, ebd::fn_ref<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial)>, ebd::fn_ref<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial&&) const>, ebd::fn_ref<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial) const>, ebd::fn_ref<int(NonTrivial&&) const>>::value, "BUG");
}

TEST(DetailTraits, is_invocable_r) {
    static_assert(!ed::is_invocable_r<int[3], int*(*)()>::value, "BUG");
    static_assert(!ed::is_invocable_r<int[], int*(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int[3], int*(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int[], int*(*)()>::value, "BUG");

    static_assert(!ed::is_invocable_r<int[3], int*(&)()>::value, "BUG");
    static_assert(!ed::is_invocable_r<int[], int*(&)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int[3], int*(&)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int[], int*(&)()>::value, "BUG");

    using FP = void(*)();
    static_assert(!ed::is_invocable_r<void(), FP(*)()>::value, "BUG");
    static_assert(!ed::is_invocable_r<void(), FP(&)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<void(), FP(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<void(), FP(&)()>::value, "BUG");

    static_assert(ed::is_invocable_r<int, int(*)()>::value, "BUG");
    static_assert(ed::is_invocable_r<int const, int(*)()>::value, "BUG");
    static_assert(ed::is_invocable_r<int volatile, int(*)()>::value, "BUG");
    static_assert(ed::is_invocable_r<int const volatile, int(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int, int(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int const, int(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int volatile, int(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<int const volatile, int(*)()>::value, "BUG");

    auto ret_int = []() noexcept -> int { return 0; };
    static_assert(ed::is_invocable_r<int, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_invocable_r<int const, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_invocable_r<int volatile, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_invocable_r<int const volatile, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_nothrow_invocable_r<int, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_nothrow_invocable_r<int const, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_nothrow_invocable_r<int volatile, decltype(ret_int)>::value, "BUG");
    static_assert(ed::is_nothrow_invocable_r<int const volatile, decltype(ret_int)>::value, "BUG");

    static_assert(ed::is_invocable_r<void(*)(), FP(*)()>::value, "BUG");
    static_assert(ed::is_invocable_r<void(*)(), FP(&)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<void(*)(), FP(*)()>::value, "BUG");
    static_assert(!ed::is_nothrow_invocable_r<void(*)(), FP(&)()>::value, "BUG");

    auto ret_fp = []() noexcept -> FP { return nullptr; };
    static_assert(ed::is_invocable_r<void(*)(), decltype(ret_fp)>::value, "BUG");
    static_assert(ed::is_invocable_r<void(*)(), decltype(ret_fp)>::value, "BUG");
    static_assert(ed::is_nothrow_invocable_r<void(*)(), decltype(ret_fp)>::value, "BUG");
    static_assert(ed::is_nothrow_invocable_r<void(*)(), decltype(ret_fp)>::value, "BUG");
}

namespace {
    struct StatefuleOp { int m; };
    int StatefuleOp_flag = 0;
}

namespace std {
    template <> // technically invalid
    struct less<StatefuleOp> {
        bool last_res = false;
        bool operator()(const StatefuleOp& a, const StatefuleOp& b) noexcept {
            last_res = last_res ? last_res : a.m < b.m;
            return last_res;
        }
    };

    template <> // technically invalid
    struct plus<StatefuleOp> {
        plus() noexcept { StatefuleOp_flag = 42; }
        StatefuleOp operator()(const StatefuleOp& a, const StatefuleOp& b) const noexcept {
            return StatefuleOp{a.m + b.m};
        }
    };
}
TEST(DetailTraits, is_standard_stateless_function_object) {
    // less
    static_assert(ed::is_standard_stateless_function_object<std::less<int>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<const int>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<volatile int>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<const volatile int>>::value, "BUG");

    static_assert(ed::is_standard_stateless_function_object<std::less<float>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<const float>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<volatile float>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<const volatile float>>::value, "BUG");

    static_assert(ed::is_standard_stateless_function_object<std::less<char>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<const char>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<volatile char>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::less<const volatile char>>::value, "BUG");

    static_assert(!ed::is_standard_stateless_function_object<std::less<StatefuleOp>>::value, "BUG");

    {
        ebd::fn<bool(const StatefuleOp& a, const StatefuleOp& b)> f = std::less<StatefuleOp>{};
        ASSERT_FALSE(f(StatefuleOp{2}, StatefuleOp{1}));
        ASSERT_TRUE(f(StatefuleOp{1}, StatefuleOp{2}));
        ASSERT_TRUE(f(StatefuleOp{2}, StatefuleOp{1}));
    }
    {
        ebd::unique_fn<bool(const StatefuleOp& a, const StatefuleOp& b)> f = std::less<StatefuleOp>{};
        ASSERT_FALSE(f(StatefuleOp{2}, StatefuleOp{1}));
        ASSERT_TRUE(f(StatefuleOp{1}, StatefuleOp{2}));
        ASSERT_TRUE(f(StatefuleOp{2}, StatefuleOp{1}));
    }
    {
        ebd::classic_fn<bool(const StatefuleOp& a, const StatefuleOp& b)> f = std::less<StatefuleOp>{};
        ASSERT_FALSE(f(StatefuleOp{2}, StatefuleOp{1}));
        ASSERT_TRUE(f(StatefuleOp{1}, StatefuleOp{2}));
        ASSERT_TRUE(f(StatefuleOp{2}, StatefuleOp{1}));
    }
    {
        ebd::__safe_fn<bool(const StatefuleOp& a, const StatefuleOp& b)> f = std::less<StatefuleOp>{};
        ASSERT_FALSE(f(StatefuleOp{2}, StatefuleOp{1}));
        ASSERT_TRUE(f(StatefuleOp{1}, StatefuleOp{2}));
        ASSERT_TRUE(f(StatefuleOp{2}, StatefuleOp{1}));
    }
    {
        std::less<StatefuleOp> obj{};
        ebd::fn_ref<bool(const StatefuleOp& a, const StatefuleOp& b)> f = obj;
        ASSERT_FALSE(f(StatefuleOp{2}, StatefuleOp{1}));
        ASSERT_TRUE(f(StatefuleOp{1}, StatefuleOp{2}));
        ASSERT_TRUE(f(StatefuleOp{2}, StatefuleOp{1}));
    }

    // plus
    static_assert(ed::is_standard_stateless_function_object<std::plus<int>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<const int>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<volatile int>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<const volatile int>>::value, "BUG");

    static_assert(ed::is_standard_stateless_function_object<std::plus<float>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<const float>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<volatile float>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<const volatile float>>::value, "BUG");

    static_assert(ed::is_standard_stateless_function_object<std::plus<char>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<const char>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<volatile char>>::value, "BUG");
    static_assert(ed::is_standard_stateless_function_object<std::plus<const volatile char>>::value, "BUG");

    static_assert(!ed::is_standard_stateless_function_object<std::plus<StatefuleOp>>::value, "BUG");

    {
        StatefuleOp_flag = 0;
        ebd::fn<StatefuleOp(const StatefuleOp& a, const StatefuleOp& b)> f = std::plus<StatefuleOp>{};
        ASSERT_EQ(StatefuleOp_flag,  42);
        StatefuleOp_flag = 0;
        ASSERT_EQ(f(StatefuleOp{2}, StatefuleOp{1}).m, 3);
        ASSERT_EQ(StatefuleOp_flag,  0);
    }
    {
        StatefuleOp_flag = 0;
        ebd::unique_fn<StatefuleOp(const StatefuleOp& a, const StatefuleOp& b)> f = std::plus<StatefuleOp>{};
        ASSERT_EQ(StatefuleOp_flag,  42);
        StatefuleOp_flag = 0;
        ASSERT_EQ(f(StatefuleOp{2}, StatefuleOp{1}).m, 3);
        ASSERT_EQ(StatefuleOp_flag,  0);
    }
    {
        StatefuleOp_flag = 0;
        ebd::classic_fn<StatefuleOp(const StatefuleOp& a, const StatefuleOp& b)> f = std::plus<StatefuleOp>{};
        ASSERT_EQ(StatefuleOp_flag,  42);
        StatefuleOp_flag = 0;
        ASSERT_EQ(f(StatefuleOp{2}, StatefuleOp{1}).m, 3);
        ASSERT_EQ(StatefuleOp_flag,  0);
    }
    {
        StatefuleOp_flag = 0;
        ebd::__safe_fn<StatefuleOp(const StatefuleOp& a, const StatefuleOp& b)> f = std::plus<StatefuleOp>{};
        ASSERT_EQ(StatefuleOp_flag,  42);
        StatefuleOp_flag = 0;
        ASSERT_EQ(f(StatefuleOp{2}, StatefuleOp{1}).m, 3);
        ASSERT_EQ(StatefuleOp_flag,  0);
    }
    {
        StatefuleOp_flag = 0;
        std::plus<StatefuleOp> obj{};
        ebd::fn_ref<StatefuleOp(const StatefuleOp& a, const StatefuleOp& b)> f = obj;
        ASSERT_EQ(StatefuleOp_flag,  42);
        StatefuleOp_flag = 0;
        ASSERT_EQ(f(StatefuleOp{2}, StatefuleOp{1}).m, 3);
        ASSERT_EQ(StatefuleOp_flag,  0);
    }
}
