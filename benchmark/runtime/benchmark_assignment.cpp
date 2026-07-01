#include "benchmark.hpp"

#include <functional>
#include <string>
#include <vector>
#include <array>
#include <utility>

// ----------------------------------------------------------------------------
// AssignmentBenchmark.CopyAssignment - Small Trivial Lambda
// ----------------------------------------------------------------------------

static void copy_assign_small_trivial_std(picobench::state& s) {
    auto a1 = std::function<int(int)>([](int x) { return x + 1; });
    auto a2 = std::function<int(int)>([](int x) { return x + 2; });
    auto b = std::function<int(int)>();
    int result = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void copy_assign_small_trivial_ebd(picobench::state& s) {
    auto a1 = ebd::fn<int(int)>([](int x) { return x + 1; });
    auto a2 = ebd::fn<int(int)>([](int x) { return x + 2; });
    auto b = ebd::fn<int(int)>();
    int result = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void copy_assign_small_trivial_fu2(picobench::state& s) {
    auto a1 = fu2::function<int(int)>([](int x) { return x + 1; });
    auto a2 = fu2::function<int(int)>([](int x) { return x + 2; });
    auto b = fu2::function<int(int)>();
    int result = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.CopyAssignmentSmallTrivial);
BENCHMARK_BASELINE(copy_assign_small_trivial_std);
BENCHMARK_NOTBASE(copy_assign_small_trivial_ebd);
BENCHMARK_NOTBASE(copy_assign_small_trivial_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.MoveAssignment - Small Trivial Lambda
// ----------------------------------------------------------------------------

static void move_assign_small_trivial_std(picobench::state& s) {
    auto b = std::function<int(int)>();
    int result = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = std::function<int(int)>([](int x) { return x + 1; });
        auto a2 = std::function<int(int)>([](int x) { return x + 2; });
        b = std::move(toggle ? a1 : a2);
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void move_assign_small_trivial_ebd(picobench::state& s) {
    auto b = ebd::fn<int(int)>();
    int result = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = ebd::fn<int(int)>([](int x) { return x + 1; });
        auto a2 = ebd::fn<int(int)>([](int x) { return x + 2; });
        b = std::move(toggle ? a1 : a2);
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void move_assign_small_trivial_fu2(picobench::state& s) {
    auto b = fu2::function<int(int)>();
    int result = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = fu2::function<int(int)>([](int x) { return x + 1; });
        auto a2 = fu2::function<int(int)>([](int x) { return x + 2; });
        b = std::move(toggle ? a1 : a2);
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.MoveAssignmentSmallTrivial);
BENCHMARK_BASELINE(move_assign_small_trivial_std);
BENCHMARK_NOTBASE(move_assign_small_trivial_ebd);
BENCHMARK_NOTBASE(move_assign_small_trivial_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.CopyAssignment - Large Capture Lambda
// ----------------------------------------------------------------------------

struct LargeCaptureData {
    std::string str;
    std::array<int, 1> numbers{};
    
    LargeCaptureData() : str("Benchmark test string for assignment performance testing") {}
};

static void copy_assign_large_capture_std(picobench::state& s) {
    LargeCaptureData data1, data2;
    auto a1 = std::function<size_t()>([data1]() { return data1.numbers.size() + data1.str.size(); });
    auto a2 = std::function<size_t()>([data2]() { return data2.numbers.size() + data2.str.size() + 1; });
    auto b = std::function<size_t()>();
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        auto* volatile ff = &b;
        volatile size_t u = (*ff)(); (void)u;
    }
}

// BUG
static void copy_assign_large_capture_ebd(picobench::state& s) {
    LargeCaptureData data1, data2;
    auto a1 = ebd::make_fn([data1]() { return data1.numbers.size() + data1.str.size(); });
    auto a2 = ebd::make_fn([data2]() { return data2.numbers.size() + data2.str.size() + 1; });
    decltype(a1) b;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        auto* volatile ff = &b;
        volatile size_t u = (*ff)(); (void)u;
    }
}

static void copy_assign_large_capture_fu2(picobench::state& s) {
    LargeCaptureData data1, data2;
    auto a1 = fu2::function<size_t()>([data1]() { return data1.numbers.size() + data1.str.size(); });
    auto a2 = fu2::function<size_t()>([data2]() { return data2.numbers.size() + data2.str.size() + 1; });
    auto b = fu2::function<size_t()>();
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        auto* volatile ff = &b;
        volatile size_t u = (*ff)(); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.CopyAssignmentLargeCapture);
BENCHMARK_BASELINE(copy_assign_large_capture_std);
BENCHMARK_NOTBASE(copy_assign_large_capture_ebd);
BENCHMARK_NOTBASE(copy_assign_large_capture_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.MoveAssignment - Large Capture Lambda
// ----------------------------------------------------------------------------

static void move_assign_large_capture_std(picobench::state& s) {
    auto b = std::function<size_t()>();
    size_t total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        LargeCaptureData data1, data2;
        auto a1 = std::function<size_t()>([data1]() { return data1.numbers.size() + data1.str.size(); });
        auto a2 = std::function<size_t()>([data2]() { return data2.numbers.size() + data2.str.size() + 1; });
        b = std::move(toggle ? a1 : a2);
        total += b();
        auto* volatile ff = &b;
        volatile size_t u = (*ff)(); (void)u;
        (void)total;
    }
}

static void move_assign_large_capture_ebd(picobench::state& s) {
    LargeCaptureData dummy_data;
    auto dummy = ebd::make_fn([dummy_data]() { return dummy_data.numbers.size(); });
    decltype(dummy) b;
    size_t total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        LargeCaptureData data1, data2;
        auto a1 = ebd::make_fn([data1]() { return data1.numbers.size() + data1.str.size(); });
        auto a2 = ebd::make_fn([data2]() { return data2.numbers.size() + data2.str.size() + 1; });
        b = std::move(toggle ? a1 : a2);
        total += b();
        auto* volatile ff = &b;
        volatile size_t u = (*ff)(); (void)u;
        (void)total;
    }
}

static void move_assign_large_capture_fu2(picobench::state& s) {
    auto b = fu2::function<size_t()>();
    size_t total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        LargeCaptureData data1, data2;
        auto a1 = fu2::function<size_t()>([data1]() { return data1.numbers.size() + data1.str.size(); });
        auto a2 = fu2::function<size_t()>([data2]() { return data2.numbers.size() + data2.str.size() + 1; });
        b = std::move(toggle ? a1 : a2);
        total += b();
        auto* volatile ff = &b;
        volatile size_t u = (*ff)(); (void)u;
        (void)total;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.MoveAssignmentLargeCapture);
BENCHMARK_BASELINE(move_assign_large_capture_std);
BENCHMARK_NOTBASE(move_assign_large_capture_ebd);
BENCHMARK_NOTBASE(move_assign_large_capture_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.CopyAssignment - Stateless Lambda (Small Buffer Optimization)
// ----------------------------------------------------------------------------

static void copy_assign_stateless_std(picobench::state& s) {
    auto a1 = std::function<int(int, int)>(std::plus<int>{});
    auto a2 = std::function<int(int, int)>(std::minus<int>{});
    auto b = std::function<int(int, int)>();
    int total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        total = b(total, 1);
        auto* volatile ff = &b;
        volatile int u = (*ff)(total, 1); (void)u;
    }
}

static void copy_assign_stateless_ebd(picobench::state& s) {
    auto a1 = ebd::fn<int(int, int)>(std::plus<int>{});
    auto a2 = ebd::fn<int(int, int)>(std::minus<int>{});
    auto b = ebd::fn<int(int, int)>();
    int total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        total = b(total, 1);
        auto* volatile ff = &b;
        volatile int u = (*ff)(total, 1); (void)u;
    }
}

static void copy_assign_stateless_fu2(picobench::state& s) {
    auto a1 = fu2::function<int(int, int)>(std::plus<int>{});
    auto a2 = fu2::function<int(int, int)>(std::minus<int>{});
    auto b = fu2::function<int(int, int)>();
    int total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        total = b(total, 1);
        auto* volatile ff = &b;
        volatile int u = (*ff)(total, 1); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.CopyAssignmentStateless);
BENCHMARK_BASELINE(copy_assign_stateless_std);
BENCHMARK_NOTBASE(copy_assign_stateless_ebd);
BENCHMARK_NOTBASE(copy_assign_stateless_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.MoveAssignment - Stateless Lambda (Small Buffer Optimization)
// ----------------------------------------------------------------------------

static void move_assign_stateless_std(picobench::state& s) {
    auto b = std::function<int(int, int)>();
    int total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = std::function<int(int, int)>(std::plus<int>{});
        auto a2 = std::function<int(int, int)>(std::minus<int>{});
        b = std::move(toggle ? a1 : a2);
        total = b(total, 1);
        auto* volatile ff = &b;
        volatile int u = (*ff)(total, 1); (void)u;
    }
}

static void move_assign_stateless_ebd(picobench::state& s) {
    auto b = ebd::fn<int(int, int)>();
    int total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = ebd::fn<int(int, int)>(std::plus<int>{});
        auto a2 = ebd::fn<int(int, int)>(std::minus<int>{});
        b = std::move(toggle ? a1 : a2);
        total = b(total, 1);
        auto* volatile ff = &b;
        volatile int u = (*ff)(total, 1); (void)u;
    }
}

static void move_assign_stateless_fu2(picobench::state& s) {
    auto b = fu2::function<int(int, int)>();
    int total = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = fu2::function<int(int, int)>(std::plus<int>{});
        auto a2 = fu2::function<int(int, int)>(std::minus<int>{});
        b = std::move(toggle ? a1 : a2);
        total = b(total, 1);
        auto* volatile ff = &b;
        volatile int u = (*ff)(total, 1); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.MoveAssignmentStateless);
BENCHMARK_BASELINE(move_assign_stateless_std);
BENCHMARK_NOTBASE(move_assign_stateless_ebd);
BENCHMARK_NOTBASE(move_assign_stateless_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.CopyAssignment - Function Pointer
// ----------------------------------------------------------------------------

static int bench_func1(int x) { return x * 2; }
static int bench_func2(int x) { return x * 3; }

static void copy_assign_func_ptr_std(picobench::state& s) {
    auto a1 = std::function<int(int)>(bench_func1);
    auto a2 = std::function<int(int)>(bench_func2);
    auto b = std::function<int(int)>(bench_func1);
    int result = 1;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void copy_assign_func_ptr_ebd(picobench::state& s) {
    auto a1 = ebd::fn<int(int)>(bench_func1);
    auto a2 = ebd::fn<int(int)>(bench_func2);
    auto b = ebd::fn<int(int)>(bench_func1);
    int result = 1;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void copy_assign_func_ptr_fu2(picobench::state& s) {
    auto a1 = fu2::function<int(int)>(bench_func1);
    auto a2 = fu2::function<int(int)>(bench_func2);
    auto b = fu2::function<int(int)>(bench_func1);
    int result = 1;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.CopyAssignmentFuncPtr);
BENCHMARK_BASELINE(copy_assign_func_ptr_std);
BENCHMARK_NOTBASE(copy_assign_func_ptr_ebd);
BENCHMARK_NOTBASE(copy_assign_func_ptr_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.MoveAssignment - Function Pointer
// ----------------------------------------------------------------------------

static void move_assign_func_ptr_std(picobench::state& s) {
    auto b = std::function<int(int)>(bench_func1);
    int result = 1;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = std::function<int(int)>(bench_func1);
        auto a2 = std::function<int(int)>(bench_func2);
        b = std::move(toggle ? a1 : a2);
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void move_assign_func_ptr_ebd(picobench::state& s) {
    auto b = ebd::fn<int(int)>(bench_func1);
    int result = 1;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = ebd::fn<int(int)>(bench_func1);
        auto a2 = ebd::fn<int(int)>(bench_func2);
        b = std::move(toggle ? a1 : a2);
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

static void move_assign_func_ptr_fu2(picobench::state& s) {
    auto b = fu2::function<int(int)>(bench_func1);
    int result = 1;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = fu2::function<int(int)>(bench_func1);
        auto a2 = fu2::function<int(int)>(bench_func2);
        b = std::move(toggle ? a1 : a2);
        result = b(result);
        auto* volatile ff = &b;
        volatile int u = (*ff)(result); (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.MoveAssignmentFuncPtr);
BENCHMARK_BASELINE(move_assign_func_ptr_std);
BENCHMARK_NOTBASE(move_assign_func_ptr_ebd);
BENCHMARK_NOTBASE(move_assign_func_ptr_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.CopyAssignment - Between Same Type
// ----------------------------------------------------------------------------

static void copy_assign_same_type_std(picobench::state& s) {
    auto a1 = std::function<void()>([]() {});
    auto a2 = std::function<void()>([]() {});
    auto b = std::function<void()>([]() {});
    int dummy = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        b();
        dummy++;
        auto* volatile ff = &b;
        (*ff)();
        volatile int u = dummy; (void)u;
    }
}

static void copy_assign_same_type_ebd(picobench::state& s) {
    auto a1 = ebd::fn<void()>([]() {});
    auto a2 = ebd::fn<void()>([]() {});
    auto b = ebd::fn<void()>([]() {});
    int dummy = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        b();
        dummy++;
        auto* volatile ff = &b;
        (*ff)();
        volatile int u = dummy; (void)u;
    }
}

static void copy_assign_same_type_fu2(picobench::state& s) {
    auto a1 = fu2::function<void()>([]() {});
    auto a2 = fu2::function<void()>([]() {});
    auto b = fu2::function<void()>([]() {});
    int dummy = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        b = toggle ? a1 : a2;
        b();
        dummy++;
        auto* volatile ff = &b;
        (*ff)();
        volatile int u = dummy; (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.CopyAssignmentSameType);
BENCHMARK_BASELINE(copy_assign_same_type_std);
BENCHMARK_NOTBASE(copy_assign_same_type_ebd);
BENCHMARK_NOTBASE(copy_assign_same_type_fu2);

// ----------------------------------------------------------------------------
// AssignmentBenchmark.MoveAssignment - Between Same Type
// ----------------------------------------------------------------------------

static void move_assign_same_type_std(picobench::state& s) {
    auto b = std::function<void()>([]() {});
    int dummy = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = std::function<void()>([]() {});
        auto a2 = std::function<void()>([]() {});
        b = std::move(toggle ? a1 : a2);
        b();
        dummy++;
        auto* volatile ff = &b;
        (*ff)();
        volatile int u = dummy; (void)u;
    }
}

static void move_assign_same_type_ebd(picobench::state& s) {
    auto b = ebd::fn<void()>([]() {});
    int dummy = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = ebd::fn<void()>([]() {});
        auto a2 = ebd::fn<void()>([]() {});
        b = std::move(toggle ? a1 : a2);
        b();
        dummy++;
        auto* volatile ff = &b;
        (*ff)();
        volatile int u = dummy; (void)u;
    }
}

static void move_assign_same_type_fu2(picobench::state& s) {
    auto b = fu2::function<void()>([]() {});
    int dummy = 0;
    bool toggle = false;

    for (auto _ : s) {
        toggle = !toggle;
        auto a1 = fu2::function<void()>([]() {});
        auto a2 = fu2::function<void()>([]() {});
        b = std::move(toggle ? a1 : a2);
        b();
        dummy++;
        auto* volatile ff = &b;
        (*ff)();
        volatile int u = dummy; (void)u;
    }
}

BENCHMARK_UNIT(AssignmentBenchmark.MoveAssignmentSameType);
BENCHMARK_BASELINE(move_assign_same_type_std);
BENCHMARK_NOTBASE(move_assign_same_type_ebd);
BENCHMARK_NOTBASE(move_assign_same_type_fu2);
