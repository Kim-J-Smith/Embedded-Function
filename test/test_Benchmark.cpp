#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include <functional>
#include <chrono>
#include <cstdio>

#if defined(_MSC_VER)
# include <intrin.h>
# define MEM_BARRIER() _ReadWriteBarrier()
# define NO_INLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
# define MEM_BARRIER() __asm__ volatile ("" : : : "memory")
# define NO_INLINE __attribute__((noinline))
#else
# define MEM_BARRIER()
# define NO_INLINE
#endif

static int add(int a, int b) { volatile int c = a + b; (void)c; return a + b; }
static int sub(int a, int b) { volatile int c = a - b; (void)c; return a - b; }

template <typename Fn>
NO_INLINE static void run_benchmark_free_function(Fn&& fn, int count) {
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
    run_benchmark_free_function(std_fn1, call_count);
    MEM_BARRIER();
    auto tPoint_std_end1 = std::chrono::high_resolution_clock::now();

    // test std_fn2
    MEM_BARRIER();
    auto tPoint_std_begin2 = std::chrono::high_resolution_clock::now();
    run_benchmark_free_function(std_fn2, call_count);
    MEM_BARRIER();
    auto tPoint_std_end2 = std::chrono::high_resolution_clock::now();

    // test ebd_fn1
    MEM_BARRIER();
    auto tPoint_ebd_begin1 = std::chrono::high_resolution_clock::now();
    run_benchmark_free_function(ebd_fn1, call_count);
    MEM_BARRIER();
    auto tPoint_ebd_end1 = std::chrono::high_resolution_clock::now();

    // test ebd_fn2
    MEM_BARRIER();
    auto tPoint_ebd_begin2 = std::chrono::high_resolution_clock::now();
    run_benchmark_free_function(ebd_fn2, call_count);
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


template <typename Fn>
NO_INLINE static void run_benchmark_lambda(Fn&& fn, int count) {
    for (int i = 0; i < count; i++) {
        (void)fn(111, 222);
    }
}

TEST(Benchmark, LambdaTest) {

    constexpr int call_count = 10000000;

    int magic_number = 2147318;

    auto lambda_add = [magic_number](int a, int b) {
        volatile int c = magic_number;
        (void)c;
        return a + b;
    };

    auto lambda_sub = [magic_number](int a, int b) {
        volatile int c = magic_number;
        (void)c;
        return a - b;
    };

    auto std_fn1 = std::function<int(int, int)>(lambda_add);
    auto std_fn2 = std::function<int(int, int)>(lambda_sub);

    auto ebd_fn1 = ebd::make_fn(lambda_add);
    auto ebd_fn2 = ebd::make_fn(lambda_sub);

    // test std_fn1
    MEM_BARRIER();
    auto tPoint_std_begin1 = std::chrono::high_resolution_clock::now();
    run_benchmark_lambda(std_fn1, call_count);
    MEM_BARRIER();
    auto tPoint_std_end1 = std::chrono::high_resolution_clock::now();

    // test std_fn2
    MEM_BARRIER();
    auto tPoint_std_begin2 = std::chrono::high_resolution_clock::now();
    run_benchmark_lambda(std_fn2, call_count);
    MEM_BARRIER();
    auto tPoint_std_end2 = std::chrono::high_resolution_clock::now();

    // test ebd_fn1
    MEM_BARRIER();
    auto tPoint_ebd_begin1 = std::chrono::high_resolution_clock::now();
    run_benchmark_lambda(ebd_fn1, call_count);
    MEM_BARRIER();
    auto tPoint_ebd_end1 = std::chrono::high_resolution_clock::now();

    // test ebd_fn2
    MEM_BARRIER();
    auto tPoint_ebd_begin2 = std::chrono::high_resolution_clock::now();
    run_benchmark_lambda(ebd_fn2, call_count);
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
