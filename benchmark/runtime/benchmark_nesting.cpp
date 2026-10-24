#include "benchmark.hpp"

#include <functional>
#include <utility>

// Five levels with different value/`&&` signatures of a non register-passable
// payload: ebd converts (is_convertible_from_specialization), others may re-wrap.

struct nesting_payload {
    int v0, v1, v2, v3, v4;
};

static int nesting_base(nesting_payload a, nesting_payload b, nesting_payload c) {
    return a.v0 + b.v1 + c.v2;
}

#define NESTING_MOVE(x)   std::move(x)
#define NESTING_BORROW(x) (x)

#define NESTING_CHAIN(WRAPPER, PASS)                                                     \
    WRAPPER<int(nesting_payload, nesting_payload, nesting_payload)> f1 = &nesting_base;  \
    WRAPPER<int(nesting_payload&&, nesting_payload, nesting_payload)> f2 = PASS(f1);     \
    WRAPPER<int(nesting_payload, nesting_payload&&, nesting_payload)> f3 = PASS(f2);     \
    WRAPPER<int(nesting_payload, nesting_payload, nesting_payload&&)> f4 = PASS(f3);     \
    WRAPPER<int(nesting_payload&&, nesting_payload&&, nesting_payload&&)> f5 = PASS(f4); \
    auto w = benchmark_opaque_noninline(PASS(f5));                                       \
    nesting_payload payload{1, 2, 3, 4, 5};                                              \
    for (std::size_t i = 0; i < BENCHMARK_WARMUP; i++) {                                 \
        volatile int res = w(NESTING_MOVE(payload), NESTING_MOVE(payload), NESTING_MOVE(payload)); \
        (void)res;                                                                       \
    }                                                                                    \
    for (auto _ : s) {                                                                   \
        volatile int res = w(NESTING_MOVE(payload), NESTING_MOVE(payload), NESTING_MOVE(payload)); \
        (void)res;                                                                       \
    }

static void nesting_std(picobench::state& s) {
    NESTING_CHAIN(std::function, NESTING_BORROW)
}

static void nesting_fu2(picobench::state& s) {
    NESTING_CHAIN(fu2::function, NESTING_BORROW)
}

static void nesting_ebd(picobench::state& s) {
    NESTING_CHAIN(ebd::fn, NESTING_BORROW)
}

#if __cpp_lib_copyable_function >= 202306L
static void nesting_std_copyable(picobench::state& s) {
    NESTING_CHAIN(std::copyable_function, NESTING_BORROW)
}
#endif

BENCHMARK_UNIT(Nesting.SimilarSignature.Copyable);
BENCHMARK_BASELINE(nesting_std);
BENCHMARK_NOTBASE(nesting_fu2);
#if __cpp_lib_copyable_function >= 202306L
BENCHMARK_NOTBASE(nesting_std_copyable);
#endif
BENCHMARK_NOTBASE(nesting_ebd);

#if __cpp_lib_move_only_function >= 202110L
static void nesting_std_moveonly(picobench::state& s) {
    NESTING_CHAIN(std::move_only_function, NESTING_MOVE)
}

static void nesting_ebd_moveonly(picobench::state& s) {
    NESTING_CHAIN(ebd::unique_fn, NESTING_MOVE)
}

BENCHMARK_UNIT(Nesting.SimilarSignature.MoveOnly);
BENCHMARK_BASELINE(nesting_std_moveonly);
BENCHMARK_NOTBASE(nesting_ebd_moveonly);
#endif // C++ >= 23

#if __cpp_lib_function_ref >= 202603L
static void nesting_std_ref(picobench::state& s) {
    NESTING_CHAIN(std::function_ref, NESTING_BORROW)
}

static void nesting_ebd_ref(picobench::state& s) {
    NESTING_CHAIN(ebd::fn_ref, NESTING_BORROW)
}

BENCHMARK_UNIT(Nesting.SimilarSignature.Reference);
BENCHMARK_BASELINE(nesting_std_ref);
BENCHMARK_NOTBASE(nesting_ebd_ref);
#endif // C++ >= 26
