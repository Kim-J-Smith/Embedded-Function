#include "embed/embed_function.hpp"
#include "benchmark.hpp"

#include <cstdio>
#include <cstdint>

static void benchmark_lambda_scalar_param_ebd() {
    ebd::fn<int(int, int, int, int)> fn_scalar1_ebd = [](int,int,int,int) {
        volatile int unused = 1;
        (void)unused;
        return 1;
    };
    ebd::fn<int(int, int, int, int)> fn_scalar2_ebd = [](int,int,int,int) {
        volatile int unused = 1;
        (void)unused;
        return 2;
    };

    fn_scalar1_ebd.swap(fn_scalar2_ebd);

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_scalar1_ebd(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_ebd(0x222, 0x111, 0x333, 0x444);
    }
}

static void benchmark_lambda_scalar_param_std() {
    std::function<int(int, int, int, int)> fn_scalar1_std = [](int,int,int,int) {
        volatile int unused = 1;
        (void)unused;
        return 1;
    };
    std::function<int(int, int, int, int)> fn_scalar2_std = [](int,int,int,int) {
        volatile int unused = 1;
        (void)unused;
        return 2;
    };

    fn_scalar1_std.swap(fn_scalar2_std);

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_scalar1_std(0x111, 0x222, 0x333, 0x444);
        fn_scalar2_std(0x222, 0x111, 0x333, 0x444);
    }
}

static void benchmark_lambda_trivial_param_ebd() {
    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial_ebd1 = [](
        benchmark_trivial_struct,benchmark_trivial_struct,
        benchmark_trivial_struct,benchmark_trivial_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };

    ebd::fn<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial_ebd2 = [](
        benchmark_trivial_struct,benchmark_trivial_struct,
        benchmark_trivial_struct,benchmark_trivial_struct
    ) {
        volatile int unused = 2;
        (void)unused;
    };

    fn_trivial_ebd1.swap(fn_trivial_ebd2);

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_ebd1(trivial_, trivial_, trivial_, trivial_);
        fn_trivial_ebd2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_lambda_trivial_param_std() {
    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial_std1 = [](
        benchmark_trivial_struct,benchmark_trivial_struct,
        benchmark_trivial_struct,benchmark_trivial_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };

    std::function<void(
        benchmark_trivial_struct, benchmark_trivial_struct,
        benchmark_trivial_struct, benchmark_trivial_struct
    )> fn_trivial_std2 = [](
        benchmark_trivial_struct,benchmark_trivial_struct,
        benchmark_trivial_struct,benchmark_trivial_struct
    ) {
        volatile int unused = 2;
        (void)unused;
    };

    fn_trivial_std1.swap(fn_trivial_std2);

    benchmark_trivial_struct trivial_;
    trivial_.pod = nullptr;

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_std1(trivial_, trivial_, trivial_, trivial_);
        fn_trivial_std2(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_lambda_call_trivial_param_ebd() {
    ebd::fn<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial_ebd = [](
        benchmark_call_trivial_struct,benchmark_call_trivial_struct,
        benchmark_call_trivial_struct,benchmark_call_trivial_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };

    auto copy = fn_trivial_ebd;
    fn_trivial_ebd.swap(copy);

    benchmark_call_trivial_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_ebd(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_lambda_call_trivial_param_std() {
    std::function<void(
        benchmark_call_trivial_struct, benchmark_call_trivial_struct,
        benchmark_call_trivial_struct, benchmark_call_trivial_struct
    )> fn_trivial_std = [](
        benchmark_call_trivial_struct,benchmark_call_trivial_struct,
        benchmark_call_trivial_struct,benchmark_call_trivial_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };

    auto copy = fn_trivial_std;
    fn_trivial_std.swap(copy);

    benchmark_call_trivial_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES; i++) {
        fn_trivial_std(trivial_, trivial_, trivial_, trivial_);
    }
}

static void benchmark_lambda_huge_trivial_param_ebd() {
    ebd::fn<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_ebd1 = [](
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };
    ebd::fn<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_ebd2 = [](
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    ) {
        volatile int unused = 2;
        (void)unused;
    };

    benchmark_huge_trivial_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES/100; i++) {
        fn_trivial_ebd1(trivial_, trivial_);
        fn_trivial_ebd2(trivial_, trivial_);
    }
}

static void benchmark_lambda_huge_trivial_param_std() {
    std::function<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_std1 = [](
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };
    std::function<void(
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    )> fn_trivial_std2 = [](
        benchmark_huge_trivial_struct, benchmark_huge_trivial_struct
    ) {
        volatile int unused = 2;
        (void)unused;
    };

    benchmark_huge_trivial_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES/100; i++) {
        fn_trivial_std1(trivial_, trivial_);
        fn_trivial_std2(trivial_, trivial_);
    }
}




static void benchmark_lambda_copy_hard_param_ebd() {
    ebd::fn<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_ebd1 = [](
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };
    ebd::fn<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_ebd2 = [](
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    ) {
        volatile int unused = 2;
        (void)unused;
    };

    benchmark_copy_hard_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES/10000; i++) {
        fn_copy_hard_ebd1(trivial_, trivial_);
        fn_copy_hard_ebd2(trivial_, trivial_);
    }
}

static void benchmark_lambda_copy_hard_param_std() {
    std::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_std1 = [](
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    ) {
        volatile int unused = 1;
        (void)unused;
    };
    std::function<void(
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    )> fn_copy_hard_std2 = [](
        benchmark_copy_hard_struct, benchmark_copy_hard_struct
    ) {
        volatile int unused = 2;
        (void)unused;
    };

    benchmark_copy_hard_struct trivial_{};

    for (int i = 0; i < BENCHMARK_TIMES/10000; i++) {
        fn_copy_hard_std1(trivial_, trivial_);
        fn_copy_hard_std2(trivial_, trivial_);
    }
}

void benchmark__lambda() {

    /// lambda test => scalar parameter

    auto ebd__benchmark = benchmark_get_benchmark(
        benchmark_lambda_scalar_param_ebd);

    auto std__benchmark = benchmark_get_benchmark(
        benchmark_lambda_scalar_param_std);

    double time_ebd_scalar_param = ebd__benchmark();
    double time_std_scalar_param = std__benchmark();
    time_ebd_scalar_param += ebd__benchmark();
    time_std_scalar_param += std__benchmark();
    time_ebd_scalar_param += ebd__benchmark();
    time_std_scalar_param += std__benchmark();

    printf("[Benchmark lambda scalar]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_scalar_param, 
            time_std_scalar_param,
            100.0 * (time_ebd_scalar_param/time_std_scalar_param - 1.0));
    fflush(stdout);

    /// lambda test => trivial parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_lambda_trivial_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_lambda_trivial_param_std);

    double time_ebd_trivial_param = ebd__benchmark();
    double time_std_trivial_param = std__benchmark();
    time_ebd_trivial_param += ebd__benchmark();
    time_std_trivial_param += std__benchmark();
    time_ebd_trivial_param += ebd__benchmark();
    time_std_trivial_param += std__benchmark();

    printf("[Benchmark lambda trivial]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_trivial_param, 
            time_std_trivial_param,
            100.0 * (time_ebd_trivial_param/time_std_trivial_param - 1.0));
    fflush(stdout);

    /// lambda test => call trivial parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_lambda_call_trivial_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_lambda_call_trivial_param_std);

    double time_ebd_call_trivial_param = ebd__benchmark();
    double time_std_call_trivial_param = std__benchmark();
    time_ebd_call_trivial_param += ebd__benchmark();
    time_std_call_trivial_param += std__benchmark();
    time_ebd_call_trivial_param += ebd__benchmark();
    time_std_call_trivial_param += std__benchmark();

    printf("[Benchmark lambda call-trivial]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_call_trivial_param, 
            time_std_call_trivial_param,
            100 * (time_ebd_call_trivial_param/time_std_call_trivial_param - 1.0));
    fflush(stdout);

    /// lambda test => huge trivial parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_lambda_huge_trivial_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_lambda_huge_trivial_param_std);

    double time_ebd_huge_trivial_param = ebd__benchmark();
    double time_std_huge_trivial_param = std__benchmark();
    time_ebd_huge_trivial_param += ebd__benchmark();
    time_std_huge_trivial_param += std__benchmark();
    time_ebd_huge_trivial_param += ebd__benchmark();
    time_std_huge_trivial_param += std__benchmark();

    printf("[Benchmark lambda huge-trivial]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_huge_trivial_param, 
            time_std_huge_trivial_param,
            100.0 * (time_ebd_huge_trivial_param/time_std_huge_trivial_param - 1.0));
    fflush(stdout);

    /// lambda test => copy hard parameter

    ebd__benchmark = benchmark_get_benchmark(
        benchmark_lambda_copy_hard_param_ebd);

    std__benchmark = benchmark_get_benchmark(
        benchmark_lambda_copy_hard_param_std);

    double time_ebd_copy_hard_param = ebd__benchmark();
    double time_std_copy_hard_param = std__benchmark();
    time_ebd_copy_hard_param += ebd__benchmark();
    time_std_copy_hard_param += std__benchmark();
    time_ebd_copy_hard_param += ebd__benchmark();
    time_std_copy_hard_param += std__benchmark();

    printf("[Benchmark lambda copy-hard]"
            " time-ebd = %.2lf us, time-std = %.2lf us, Time: %.6lf%%\n", 
            time_ebd_copy_hard_param, 
            time_std_copy_hard_param,
            100.0 * (time_ebd_copy_hard_param/time_std_copy_hard_param - 1.0));
    fflush(stdout);
}
