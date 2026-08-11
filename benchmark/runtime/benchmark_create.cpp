#include "benchmark.hpp"

// ----------------------------------------------------------------------------
// CreateBenchmark.CaptureLambda
// ----------------------------------------------------------------------------

static void lambda_std(picobench::state& s) {
    int a = 0;

    for (auto _ : s) {
        a++;
        std::function<int()> f = [a]() -> int { return a; };
        auto* volatile ff = &f;
        volatile int u = (*ff)(); (void)u;
    }
}

static void lambda_ebd(picobench::state& s) {
    int a = 0;

    for (auto _ : s) {
        a++;
        ebd::fn<int()> f = [a]() -> int { return a; };
        auto* volatile ff = &f;
        volatile int u = (*ff)(); (void)u;
    }
}

static void lambda_fu2(picobench::state& s) {
    int a = 0;

    for (auto _ : s) {
        a++;
        fu2::function<int()> f = [a]() -> int { return a; };
        auto* volatile ff = &f;
        volatile int u = (*ff)(); (void)u;
    }
}

static void lambda_pro(picobench::state& s) {
    int a = 0;
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, int()>
        ::restrict_layout<ebd::fn<int()>::get_buffer_size()>
        ::support_copy<pro::constraint_level::nontrivial>
        ::build;

    for (auto _ : s) {
        a++;
        pro::proxy<Invoker> f = pro::make_proxy_inplace<Invoker>([a]() -> int { return a; });
        auto* volatile ff = &f;
        volatile int u = (**ff)(); (void)u;
    }
}

BENCHMARK_UNIT(CreateBenchmark.CaptureLambda);
BENCHMARK_BASELINE(lambda_std);
BENCHMARK_NOTBASE(lambda_ebd);
BENCHMARK_NOTBASE(lambda_fu2);
BENCHMARK_NOTBASE(lambda_pro);


// ----------------------------------------------------------------------------
// CreateBenchmark.NonTrivialFunctor
// ----------------------------------------------------------------------------

struct NonTrivialFunctor
{
    int a;
    NonTrivialFunctor(int a) : a(a) {}
    NonTrivialFunctor(const NonTrivialFunctor&) {}

    int operator()() { return a; }
};


static void non_trivial_functor_std(picobench::state& s) {
    int a = 0;

    for (auto _ : s) {
        a++;
        std::function<int()> f = NonTrivialFunctor{a};
        auto* volatile ff = &f;
        volatile int u = (*ff)(); (void)u;
    }
}

static void non_trivial_functor_ebd(picobench::state& s) {
    int a = 0;

    for (auto _ : s) {
        a++;
        ebd::fn<int()> f = NonTrivialFunctor{a};
        auto* volatile ff = &f;
        volatile int u = (*ff)(); (void)u;
    }
}

static void non_trivial_functor_fu2(picobench::state& s) {
    int a = 0;

    for (auto _ : s) {
        a++;
        fu2::function<int()> f = NonTrivialFunctor{a};
        auto* volatile ff = &f;
        volatile int u = (*ff)(); (void)u;
    }
}

static void non_trivial_functor_pro(picobench::state& s) {
    int a = 0;
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, int()>
        ::restrict_layout<ebd::fn<int()>::get_buffer_size()>
        ::support_copy<pro::constraint_level::nontrivial>
        ::support_relocation<pro::constraint_level::none>
        ::build;

    for (auto _ : s) {
        a++;
        pro::proxy<Invoker> f = pro::make_proxy_inplace<Invoker>(NonTrivialFunctor{a});
        auto* volatile ff = &f;
        volatile int u = (**ff)(); (void)u;
    }
}

BENCHMARK_UNIT(CreateBenchmark.NonTrivialFunctor);
BENCHMARK_BASELINE(non_trivial_functor_std);
BENCHMARK_NOTBASE(non_trivial_functor_ebd);
BENCHMARK_NOTBASE(non_trivial_functor_fu2);
BENCHMARK_NOTBASE(non_trivial_functor_pro);
