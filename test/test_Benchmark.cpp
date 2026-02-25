#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include <functional>
#include <chrono>
#include <cstdio>

#if defined(_MSC_VER)
# include <intrin.h>
# define MEM_BARRIER() _ReadWriteBarrier()
#elif defined(__GNUC__) || defined(__clang__)
# define MEM_BARRIER() __asm__("" : : : "memory")
#else
# define MEM_BARRIER()
#endif

static int add(int a, int b) { volatile int c = a + b; (void)c; return a + b; }
static int sub(int a, int b) { volatile int c = a - b; (void)c; return a - b; }

template <typename Fn>
static void run_benchmark(Fn&& fn, int count) {
    for (int i = 0; i < count; i++) {
        (void)fn(111, 222);
    }
}

static double get_benchmark_time_us(
    std::chrono::high_resolution_clock::time_point begin,
    std::chrono::high_resolution_clock::time_point end
) {
    using us_t = std::chrono::duration<double, std::micro>;
    auto time = std::chrono::duration_cast<us_t>(end - begin);
    return time.count();
}

TEST(Benchmark, FreeFunctionTest) {

    constexpr int call_count = 10000000;

    auto std_fn1 = std::function<int(int, int)>(add);
    auto std_fn2 = std::function<int(int, int)>(sub);

    auto ebd_fn1 = ebd::make_fn(add);
    auto ebd_fn2 = ebd::make_fn(sub);

    // test std_fn1
    MEM_BARRIER();
    auto tPoint_std_begin1 = std::chrono::high_resolution_clock::now();
    run_benchmark(std_fn1, call_count);
    MEM_BARRIER();
    auto tPoint_std_end1 = std::chrono::high_resolution_clock::now();

    // test std_fn2
    MEM_BARRIER();
    auto tPoint_std_begin2 = std::chrono::high_resolution_clock::now();
    run_benchmark(std_fn2, call_count);
    MEM_BARRIER();
    auto tPoint_std_end2 = std::chrono::high_resolution_clock::now();

    // test ebd_fn1
    MEM_BARRIER();
    auto tPoint_ebd_begin1 = std::chrono::high_resolution_clock::now();
    run_benchmark(ebd_fn1, call_count);
    MEM_BARRIER();
    auto tPoint_ebd_end1 = std::chrono::high_resolution_clock::now();

    // test ebd_fn2
    MEM_BARRIER();
    auto tPoint_ebd_begin2 = std::chrono::high_resolution_clock::now();
    run_benchmark(ebd_fn2, call_count);
    MEM_BARRIER();
    auto tPoint_ebd_end2 = std::chrono::high_resolution_clock::now();

    double std_time1 = get_benchmark_time_us(tPoint_std_begin1, tPoint_std_end1);
    double std_time2 = get_benchmark_time_us(tPoint_std_begin2, tPoint_std_end2);
    double ebd_time1 = get_benchmark_time_us(tPoint_ebd_begin1, tPoint_ebd_end1);
    double ebd_time2 = get_benchmark_time_us(tPoint_ebd_begin2, tPoint_ebd_end2);

    double std_time_all = std_time1 + std_time2;
    double ebd_time_all = ebd_time1 + ebd_time2;

    double performance_ratio = std_time_all / ebd_time_all; // The bigger the better.

    std::printf("[ Benchmark] calls=%d, ebd::fn=%lf, std::function=%lf, ratio=%f\n",
        call_count, ebd_time_all, std_time_all, performance_ratio);

    EXPECT_EQ(performance_ratio > 0.9, true);
    SUCCEED();
}

