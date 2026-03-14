#include "embed/embed_function.hpp"
#include "benchmark.hpp"

#include <cstdio>
#include <cstdint>

static int scalar_1(int, int, int, int) {
    volatile int unused = 1; (void)unused; return 0;
}
static int scalar_2(int, int, int, int) {
    volatile int unused = 1; (void)unused; return 0;
}

static void pass_trivial_args(
    benchmark_trivial_struct, 
    benchmark_trivial_struct,
    benchmark_trivial_struct,
    benchmark_trivial_struct
) {
    volatile int unused = 1;
    (void)unused;
}

static void pass_call_trivial_args(
    benchmark_call_trivial_struct, 
    benchmark_call_trivial_struct,
    benchmark_call_trivial_struct,
    benchmark_call_trivial_struct
) {
    volatile int unused = 1;
    (void)unused;
}

static void pass_huge_struct_1(
    benchmark_huge_trivial_struct, 
    benchmark_huge_trivial_struct
) {
    volatile int unused = 1;
    (void)unused;
}

static void pass_huge_struct_2(
    benchmark_huge_trivial_struct, 
    benchmark_huge_trivial_struct
) {
    volatile int unused = 1;
    (void)unused;
}

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

static void benchmark_free_function_scalar_param_ebd() {
    ebd::fn<int(int, int, int, int)> fn_scalar1_ebd = scalar_1;
    ebd::fn<int(int, int, int, int)> fn_scalar2_ebd = scalar_2;

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_scalar1_ebd(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_ebd(0x222, 0x111, 0x333, 0x444);
    }
}

static void benchmark_free_function_scalar_param_std() {
    std::function<int(int, int, int, int)> fn_scalar1_std = scalar_1;
    std::function<int(int, int, int, int)> fn_scalar2_std = scalar_2;

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_scalar1_std(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_std(0x222, 0x111, 0x333, 0x444);
    }
}

static void benchmark_free_function_trivial_param_ebd() {
    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial_ebd = nullptr;
    fn_trivial_ebd = pass_trivial_args;

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_ebd(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_free_function_trivial_param_std() {
    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial_std = nullptr;
    fn_trivial_std = pass_trivial_args;

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_std(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_free_function_call_trivial_param_ebd() {
    ebd::fn<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial_ebd = nullptr;
    fn_trivial_ebd = pass_call_trivial_args;
    auto copy = fn_trivial_ebd;
    fn_trivial_ebd.swap(copy);

    benchmark_call_trivial_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_ebd(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_free_function_call_trivial_param_std() {
    std::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial_std = nullptr;
    fn_trivial_std = pass_call_trivial_args;
    auto copy = fn_trivial_std;
    fn_trivial_std.swap(copy);

    benchmark_call_trivial_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_std(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_free_function_huge_trivial_param_ebd() {
    ebd::fn<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_ebd1 = pass_huge_struct_1;
    ebd::fn<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_ebd2 = pass_huge_struct_2;

    benchmark_huge_trivial_struct trivial_{};
    fn_trivial_ebd1.swap(fn_trivial_ebd2);

    for (int i = 0; i < BENCHMARK_TIMES/100; i++) {
        fn_trivial_ebd1(trivial_, trivial_);
        fn_trivial_ebd2(trivial_, trivial_);
    }
}

static void benchmark_free_function_huge_trivial_param_std() {
    std::function<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_std1 = pass_huge_struct_1;
    std::function<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_std2 = pass_huge_struct_2;

    benchmark_huge_trivial_struct trivial_{};
    fn_trivial_std1.swap(fn_trivial_std2);

    for (int i = 0; i < BENCHMARK_TIMES/100; i++) {
        fn_trivial_std1(trivial_, trivial_);
        fn_trivial_std2(trivial_, trivial_);
    }
}




static void benchmark_free_function_copy_hard_param_ebd() {
    ebd::fn<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_ebd1 = pass_copy_hard_struct_1;
    ebd::fn<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_ebd2 = pass_copy_hard_struct_2;

    benchmark_copy_hard_struct trivial_{};
    fn_copy_hard_ebd1.swap(fn_copy_hard_ebd2);

    for (int i = 0; i < BENCHMARK_TIMES/10000; i++) {
        fn_copy_hard_ebd1(trivial_, trivial_);
        fn_copy_hard_ebd2(trivial_, trivial_);
    }
}

static void benchmark_free_function_copy_hard_param_std() {
    std::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_std1 = pass_copy_hard_struct_1;
    std::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_std2 = pass_copy_hard_struct_2;

    benchmark_copy_hard_struct trivial_{};
    fn_copy_hard_std1.swap(fn_copy_hard_std2);

    for (int i = 0; i < BENCHMARK_TIMES/10000; i++) {
        fn_copy_hard_std1(trivial_, trivial_);
        fn_copy_hard_std2(trivial_, trivial_);
    }
}

void benchmark__free_function() {

    /// free function test => scalar parameter

    auto ebd__benchmark = benchmark_get_benchmark(
        benchmark_free_function_scalar_param_ebd);

    auto std__benchmark = benchmark_get_benchmark(
        benchmark_free_function_scalar_param_std);

    double time_ebd_scalar_param = ebd__benchmark();
    double time_std_scalar_param = std__benchmark();
    time_ebd_scalar_param += ebd__benchmark();
    time_std_scalar_param += std__benchmark();
    time_ebd_scalar_param += ebd__benchmark();
    time_std_scalar_param += std__benchmark();

    printf("[Benchmark free function scalar]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_scalar_param, 
            time_std_scalar_param,
            100.0 * (time_ebd_scalar_param/time_std_scalar_param - 1.0));
    fflush(stdout);

    /// free function test => trivial parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_free_function_trivial_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_free_function_trivial_param_std);

    double time_ebd_trivial_param = ebd__benchmark();
    double time_std_trivial_param = std__benchmark();
    time_ebd_trivial_param += ebd__benchmark();
    time_std_trivial_param += std__benchmark();
    time_ebd_trivial_param += ebd__benchmark();
    time_std_trivial_param += std__benchmark();

    printf("[Benchmark free function trivial]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_trivial_param, 
            time_std_trivial_param,
            100.0 * (time_ebd_trivial_param/time_std_trivial_param - 1.0));
    fflush(stdout);

    /// free function test => call trivial parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_free_function_call_trivial_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_free_function_call_trivial_param_std);

    double time_ebd_call_trivial_param = ebd__benchmark();
    double time_std_call_trivial_param = std__benchmark();
    time_ebd_call_trivial_param += ebd__benchmark();
    time_std_call_trivial_param += std__benchmark();
    time_ebd_call_trivial_param += ebd__benchmark();
    time_std_call_trivial_param += std__benchmark();

    printf("[Benchmark free function call-trivial]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_call_trivial_param, 
            time_std_call_trivial_param,
            100 * (time_ebd_call_trivial_param/time_std_call_trivial_param - 1.0));
    fflush(stdout);

    /// free function test => huge trivial parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_free_function_huge_trivial_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_free_function_huge_trivial_param_std);

    double time_ebd_huge_trivial_param = ebd__benchmark();
    double time_std_huge_trivial_param = std__benchmark();
    time_ebd_huge_trivial_param += ebd__benchmark();
    time_std_huge_trivial_param += std__benchmark();
    time_ebd_huge_trivial_param += ebd__benchmark();
    time_std_huge_trivial_param += std__benchmark();

    printf("[Benchmark free function huge-trivial]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_huge_trivial_param, 
            time_std_huge_trivial_param,
            100.0 * (time_ebd_huge_trivial_param/time_std_huge_trivial_param - 1.0));
    fflush(stdout);

    /// free function test => copy hard parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_free_function_copy_hard_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_free_function_copy_hard_param_std);

    double time_ebd_copy_hard_param = ebd__benchmark();
    double time_std_copy_hard_param = std__benchmark();
    time_ebd_copy_hard_param += ebd__benchmark();
    time_std_copy_hard_param += std__benchmark();
    time_ebd_copy_hard_param += ebd__benchmark();
    time_std_copy_hard_param += std__benchmark();

    printf("[Benchmark free function copy-hard]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_copy_hard_param, 
            time_std_copy_hard_param,
            100.0 * (time_ebd_copy_hard_param/time_std_copy_hard_param - 1.0));
    fflush(stdout);
}

