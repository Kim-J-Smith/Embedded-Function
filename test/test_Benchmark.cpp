#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

#include <functional>
#include <chrono>
#include <cstdio>

static int bench_add(int left_value, int right_value) { return left_value + right_value; }

static long long to_us(std::chrono::steady_clock::duration d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

TEST(Benchmark, CallOverheads) {
    const int call_count = 1000000;
    volatile int result_accumulator = 0;

    auto start_time = std::chrono::steady_clock::now();
    for (int iteration_index = 0; iteration_index < call_count; ++iteration_index) {
        result_accumulator += bench_add(iteration_index, 1);
    }
    auto elapsed_function_pointer = std::chrono::steady_clock::now() - start_time;

    ebd::fn<int(int,int)> embedded_fn = bench_add;
    start_time = std::chrono::steady_clock::now();
    for (int iteration_index = 0; iteration_index < call_count; ++iteration_index) {
        result_accumulator += embedded_fn(iteration_index, 1);
    }
    auto elapsed_embedded_fn = std::chrono::steady_clock::now() - start_time;

    std::function<int(int,int)> std_function = bench_add;
    start_time = std::chrono::steady_clock::now();
    for (int iteration_index = 0; iteration_index < call_count; ++iteration_index) {
        result_accumulator += std_function(iteration_index, 1);
    }
    auto elapsed_std_function = std::chrono::steady_clock::now() - start_time;

    std::printf("[ Benchmark] calls=%d fp=%lldus ebd::fn=%lldus std::function=%lldus sink=%d\n",
        call_count,
        static_cast<long long>(to_us(elapsed_function_pointer)),
        static_cast<long long>(to_us(elapsed_embedded_fn)),
        static_cast<long long>(to_us(elapsed_std_function)),
        static_cast<int>(result_accumulator));

    auto embedded_fn_time_us = static_cast<double>(to_us(elapsed_embedded_fn));
    auto std_function_time_us = static_cast<double>(to_us(elapsed_std_function));
    auto performance_ratio = std_function_time_us / embedded_fn_time_us;
    EXPECT_EQ(performance_ratio > 0.90, true);
    SUCCEED();
}
