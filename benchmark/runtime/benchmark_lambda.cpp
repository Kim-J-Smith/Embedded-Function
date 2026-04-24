#include "benchmark.hpp"

// ----------------------------------------------------------------------------
// Lambda.ScalarParameters
// ----------------------------------------------------------------------------

BENCHMARK_UNIT(Lambda.ScalarParameters);

static void lambda_scalar_std(picobench::state& s) {
    auto l1 = [](int, int, int, int) { volatile int unused = 1; (void)unused; return 0; };
    auto l2 = [](int, int, int, int) { volatile int unused = 1; (void)unused; return 0; };
    std::function<int(int, int, int, int)> fn1 = l1;
    std::function<int(int, int, int, int)> fn2 = l2;

    for (auto _ : s) {
        fn1(0x111, 0x222, 0x333, 0x444);
        fn2(0x222, 0x111, 0x333, 0x444);
    }
}

static void lambda_scalar_ebd(picobench::state& s) {
    auto l1 = [](int, int, int, int) { volatile int unused = 1; (void)unused; return 0; };
    auto l2 = [](int, int, int, int) { volatile int unused = 1; (void)unused; return 0; };
    ebd::fn<int(int, int, int, int)> fn1 = l1;
    ebd::fn<int(int, int, int, int)> fn2 = l2;

    for (auto _ : s) {
        fn1(0x111, 0x222, 0x333, 0x444);
        fn2(0x222, 0x111, 0x333, 0x444);
    }
}

static void lambda_scalar_fu2(picobench::state& s) {
    auto l1 = [](int, int, int, int) { volatile int unused = 1; (void)unused; return 0; };
    auto l2 = [](int, int, int, int) { volatile int unused = 1; (void)unused; return 0; };
    fu2::function<int(int, int, int, int)> fn1 = l1;
    fu2::function<int(int, int, int, int)> fn2 = l2;

    for (auto _ : s) {
        fn1(0x111, 0x222, 0x333, 0x444);
        fn2(0x222, 0x111, 0x333, 0x444);
    }
}

BENCHMARK_BASELINE(lambda_scalar_std);
BENCHMARK_NOTBASE(lambda_scalar_ebd);
BENCHMARK_NOTBASE(lambda_scalar_fu2);


// ----------------------------------------------------------------------------
// Lambda.TrivialParameters
// ----------------------------------------------------------------------------

BENCHMARK_UNIT(Lambda.TrivialParameters);

static void lambda_trivial_std(picobench::state& s) {
    auto l1 = [](benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    std::function<void(benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct)> fn1 = l1;
    std::function<void(benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct)> fn2 = l2;

    benchmark_trivial_struct trivial_{nullptr};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void lambda_trivial_ebd(picobench::state& s) {
    auto l1 = [](benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    ebd::fn<void(benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct)> fn1 = l1;
    ebd::fn<void(benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct)> fn2 = l2;

    benchmark_trivial_struct trivial_{nullptr};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void lambda_trivial_fu2(picobench::state& s) {
    auto l1 = [](benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    fu2::function<void(benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct)> fn1 = l1;
    fu2::function<void(benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct, benchmark_trivial_struct)> fn2 = l2;

    benchmark_trivial_struct trivial_{nullptr};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(lambda_trivial_std);
BENCHMARK_NOTBASE(lambda_trivial_ebd);
BENCHMARK_NOTBASE(lambda_trivial_fu2);


// ----------------------------------------------------------------------------
// Lambda.CallTrivialParameters
// ----------------------------------------------------------------------------

BENCHMARK_UNIT(Lambda.CallTrivialParameters);

static void lambda_calltrivial_std(picobench::state& s) {
    auto l1 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 1; (void)unused; 
    };
    auto l2 = [](benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct) { 
        volatile int unused = 2; (void)unused; 
    };
    std::function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn1 = l1;
    std::function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn2 = l2;

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
    ebd::fn<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn1 = l1;
    ebd::fn<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn2 = l2;

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
    fu2::function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn1 = l1;
    fu2::function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn2 = l2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(lambda_calltrivial_std);
BENCHMARK_NOTBASE(lambda_calltrivial_ebd);
BENCHMARK_NOTBASE(lambda_calltrivial_fu2);
