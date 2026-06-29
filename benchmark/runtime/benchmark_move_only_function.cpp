#include "benchmark.hpp"

// ----------------------------------------------------------------------------
// MoveOnlyFunction.Lambda.CallTrivialParam
// ----------------------------------------------------------------------------

#if __cpp_lib_move_only_function >= 202110L

static void lambda_calltrivial_std(picobench::state& s) {
    auto l1 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    std::move_only_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn1 = l1;
    std::move_only_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn2 = l2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void lambda_calltrivial_ebd(picobench::state& s) {
    auto l1 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    ebd::unique_fn<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn1 = l1;
    ebd::unique_fn<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn2 = l2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void lambda_calltrivial_fu2(picobench::state& s) {
    auto l1 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    fu2::unique_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn1 = l1;
    fu2::unique_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn2 = l2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Lambda.CallTrivialParam);
BENCHMARK_BASELINE(lambda_calltrivial_std);
BENCHMARK_NOTBASE(lambda_calltrivial_ebd);
BENCHMARK_NOTBASE(lambda_calltrivial_fu2);

#endif
