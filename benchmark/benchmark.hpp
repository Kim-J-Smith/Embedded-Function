#pragma once

#include <chrono>
#include "embed/embed_function.hpp"

#if defined(_MSC_VER)
# include <intrin.h>
# define BENCHMARK_BARRIER() _ReadWriteBarrier()
# define BENCHMARK_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
# define BENCHMARK_BARRIER() __asm__ volatile ("" : : : "memory")
# define BENCHMARK_NOINLINE __attribute__((noinline))
#else
# define BENCHMARK_BARRIER()
# define BENCHMARK_NOINLINE
#endif

#define BENCHMARK_TIMES 10000000 // 10_000_000

inline double benchmark_get_time_us(
    std::chrono::high_resolution_clock::time_point begin,
    std::chrono::high_resolution_clock::time_point end
) {
    using us_t = std::chrono::duration<double, std::micro>;
    auto time = std::chrono::duration_cast<us_t>(end - begin);
    return time.count();
}

template <typename T>
inline auto benchmark_get_benchmark(T&& run_benchmark) noexcept
-> ebd::fn<double() const> {
    return [run_benchmark]() -> double {
        auto time_begin = std::chrono::high_resolution_clock::now();

        BENCHMARK_BARRIER();

        run_benchmark();

        BENCHMARK_BARRIER();

        auto time_end = std::chrono::high_resolution_clock::now();

        return benchmark_get_time_us(time_begin, time_end);
    };
}

struct benchmark_trivial_struct {
    void* pod;
};

struct benchmark_call_trivial_struct {
    void* pod;
    benchmark_call_trivial_struct() : pod(nullptr) {
        volatile int unused = 114;
        (void)unused;
    }
};

struct benchmark_huge_trivial_struct {
    volatile char huge[1024];
};

struct benchmark_copy_hard_struct {
    benchmark_copy_hard_struct() = default;
    ~benchmark_copy_hard_struct() = default;

    benchmark_copy_hard_struct(const benchmark_copy_hard_struct&)
    {
        volatile int c = 1;
        for (int i = 0; i < 400; i++) {
            c = static_cast<int>((c * static_cast<double>(i)) + (c * c)) & 0x0F0F0F;
        }
    }
};

struct benchmark_trivial_functor {
    void* m_var;
    void* operator()() const {
        return m_var;
    }
};
