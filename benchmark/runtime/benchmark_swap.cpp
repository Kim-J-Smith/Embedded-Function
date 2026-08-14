#include "benchmark.hpp"

#include <functional>

static void use_std_swap_trivial(picobench::state& s) {
    ebd::fn<int(int)> f1 = +[](int n) { return 42 + n; };
    ebd::fn<int(int)> f2 = +[](int n) { return 43 + n; };

    for (auto _ : s) {
        std::swap(f1, f2);
    }
    volatile int ret = f1(314); (void)ret;
}

static void use_adl_swap_trivial(picobench::state& s) {
    ebd::fn<int(int)> f1 = +[](int n) { return 42 + n; };
    ebd::fn<int(int)> f2 = +[](int n) { return 43 + n; };

    using std::swap; // no use

    for (auto _ : s) {
        swap(f1, f2);
    }
    volatile int ret = f1(314); (void)ret;
}

BENCHMARK_UNIT(ADLSwapBenchmark.fn_swap_trivial);
BENCHMARK_BASELINE(use_std_swap_trivial);
BENCHMARK_NOTBASE(use_adl_swap_trivial);

static void use_std_swap_non_trivial(picobench::state& s) {
    ebd::fn<int(int)> f1 = benchmark_non_trivial_struct_add42{};
    ebd::fn<int(int)> f2 = benchmark_non_trivial_struct_add43{};

    for (auto _ : s) {
        std::swap(f1, f2);
    }
    volatile int ret = f1(314); (void)ret;
}

static void use_adl_swap_non_trivial(picobench::state& s) {
    ebd::fn<int(int)> f1 = benchmark_non_trivial_struct_add42{};
    ebd::fn<int(int)> f2 = benchmark_non_trivial_struct_add43{};

    using std::swap; // no use

    for (auto _ : s) {
        swap(f1, f2);
    }
    volatile int ret = f1(314); (void)ret;
}

BENCHMARK_UNIT(ADLSwapBenchmark.fn_swap_non_trivial);
BENCHMARK_BASELINE(use_std_swap_non_trivial);
BENCHMARK_NOTBASE(use_adl_swap_non_trivial);
