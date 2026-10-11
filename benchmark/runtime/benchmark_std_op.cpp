#include "benchmark.hpp"

#include <functional>

static void std_op_wrapper_fn_std(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto w_less = benchmark_opaque_noninline<std::function<bool(int, int)>>(less);
    auto w_greater = benchmark_opaque_noninline<std::function<bool(int, int)>>(greater);

    auto f = [](std::function<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(w_less);
        volatile bool res2 = f(w_greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_ebd(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto w_less = benchmark_opaque_noninline<ebd::fn<bool(int, int)>>(less);
    auto w_greater = benchmark_opaque_noninline<ebd::fn<bool(int, int)>>(greater);

    auto f = [](ebd::fn<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(w_less);
        volatile bool res2 = f(w_greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_fu2(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto w_less = benchmark_opaque_noninline<fu2::function<bool(int, int)>>(less);
    auto w_greater = benchmark_opaque_noninline<fu2::function<bool(int, int)>>(greater);

    auto f = [](fu2::function<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(w_less);
        volatile bool res2 = f(w_greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_ref_ebd(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto w_less = benchmark_opaque_noninline<ebd::fn_ref<bool(int, int)>>(less);
    auto w_greater = benchmark_opaque_noninline<ebd::fn_ref<bool(int, int)>>(greater);

    auto f = [](ebd::fn_ref<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(w_less);
        volatile bool res2 = f(w_greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_view_fu2(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto w_less = benchmark_opaque_noninline<fu2::function_view<bool(int, int)>>(less);
    auto w_greater = benchmark_opaque_noninline<fu2::function_view<bool(int, int)>>(greater);

    auto f = [](fu2::function_view<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(w_less);
        volatile bool res2 = f(w_greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_pro(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, bool(int, int)>
        ::restrict_layout<ebd::fn<int()>::get_buffer_size()>
        ::support_copy<pro::constraint_level::nontrivial>
        ::build;

    auto w_less = benchmark_opaque_noninline(pro::make_proxy_inplace<Invoker>(less));
    auto w_greater = benchmark_opaque_noninline(pro::make_proxy_inplace<Invoker>(greater));

    auto f = [](pro::proxy<Invoker> f) { return (*f)(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(w_less);
        volatile bool res2 = f(w_greater);
        (void)res1; (void)res2;
    }
}

BENCHMARK_UNIT(StdOperatorWrapper.FunctionWrapperAsParams);
BENCHMARK_BASELINE(std_op_wrapper_fn_std);
BENCHMARK_NOTBASE(std_op_wrapper_fn_fu2);
BENCHMARK_NOTBASE(std_op_wrapper_fn_ebd);
BENCHMARK_NOTBASE(std_op_wrapper_fn_view_fu2);
BENCHMARK_NOTBASE(std_op_wrapper_fn_ref_ebd);
BENCHMARK_NOTBASE(std_op_wrapper_fn_pro);
