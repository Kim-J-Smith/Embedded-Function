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
    constexpr int DetailTraits_invoke_free_fn(int n) noexcept { return n + 1; }
    int DetailTraits_invoke_free_fn_throw(int n) { return n + 2; }

    struct DetailTraits_invoke_target {
        int value;

        constexpr explicit DetailTraits_invoke_target(int v) noexcept : value(v) {}
        constexpr int add(int n) const noexcept { return value + n; }
        int& ref() noexcept { return value; }
        void set(int n) noexcept { value = n; }
    };

    struct DetailTraits_invoke_functor {
        int factor;
        constexpr int operator()(int n) const noexcept { return factor * n; }
    };

    template <typename Func, typename... Args>
    struct DetailTraits_can_invoke {
        template <typename F, typename... A>
        static auto test(int) -> decltype(
            ed::invoke(std::declval<F>(), std::declval<A>()...), std::true_type{});
        template <typename...>
        static std::false_type test(long);

        static const bool value = decltype(test<Func, Args...>(0))::value;
    };
} // namespace

TEST(DetailTraits, invoke) {
    // Callable objects.
    static_assert(
        std::is_same<decltype(ed::invoke(DetailTraits_invoke_free_fn, 0)), int>::value, "BUG");
    ASSERT_EQ(ed::invoke(DetailTraits_invoke_free_fn, 1), 2);
    ASSERT_EQ(ed::invoke(DetailTraits_invoke_free_fn_throw, 1), 3);

    DetailTraits_invoke_functor functor{2};
    ASSERT_EQ(ed::invoke(functor, 21), 42);
    ASSERT_EQ(ed::invoke(DetailTraits_invoke_functor{3}, 4), 12);

    // Pointer to member function.
    DetailTraits_invoke_target target(10);
    const DetailTraits_invoke_target ctarget(20);
    ASSERT_EQ(ed::invoke(&DetailTraits_invoke_target::add, target, 1), 11);
    ASSERT_EQ(ed::invoke(&DetailTraits_invoke_target::add, &target, 2), 12);
    ASSERT_EQ(ed::invoke(&DetailTraits_invoke_target::add, std::ref(target), 3), 13);
    ASSERT_EQ(ed::invoke(&DetailTraits_invoke_target::add, ctarget, 4), 24);

    int& ref_result = ed::invoke(&DetailTraits_invoke_target::ref, target);
    ref_result = 31;
    ASSERT_EQ(target.value, 31);

    // Pointer to member object.
    int DetailTraits_invoke_target::* pm = &DetailTraits_invoke_target::value;
    static_assert(
        std::is_same<decltype(ed::invoke(pm, target)), int&>::value, "BUG");
    ASSERT_EQ(ed::invoke(pm, target), 31);
    ASSERT_EQ(ed::invoke(pm, &target), 31);
    ASSERT_EQ(ed::invoke(pm, std::ref(target)), 31);

    ed::invoke(pm, target) = 7;
    ASSERT_EQ(target.value, 7);

    // Pointer-like object argument.
    std::unique_ptr<DetailTraits_invoke_target> ptr(new DetailTraits_invoke_target(30));
    ASSERT_EQ(ed::invoke(&DetailTraits_invoke_target::add, ptr, 1), 31);
    ASSERT_EQ(ed::invoke(pm, ptr), 30);

    // `void` return type.
    static_assert(
        std::is_same<
            decltype(ed::invoke(&DetailTraits_invoke_target::set, target, 0)),
            void
        >::value, "BUG");
    ed::invoke(&DetailTraits_invoke_target::set, target, 8);
    ASSERT_EQ(target.value, 8);

    // SFINAE friendliness.
    static_assert(!DetailTraits_can_invoke<int, int>::value, "BUG");
    static_assert(
        DetailTraits_can_invoke<
            decltype(&DetailTraits_invoke_target::add),
            DetailTraits_invoke_target&, int
        >::value, "BUG");
    static_assert(
        !DetailTraits_can_invoke<
            decltype(&DetailTraits_invoke_target::add), int, int
        >::value, "BUG");

#if (EMBED_CXX_VERSION >= 201703L && \
    defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L)
    static_assert(noexcept(ed::invoke(DetailTraits_invoke_free_fn, 0)), "BUG");
    static_assert(!noexcept(ed::invoke(DetailTraits_invoke_free_fn_throw, 0)), "BUG");
    static_assert(
        noexcept(ed::invoke(&DetailTraits_invoke_target::add, target, 0)), "BUG");
    static_assert(noexcept(ed::invoke(pm, target)), "BUG");
#endif

    // Constant evaluation.
#if EMBED_CXX_VERSION >= 201402L
    static_assert(ed::invoke(DetailTraits_invoke_free_fn, 41) == 42, "BUG");

    constexpr DetailTraits_invoke_target cxtarget(5);
    static_assert(
        ed::invoke(&DetailTraits_invoke_target::add, cxtarget, 1) == 6, "BUG");

    constexpr int DetailTraits_invoke_target::* cx_pm =
        &DetailTraits_invoke_target::value;
    static_assert(ed::invoke(cx_pm, cxtarget) == 5, "BUG");
#endif
}

namespace {
    struct StatefuleOp { int m; };
    int StatefuleOp_flag = 0;
}

namespace std {
    template <> // gray areas in the C++ standard
    struct less<StatefuleOp> {
        bool last_res = false;
        bool operator()(const StatefuleOp& a, const StatefuleOp& b) noexcept {
            last_res = last_res ? last_res : a.m < b.m;
            return last_res;
        }
    };

    template <> // gray areas in the C++ standard
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

// See <https://github.com/Kim-J-Smith/Embedded-Function/issues/287>.
namespace {
    template <class T> class Issue287 {};
    template <std::size_t N> class Issue287Pad {};
    template <std::size_t N>
    using signature = void(Issue287<Issue287Pad<N>>);
}
TEST(DetailTraits, is_complete_here) {
    using ed::is_complete_here;
    using ed::each_param_is_complete_here;

    static_assert(is_complete_here<Issue287<Issue287Pad<0>>>(), "BUG");
    static_assert(ed::is_complete_here<Issue287<Issue287Pad<1>>>(), "BUG");

    static_assert(each_param_is_complete_here(typename ed::unwrap_signature<signature<3>>::args{}), "BUG");
    static_assert(ed::each_param_is_complete_here(typename ed::unwrap_signature<signature<4>>::args{}), "BUG");

    static_assert(is_complete_here<int>(), "BUG");
    static_assert(is_complete_here<long>(), "BUG");
    static_assert(is_complete_here<long long>(), "BUG");
    static_assert(is_complete_here<short>(), "BUG");
    static_assert(is_complete_here<float>(), "BUG");
    static_assert(is_complete_here<double>(), "BUG");

    class Undefined;
    static_assert(is_complete_here<void*>(), "BUG");
    static_assert(is_complete_here<void()>(), "BUG");
    static_assert(is_complete_here<Undefined&>(), "BUG");
}
