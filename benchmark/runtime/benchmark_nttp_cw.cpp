#include "benchmark.hpp"

namespace {

int add(int a, int b) noexcept { return a + b; }
int sub(int a, int b) noexcept { return a - b; }

struct C {
    int m_var = 42;
    int add(int n) const noexcept { return n + m_var; }
};

};

// Just use a bare std::constant_wrapper

static void cw_cwonly_std(picobench::state& s) {
    std::function<int(int, int)> fn1 = std::cw<&add>;
    std::function<int(int, int)> fn2 = std::cw<&sub>;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }

    for (auto _ : s) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }
}

#if __cpp_lib_move_only_function >= 202110L
static void cw_cwonly_std_moveonly(picobench::state& s) {
    std::move_only_function<int(int, int) const> fn1 = std::cw<&add>;
    std::move_only_function<int(int, int) const> fn2 = std::cw<&sub>;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }

    for (auto _ : s) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }
}
#endif // C++ >= 23

#if __cpp_lib_copyable_function >= 202306L
static void cw_cwonly_std_copyable(picobench::state& s) {
    std::copyable_function<int(int, int) const> fn1 = std::cw<&add>;
    std::copyable_function<int(int, int) const> fn2 = std::cw<&sub>;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }

    for (auto _ : s) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }
}
#endif // C++ >= 26

static void cw_cwonly_ebd(picobench::state& s) {
    ebd::fn<int(int, int) const> fn1 = std::cw<&add>;
    ebd::fn<int(int, int) const> fn2 = std::cw<&sub>;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }

    for (auto _ : s) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }
}

static void cw_cwonly_fu2(picobench::state& s) {
    fu2::function<int(int, int) const> fn1 = std::cw<&add>;
    fu2::function<int(int, int) const> fn2 = std::cw<&sub>;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }

    for (auto _ : s) {
        volatile int res1 = fn1(0x111, 0x222); (void)res1;
        volatile int res2 = fn2(0x222, 0x111); (void)res2;
    }
}

static void cw_cwonly_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, int(int, int) const>
        ::restrict_layout<3 * sizeof(void*)>
        ::support_copy<pro::constraint_level::nontrivial>
        ::build;

    auto fn1 = pro::make_proxy_inplace<Invoker>(std::cw<&add>);
    auto fn2 = pro::make_proxy_inplace<Invoker>(std::cw<&sub>);

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = (*fn1)(0x111, 0x222); (void)res1;
        volatile int res2 = (*fn2)(0x222, 0x111); (void)res2;
    }

    for (auto _ : s) {
        volatile int res1 = (*fn1)(0x111, 0x222); (void)res1;
        volatile int res2 = (*fn2)(0x222, 0x111); (void)res2;
    }
}

BENCHMARK_UNIT(NTTP_Cw.CwOnly);

BENCHMARK_BASELINE(cw_cwonly_std);
#if __cpp_lib_move_only_function >= 202110L
BENCHMARK_NOTBASE(cw_cwonly_std_moveonly);
#endif // C++ >= 23
#if __cpp_lib_copyable_function >= 202306L
BENCHMARK_NOTBASE(cw_cwonly_std_copyable);
#endif // C++ >= 26
BENCHMARK_NOTBASE(cw_cwonly_ebd);
BENCHMARK_NOTBASE(cw_cwonly_fu2);
BENCHMARK_NOTBASE(cw_cwonly_pro);


// cw NTTP for member function

static void cw_nttp_member_std(picobench::state& s) {
    // `std::function` doesn't support nttp bind yet.
    C obj;
    std::function<int(const C&, int)> fn = &C::add;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }
}

#if __cpp_lib_move_only_function >= 202110L
static void cw_nttp_member_std_moveonly(picobench::state& s) {
    // `std::move_only_function` doesn't support nttp bind yet.
    C obj;
    std::move_only_function<int(const C&, int) const> fn = &C::add;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }
}
#endif // C++ >= 23

#if __cpp_lib_copyable_function >= 202306L
static void cw_nttp_member_std_copyable(picobench::state& s) {
    // `std::copyable_function` doesn't support nttp bind yet.
    C obj;
    std::copyable_function<int(const C&, int) const> fn = &C::add;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }
}
#endif // C++ >= 26

static void cw_nttp_member_ebd_normal(picobench::state& s) {
    C obj;
    ebd::fn<int(const C&, int) const> fn = &C::add;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }
}

static void cw_nttp_member_ebd_nttp(picobench::state& s) {
    ebd::fn<int(int) const> fn(std::cw<&C::add>, C{});

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn(0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = fn(0); (void)res1;
    }
}

static void cw_nttp_member_fu2(picobench::state& s) {
    // `fu2::function` doesn't support nttp bind yet.
    C obj;
    fu2::function<int(const C&, int) const> fn = &C::add;

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = fn(obj, 0); (void)res1;
    }
}

static void cw_nttp_member_pro(picobench::state& s) {
    // `pro::proxy` doesn't support nttp bind yet.
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, int(const C&, int) const>
        ::restrict_layout<3 * sizeof(void*)>
        ::support_copy<pro::constraint_level::nontrivial>
        ::build;

    C obj;
    auto fn = pro::make_proxy_inplace<Invoker>(std::mem_fn(&C::add));

    for (std::size_t i = 0; i < BENCHMARK_WARNUP; i++) {
        volatile int res1 = (*fn)(obj, 0); (void)res1;
    }

    for (auto _ : s) {
        volatile int res1 = (*fn)(obj, 0); (void)res1;
    }
}

BENCHMARK_UNIT(NTTP_Cw.MemberFunction);

BENCHMARK_BASELINE(cw_nttp_member_std);
#if __cpp_lib_move_only_function >= 202110L
BENCHMARK_NOTBASE(cw_nttp_member_std_moveonly);
#endif // C++ >= 23
#if __cpp_lib_copyable_function >= 202306L
BENCHMARK_NOTBASE(cw_nttp_member_std_copyable);
#endif // C++ >= 26
BENCHMARK_NOTBASE(cw_nttp_member_ebd_normal);
BENCHMARK_NOTBASE(cw_nttp_member_ebd_nttp);
BENCHMARK_NOTBASE(cw_nttp_member_fu2);
BENCHMARK_NOTBASE(cw_nttp_member_pro);

