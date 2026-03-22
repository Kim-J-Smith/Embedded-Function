#include "benchmark.hpp"

// ----------------------------------------------------------------------------
// Functors
// ----------------------------------------------------------------------------

struct functor_scalar {
    int operator()(int, int, int, int) const {
        volatile int unused = 1; (void)unused;
        return 0;
    }
};

struct functor_trivial {
    void operator()(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    ) const {
        volatile int unused = 1; (void)unused;
    }
};

struct functor_call_trivial {
    void operator()(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    ) const {
        volatile int unused = 1; (void)unused;
    }
};

// ----------------------------------------------------------------------------
// Functor.ScalarParameters
// ----------------------------------------------------------------------------

BENCHMARK_UNIT(Functor.ScalarParameters);

static void functor_scalar_std(picobench::state& s) {
    functor_scalar f1, f2;
    std::function<int(int, int, int, int)> fn1 = f1;
    std::function<int(int, int, int, int)> fn2 = f2;

    for (auto _ : s) {
        fn1(0x111, 0x222, 0x333, 0x444);
        fn2(0x222, 0x111, 0x333, 0x444);
    }
}

static void functor_scalar_ebd(picobench::state& s) {
    functor_scalar f1, f2;
    ebd::fn<int(int, int, int, int)> fn1 = f1;
    ebd::fn<int(int, int, int, int)> fn2 = f2;

    for (auto _ : s) {
        fn1(0x111, 0x222, 0x333, 0x444);
        fn2(0x222, 0x111, 0x333, 0x444);
    }
}

static void functor_scalar_fu2(picobench::state& s) {
    functor_scalar f1, f2;
    fu2::function<int(int, int, int, int)> fn1 = f1;
    fu2::function<int(int, int, int, int)> fn2 = f2;

    for (auto _ : s) {
        fn1(0x111, 0x222, 0x333, 0x444);
        fn2(0x222, 0x111, 0x333, 0x444);
    }
}

BENCHMARK_BASELINE(functor_scalar_std);
BENCHMARK_NOTBASE(functor_scalar_ebd);
BENCHMARK_NOTBASE(functor_scalar_fu2);


// ----------------------------------------------------------------------------
// Functor.TrivialParameters
// ----------------------------------------------------------------------------

BENCHMARK_UNIT(Functor.TrivialParameters);

static void functor_trivial_std(picobench::state& s) {
    functor_trivial f1, f2;
    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn1 = f1;
    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn2 = f2;

    benchmark_trivial_struct trivial_{nullptr};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void functor_trivial_ebd(picobench::state& s) {
    functor_trivial f1, f2;
    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn1 = f1;
    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn2 = f2;

    benchmark_trivial_struct trivial_{nullptr};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void functor_trivial_fu2(picobench::state& s) {
    functor_trivial f1, f2;
    fu2::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn1 = f1;
    fu2::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn2 = f2;

    benchmark_trivial_struct trivial_{nullptr};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(functor_trivial_std);
BENCHMARK_NOTBASE(functor_trivial_ebd);
BENCHMARK_NOTBASE(functor_trivial_fu2);


// ----------------------------------------------------------------------------
// Functor.CallTrivialParameters
// ----------------------------------------------------------------------------

BENCHMARK_UNIT(Functor.CallTrivialParameters);

static void functor_calltrivial_std(picobench::state& s) {
    functor_call_trivial f1, f2;
    std::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn1 = f1;
    std::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn2 = f2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void functor_calltrivial_ebd(picobench::state& s) {
    functor_call_trivial f1, f2;
    ebd::fn<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn1 = f1;
    ebd::fn<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn2 = f2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void functor_calltrivial_fu2(picobench::state& s) {
    functor_call_trivial f1, f2;
    fu2::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn1 = f1;
    fu2::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn2 = f2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn1(trivial_, trivial_, trivial_, trivial_);
        fn2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(functor_calltrivial_std);
BENCHMARK_NOTBASE(functor_calltrivial_ebd);
BENCHMARK_NOTBASE(functor_calltrivial_fu2);
