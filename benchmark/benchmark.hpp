#ifndef INCLUDED_BENCHMARK_HPP
#define INCLUDED_BENCHMARK_HPP

#include "embed/embed_function.hpp"
#include "function2/function2.hpp"
#include "picobench/picobench.hpp"

#define BENCHMARK_TIMES {10000, 100000, 1000000}
#define BENCHMARK_REPEAT 10

#define BENCHMARK_UNIT(unit_name) \
    PICOBENCH_SUITE(#unit_name)

#define BENCHMARK_BASELINE(funcname) \
    PICOBENCH(funcname).samples(BENCHMARK_REPEAT).iterations(BENCHMARK_TIMES).baseline()

#define BENCHMARK_NOTBASE(funcname) \
    PICOBENCH(funcname).samples(BENCHMARK_REPEAT).iterations(BENCHMARK_TIMES)

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
        for (int i = 0; i < 2; i++) {
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

#endif // INCLUDED_BENCHMARK_HPP
