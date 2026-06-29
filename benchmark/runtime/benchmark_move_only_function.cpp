#include "benchmark.hpp"

#include <string>
#include <vector>
#include <array>

// ----------------------------------------------------------------------------
// MoveOnlyFunction Parameter Types Test Suite
// ----------------------------------------------------------------------------

#if __cpp_lib_move_only_function >= 202110L

// ----------------------------------------------------------------------------
// Test 1: Basic Scalar Types (int, double, pointers)
// ----------------------------------------------------------------------------

static void scalar_params_std(picobench::state& s) {
    auto l = [](int a, double b, void* c) { 
        volatile int res = a + static_cast<int>(b);
        (void)c; (void)res; 
    };
    std::move_only_function<void(int, double, void*)> fn = l;

    for (auto _ : s) {
        fn(42, 3.14, nullptr);
    }
}

static void scalar_params_ebd(picobench::state& s) {
    auto l = [](int a, double b, void* c) { 
        volatile int res = a + static_cast<int>(b);
        (void)c; (void)res; 
    };
    ebd::unique_fn<void(int, double, void*)> fn = l;

    for (auto _ : s) {
        fn(42, 3.14, nullptr);
    }
}

static void scalar_params_fu2(picobench::state& s) {
    auto l = [](int a, double b, void* c) { 
        volatile int res = a + static_cast<int>(b);
        (void)c; (void)res; 
    };
    fu2::unique_function<void(int, double, void*)> fn = l;

    for (auto _ : s) {
        fn(42, 3.14, nullptr);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Scalar);
BENCHMARK_BASELINE(scalar_params_std);
BENCHMARK_NOTBASE(scalar_params_ebd);
BENCHMARK_NOTBASE(scalar_params_fu2);

// ----------------------------------------------------------------------------
// Test 2: Small Trivial Struct (register-passable candidate)
// ----------------------------------------------------------------------------

static void small_trivial_params_std(picobench::state& s) {
    auto l = [](benchmark_trivial_struct a, benchmark_trivial_struct b) { 
        volatile void* res = a.pod;
        (void)b; (void)res; 
    };
    std::move_only_function<void(benchmark_trivial_struct, benchmark_trivial_struct)> fn = l;
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj);
    }
}

static void small_trivial_params_ebd(picobench::state& s) {
    auto l = [](benchmark_trivial_struct a, benchmark_trivial_struct b) { 
        volatile void* res = a.pod;
        (void)b; (void)res; 
    };
    ebd::unique_fn<void(benchmark_trivial_struct, benchmark_trivial_struct)> fn = l;
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj);
    }
}

static void small_trivial_params_fu2(picobench::state& s) {
    auto l = [](benchmark_trivial_struct a, benchmark_trivial_struct b) { 
        volatile void* res = a.pod;
        (void)b; (void)res; 
    };
    fu2::unique_function<void(benchmark_trivial_struct, benchmark_trivial_struct)> fn = l;
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.SmallTrivial);
BENCHMARK_BASELINE(small_trivial_params_std);
BENCHMARK_NOTBASE(small_trivial_params_ebd);
BENCHMARK_NOTBASE(small_trivial_params_fu2);

// ----------------------------------------------------------------------------
// Test 3: Multiple Trivial Params (4 params)
// ----------------------------------------------------------------------------

static void multi_trivial_params_std(picobench::state& s) {
    auto l = [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b, 
                benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) { 
        volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; 
    };
    std::move_only_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct,
                                  benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn = l;
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj, obj, obj);
    }
}

static void multi_trivial_params_ebd(picobench::state& s) {
    auto l = [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b, 
                benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) { 
        volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; 
    };
    ebd::unique_fn<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct,
                         benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn = l;
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj, obj, obj);
    }
}

static void multi_trivial_params_fu2(picobench::state& s) {
    auto l = [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b, 
                benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) { 
        volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; 
    };
    fu2::unique_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct,
                               benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn = l;
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj, obj, obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.MultiTrivial);
BENCHMARK_BASELINE(multi_trivial_params_std);
BENCHMARK_NOTBASE(multi_trivial_params_ebd);
BENCHMARK_NOTBASE(multi_trivial_params_fu2);

// ----------------------------------------------------------------------------
// Test 4: Large Trivial Struct (not register-passable)
// ----------------------------------------------------------------------------

static void large_trivial_params_std(picobench::state& s) {
    auto l = [](benchmark_huge_trivial_struct a) { 
        volatile char c = a.huge[0];
        (void)c; 
    };
    std::move_only_function<void(benchmark_huge_trivial_struct)> fn = l;
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void large_trivial_params_ebd(picobench::state& s) {
    auto l = [](benchmark_huge_trivial_struct a) { 
        volatile char c = a.huge[0];
        (void)c; 
    };
    ebd::unique_fn<void(benchmark_huge_trivial_struct)> fn = l;
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void large_trivial_params_fu2(picobench::state& s) {
    auto l = [](benchmark_huge_trivial_struct a) { 
        volatile char c = a.huge[0];
        (void)c; 
    };
    fu2::unique_function<void(benchmark_huge_trivial_struct)> fn = l;
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.LargeTrivial);
BENCHMARK_BASELINE(large_trivial_params_std);
BENCHMARK_NOTBASE(large_trivial_params_ebd);
BENCHMARK_NOTBASE(large_trivial_params_fu2);

// ----------------------------------------------------------------------------
// Test 5: std::string Parameter
// ----------------------------------------------------------------------------

static void string_params_std(picobench::state& s) {
    auto l = [](const std::string& str) { 
        volatile size_t len = str.size();
        (void)len; 
    };
    std::move_only_function<void(const std::string&)> fn = l;
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        fn(test_str);
    }
}

static void string_params_ebd(picobench::state& s) {
    auto l = [](const std::string& str) { 
        volatile size_t len = str.size();
        (void)len; 
    };
    ebd::unique_fn<void(const std::string&)> fn = l;
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        fn(test_str);
    }
}

static void string_params_fu2(picobench::state& s) {
    auto l = [](const std::string& str) { 
        volatile size_t len = str.size();
        (void)len; 
    };
    fu2::unique_function<void(const std::string&)> fn = l;
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        fn(test_str);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.String);
BENCHMARK_BASELINE(string_params_std);
BENCHMARK_NOTBASE(string_params_ebd);
BENCHMARK_NOTBASE(string_params_fu2);

// ----------------------------------------------------------------------------
// Test 6: Mixed Parameter Types (scalar + trivial + non-trivial)
// ----------------------------------------------------------------------------

static void mixed_params_std(picobench::state& s) {
    auto l = [](int a, benchmark_trivial_struct b, const std::vector<int>& c) { 
        volatile int res = a + static_cast<int>(c.size());
        (void)b; (void)res; 
    };
    std::move_only_function<void(int, benchmark_trivial_struct, const std::vector<int>&)> fn = l;
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        fn(42, obj, vec);
    }
}

static void mixed_params_ebd(picobench::state& s) {
    auto l = [](int a, benchmark_trivial_struct b, const std::vector<int>& c) { 
        volatile int res = a + static_cast<int>(c.size());
        (void)b; (void)res; 
    };
    ebd::unique_fn<void(int, benchmark_trivial_struct, const std::vector<int>&)> fn = l;
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        fn(42, obj, vec);
    }
}

static void mixed_params_fu2(picobench::state& s) {
    auto l = [](int a, benchmark_trivial_struct b, const std::vector<int>& c) { 
        volatile int res = a + static_cast<int>(c.size());
        (void)b; (void)res; 
    };
    fu2::unique_function<void(int, benchmark_trivial_struct, const std::vector<int>&)> fn = l;
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        fn(42, obj, vec);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Mixed);
BENCHMARK_BASELINE(mixed_params_std);
BENCHMARK_NOTBASE(mixed_params_ebd);
BENCHMARK_NOTBASE(mixed_params_fu2);

// ----------------------------------------------------------------------------
// Test 7: Return Value (int)
// ----------------------------------------------------------------------------

static void return_int_std(picobench::state& s) {
    auto l = [](int a, int b) { return a + b; };
    std::move_only_function<int(int, int)> fn = l;

    for (auto _ : s) {
        volatile int res = fn(100, 200);
        (void)res;
    }
}

static void return_int_ebd(picobench::state& s) {
    auto l = [](int a, int b) { return a + b; };
    ebd::unique_fn<int(int, int)> fn = l;

    for (auto _ : s) {
        volatile int res = fn(100, 200);
        (void)res;
    }
}

static void return_int_fu2(picobench::state& s) {
    auto l = [](int a, int b) { return a + b; };
    fu2::unique_function<int(int, int)> fn = l;

    for (auto _ : s) {
        volatile int res = fn(100, 200);
        (void)res;
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Return.Int);
BENCHMARK_BASELINE(return_int_std);
BENCHMARK_NOTBASE(return_int_ebd);
BENCHMARK_NOTBASE(return_int_fu2);

// ----------------------------------------------------------------------------
// Test 8: Return Struct (by value)
// ----------------------------------------------------------------------------

struct return_struct {
    int a;
    double b;
    char c[16];
};

static void return_struct_std(picobench::state& s) {
    auto l = [](int x, double y) { 
        return_struct r{};
        r.a = x;
        r.b = y;
        return r; 
    };
    std::move_only_function<return_struct(int, double)> fn = l;

    for (auto _ : s) {
        volatile return_struct res = fn(42, 3.14);
        (void)res;
    }
}

static void return_struct_ebd(picobench::state& s) {
    auto l = [](int x, double y) { 
        return_struct r{};
        r.a = x;
        r.b = y;
        return r; 
    };
    ebd::unique_fn<return_struct(int, double)> fn = l;

    for (auto _ : s) {
        volatile return_struct res = fn(42, 3.14);
        (void)res;
    }
}

static void return_struct_fu2(picobench::state& s) {
    auto l = [](int x, double y) { 
        return_struct r{};
        r.a = x;
        r.b = y;
        return r; 
    };
    fu2::unique_function<return_struct(int, double)> fn = l;

    for (auto _ : s) {
        volatile return_struct res = fn(42, 3.14);
        (void)res;
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Return.Struct);
BENCHMARK_BASELINE(return_struct_std);
BENCHMARK_NOTBASE(return_struct_ebd);
BENCHMARK_NOTBASE(return_struct_fu2);

// ----------------------------------------------------------------------------
// Test 9: std::array Params (fixed size)
// ----------------------------------------------------------------------------

static void array_params_std(picobench::state& s) {
    auto l = [](std::array<int, 4> arr) { 
        volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
        (void)sum; 
    };
    std::move_only_function<void(std::array<int, 4>)> fn = l;
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        fn(arr);
    }
}

static void array_params_ebd(picobench::state& s) {
    auto l = [](std::array<int, 4> arr) { 
        volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
        (void)sum; 
    };
    ebd::unique_fn<void(std::array<int, 4>)> fn = l;
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        fn(arr);
    }
}

static void array_params_fu2(picobench::state& s) {
    auto l = [](std::array<int, 4> arr) { 
        volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
        (void)sum; 
    };
    fu2::unique_function<void(std::array<int, 4>)> fn = l;
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        fn(arr);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Array);
BENCHMARK_BASELINE(array_params_std);
BENCHMARK_NOTBASE(array_params_ebd);
BENCHMARK_NOTBASE(array_params_fu2);

// ----------------------------------------------------------------------------
// Test 10: Copy Hard Struct (non-trivial)
// ----------------------------------------------------------------------------

static void copy_hard_params_std(picobench::state& s) {
    auto l = [](benchmark_copy_hard_struct a) { 
        volatile int dummy = 0;
        (void)a; (void)dummy; 
    };
    std::move_only_function<void(benchmark_copy_hard_struct)> fn = l;
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void copy_hard_params_ebd(picobench::state& s) {
    auto l = [](benchmark_copy_hard_struct a) { 
        volatile int dummy = 0;
        (void)a; (void)dummy; 
    };
    ebd::unique_fn<void(benchmark_copy_hard_struct)> fn = l;
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void copy_hard_params_fu2(picobench::state& s) {
    auto l = [](benchmark_copy_hard_struct a) { 
        volatile int dummy = 0;
        (void)a; (void)dummy; 
    };
    fu2::unique_function<void(benchmark_copy_hard_struct)> fn = l;
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.CopyHard);
BENCHMARK_BASELINE(copy_hard_params_std);
BENCHMARK_NOTBASE(copy_hard_params_ebd);
BENCHMARK_NOTBASE(copy_hard_params_fu2);

// ----------------------------------------------------------------------------
// Test 11: Noexcept Qualified Function
// ----------------------------------------------------------------------------

static void noexcept_params_std(picobench::state& s) {
    auto l = [](int a, double b) noexcept { 
        volatile int res = a + static_cast<int>(b);
        (void)res; 
    };
    std::move_only_function<void(int, double) noexcept> fn = l;

    for (auto _ : s) {
        fn(42, 3.14);
    }
}

static void noexcept_params_ebd(picobench::state& s) {
    auto l = [](int a, double b) noexcept { 
        volatile int res = a + static_cast<int>(b);
        (void)res; 
    };
    ebd::unique_fn<void(int, double) noexcept> fn = l;

    for (auto _ : s) {
        fn(42, 3.14);
    }
}

static void noexcept_params_fu2(picobench::state& s) {
    auto l = [](int a, double b) noexcept { 
        volatile int res = a + static_cast<int>(b);
        (void)res; 
    };
    fu2::unique_function<void(int, double) noexcept> fn = l;

    for (auto _ : s) {
        fn(42, 3.14);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Noexcept);
BENCHMARK_BASELINE(noexcept_params_std);
BENCHMARK_NOTBASE(noexcept_params_ebd);
BENCHMARK_NOTBASE(noexcept_params_fu2);

// ----------------------------------------------------------------------------
// Test 12: Const Qualified Function
// ----------------------------------------------------------------------------

static void const_params_std(picobench::state& s) {
    auto l = [](int a, int b) { return a * b; };
    std::move_only_function<int(int, int) const> fn = l;

    for (auto _ : s) {
        volatile int res = fn(10, 20);
        (void)res;
    }
}

static void const_params_ebd(picobench::state& s) {
    auto l = [](int a, int b) { return a * b; };
    ebd::unique_fn<int(int, int) const> fn = l;

    for (auto _ : s) {
        volatile int res = fn(10, 20);
        (void)res;
    }
}

static void const_params_fu2(picobench::state& s) {
    auto l = [](int a, int b) { return a * b; };
    fu2::unique_function<int(int, int) const> fn = l;

    for (auto _ : s) {
        volatile int res = fn(10, 20);
        (void)res;
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Const);
BENCHMARK_BASELINE(const_params_std);
BENCHMARK_NOTBASE(const_params_ebd);
BENCHMARK_NOTBASE(const_params_fu2);

#endif
