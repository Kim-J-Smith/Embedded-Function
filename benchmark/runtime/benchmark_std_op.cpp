#include "benchmark.hpp"

#include <functional>

static void std_op_wrapper_fn_std(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto f = [](std::function<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(less);
        volatile bool res2 = f(greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_ebd(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto f = [](ebd::fn<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(less);
        volatile bool res2 = f(greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_fu2(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto f = [](fu2::function<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(less);
        volatile bool res2 = f(greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_ref_ebd(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto f = [](ebd::fn_ref<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(less);
        volatile bool res2 = f(greater);
        (void)res1; (void)res2;
    }
}

static void std_op_wrapper_fn_view_fu2(picobench::state& s) {
    auto less = std::less<int>{};
    auto greater = std::greater<int>{};

    auto f = [](fu2::function_view<bool(int, int)> f) { return f(0x111, 0x222); };

    for (auto _ : s) {
        volatile bool res1 = f(less);
        volatile bool res2 = f(greater);
        (void)res1; (void)res2;
    }
}

BENCHMARK_UNIT(StdOperatorWrapper.FunctionWrapperAsParams);
BENCHMARK_BASELINE(std_op_wrapper_fn_std);
BENCHMARK_NOTBASE(std_op_wrapper_fn_fu2);
BENCHMARK_NOTBASE(std_op_wrapper_fn_ebd);
BENCHMARK_NOTBASE(std_op_wrapper_fn_view_fu2);
BENCHMARK_NOTBASE(std_op_wrapper_fn_ref_ebd);
