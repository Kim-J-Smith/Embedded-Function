#include "benchmark.hpp"

BENCHMARK_UNIT(FreeFunction.ScalarParameters);

static int scalar_1(int, int, int, int) {
    volatile int unused = 1; (void)unused; return 0;
}
static int scalar_2(int, int, int, int) {
    volatile int unused = 1; (void)unused; return 0;
}

static void free_scalar_std(picobench::state& s) {
    std::function<int(int, int, int, int)> fn_scalar1_std = scalar_1;
    std::function<int(int, int, int, int)> fn_scalar2_std = scalar_2;

    for (auto _ : s) {
        fn_scalar1_std(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_std(0x222, 0x111, 0x333, 0x444);
    }
}

static void free_scalar_ebd(picobench::state& s) {
    ebd::fn<int(int, int, int, int)> fn_scalar1_ebd = scalar_1;
    ebd::fn<int(int, int, int, int)> fn_scalar2_ebd = scalar_2;

    for (auto _ : s) {
        fn_scalar1_ebd(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_ebd(0x222, 0x111, 0x333, 0x444);
    }
}

static void free_scalar_fu2(picobench::state& s) {
    fu2::function<int(int, int, int, int)> fn_scalar1_fu2 = scalar_1;
    fu2::function<int(int, int, int, int)> fn_scalar2_fu2 = scalar_2;

    for (auto _ : s) {
        fn_scalar1_fu2(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_fu2(0x222, 0x111, 0x333, 0x444);
    }
}

BENCHMARK_BASELINE(free_scalar_std);
BENCHMARK_NOTBASE(free_scalar_ebd);
BENCHMARK_NOTBASE(free_scalar_fu2);


BENCHMARK_UNIT(FreeFunction.TrivialParameters);

static void pass_trivial_args_1(
    benchmark_trivial_struct, 
    benchmark_trivial_struct,
    benchmark_trivial_struct,
    benchmark_trivial_struct
) { volatile int unused = 1; (void)unused; }
static void pass_trivial_args_2(
    benchmark_trivial_struct, 
    benchmark_trivial_struct,
    benchmark_trivial_struct,
    benchmark_trivial_struct
) { volatile int unused = 2; (void)unused; }

static void free_trivial_std(picobench::state& s) {
    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial1_std = pass_trivial_args_1;
    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial2_std = pass_trivial_args_2;

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (auto _ : s) {
        fn_trivial1_std(trivial_, trivial_, trivial_, trivial_);
        fn_trivial2_std(trivial_, trivial_, trivial_, trivial_);
    }
}

static void free_trivial_ebd(picobench::state& s) {
    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial1_ebd = pass_trivial_args_1;
    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial2_ebd = pass_trivial_args_2;

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (auto _ : s) {
        fn_trivial1_ebd(trivial_, trivial_, trivial_, trivial_);
        fn_trivial2_ebd(trivial_, trivial_, trivial_, trivial_);
    }
}

static void free_trivial_fu2(picobench::state& s) {
    fu2::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial1_fu2 = pass_trivial_args_1;
    fu2::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial2_fu2 = pass_trivial_args_2;

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (auto _ : s) {
        fn_trivial1_fu2(trivial_, trivial_, trivial_, trivial_);
        fn_trivial2_fu2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(free_trivial_std);
BENCHMARK_NOTBASE(free_trivial_ebd);
BENCHMARK_NOTBASE(free_trivial_fu2);


BENCHMARK_UNIT(FreeFunction.CopyHardParameters);

static void pass_copy_hard_struct_1(
    benchmark_copy_hard_struct, 
    benchmark_copy_hard_struct
) {
    volatile int unused = 1;
    (void)unused;
}

static void pass_copy_hard_struct_2(
    benchmark_copy_hard_struct, 
    benchmark_copy_hard_struct
) {
    volatile int unused = 1;
    (void)unused;
}

static void free_copyhard_std(picobench::state& s) {
    std::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard1_std = pass_copy_hard_struct_1;
    std::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard2_std = pass_copy_hard_struct_2;

    benchmark_copy_hard_struct trivial_{};
    fn_copy_hard1_std.swap(fn_copy_hard2_std);

    for (auto _ : s) {
        fn_copy_hard1_std(trivial_, trivial_);
        fn_copy_hard2_std(trivial_, trivial_);
    }
}

static void free_copyhard_ebd(picobench::state& s) {
    ebd::fn<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard1_ebd = pass_copy_hard_struct_1;
    ebd::fn<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard2_ebd = pass_copy_hard_struct_2;

    benchmark_copy_hard_struct trivial_{};
    fn_copy_hard1_ebd.swap(fn_copy_hard2_ebd);

    for (auto _ : s) {
        fn_copy_hard1_ebd(trivial_, trivial_);
        fn_copy_hard2_ebd(trivial_, trivial_);
    }
}

static void free_copyhard_fu2(picobench::state& s) {
    fu2::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard1_fu2 = pass_copy_hard_struct_1;
    fu2::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard2_fu2 = pass_copy_hard_struct_2;

    benchmark_copy_hard_struct trivial_{};
    fn_copy_hard1_fu2.swap(fn_copy_hard2_fu2);

    for (auto _ : s) {
        fn_copy_hard1_fu2(trivial_, trivial_);
        fn_copy_hard2_fu2(trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(free_copyhard_std);
BENCHMARK_NOTBASE(free_copyhard_ebd);
BENCHMARK_NOTBASE(free_copyhard_fu2);


BENCHMARK_UNIT(FreeFunction.CallTrivialParameters);

static void pass_call_trivial_args_1(
    benchmark_call_trivial_struct, 
    benchmark_call_trivial_struct,
    benchmark_call_trivial_struct,
    benchmark_call_trivial_struct
) { volatile int unused = 1; (void)unused; }

static void pass_call_trivial_args_2(
    benchmark_call_trivial_struct, 
    benchmark_call_trivial_struct,
    benchmark_call_trivial_struct,
    benchmark_call_trivial_struct
) { volatile int unused = 2; (void)unused; }

static void free_calltrivial_std(picobench::state& s) {
    std::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial1_std = pass_call_trivial_args_1;
    std::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial2_std = pass_call_trivial_args_2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn_trivial1_std(trivial_, trivial_, trivial_, trivial_);
        fn_trivial2_std(trivial_, trivial_, trivial_, trivial_);
    }
}

static void free_calltrivial_ebd(picobench::state& s) {
    ebd::fn<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial1_ebd = pass_call_trivial_args_1;
    ebd::fn<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial2_ebd = pass_call_trivial_args_2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn_trivial1_ebd(trivial_, trivial_, trivial_, trivial_);
        fn_trivial2_ebd(trivial_, trivial_, trivial_, trivial_);
    }
}

static void free_calltrivial_fu2(picobench::state& s) {
    fu2::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial1_fu2 = pass_call_trivial_args_1;
    fu2::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial2_fu2 = pass_call_trivial_args_2;

    benchmark_call_trivial_struct trivial_{};

    for (auto _ : s) {
        fn_trivial1_fu2(trivial_, trivial_, trivial_, trivial_);
        fn_trivial2_fu2(trivial_, trivial_, trivial_, trivial_);
    }
}

BENCHMARK_BASELINE(free_calltrivial_std);
BENCHMARK_NOTBASE(free_calltrivial_ebd);
BENCHMARK_NOTBASE(free_calltrivial_fu2);
