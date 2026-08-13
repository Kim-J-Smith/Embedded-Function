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
    std::move_only_function<void(int, double, void*)> fn = 
        [](int a, double b, void* c) { 
            volatile int res = a + static_cast<int>(b);
            (void)c; (void)res; 
        };

    for (auto _ : s) {
        fn(42, 3.14, nullptr);
    }
}

static void scalar_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(int, double, void*)> fn = 
        [](int a, double b, void* c) { 
            volatile int res = a + static_cast<int>(b);
            (void)c; (void)res; 
        };

    for (auto _ : s) {
        fn(42, 3.14, nullptr);
    }
}

static void scalar_params_fu2(picobench::state& s) {
    fu2::unique_function<void(int, double, void*)> fn =
        [](int a, double b, void* c) {
            volatile int res = a + static_cast<int>(b);
            (void)c; (void)res;
        };

    for (auto _ : s) {
        fn(42, 3.14, nullptr);
    }
}

static void scalar_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(int, double, void*)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](int a, double b, void* c) {
            volatile int res = a + static_cast<int>(b);
            (void)c; (void)res;
        });

    for (auto _ : s) {
        (*fn)(42, 3.14, nullptr);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Scalar);
BENCHMARK_BASELINE(scalar_params_std);
BENCHMARK_NOTBASE(scalar_params_ebd);
BENCHMARK_NOTBASE(scalar_params_fu2);
BENCHMARK_NOTBASE(scalar_params_pro);

// ----------------------------------------------------------------------------
// Test 2: Small Trivial Struct (register-passable candidate)
// ----------------------------------------------------------------------------

static void small_trivial_params_std(picobench::state& s) {
    std::move_only_function<void(benchmark_trivial_struct, benchmark_trivial_struct)> fn = 
        [](benchmark_trivial_struct a, benchmark_trivial_struct b) { 
            void* volatile res = a.pod;
            (void)b; (void)res; 
        };
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj);
    }
}

static void small_trivial_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(benchmark_trivial_struct, benchmark_trivial_struct)> fn = 
        [](benchmark_trivial_struct a, benchmark_trivial_struct b) { 
            void* volatile res = a.pod;
            (void)b; (void)res; 
        };
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj);
    }
}

static void small_trivial_params_fu2(picobench::state& s) {
    fu2::unique_function<void(benchmark_trivial_struct, benchmark_trivial_struct)> fn =
        [](benchmark_trivial_struct a, benchmark_trivial_struct b) {
            void* volatile res = a.pod;
            (void)b; (void)res;
        };
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj);
    }
}

static void small_trivial_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(
            benchmark_trivial_struct, benchmark_trivial_struct)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](benchmark_trivial_struct a, benchmark_trivial_struct b) {
            void* volatile res = a.pod;
            (void)b; (void)res;
        });
    benchmark_trivial_struct obj{};

    for (auto _ : s) {
        (*fn)(obj, obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.SmallTrivial);
BENCHMARK_BASELINE(small_trivial_params_std);
BENCHMARK_NOTBASE(small_trivial_params_ebd);
BENCHMARK_NOTBASE(small_trivial_params_fu2);
BENCHMARK_NOTBASE(small_trivial_params_pro);

// ----------------------------------------------------------------------------
// Test 3: Multiple Trivial Params (4 params)
// ----------------------------------------------------------------------------

static void multi_trivial_params_std(picobench::state& s) {
    std::move_only_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct,
                                  benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn = 
        [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b, 
           benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) { 
            volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; (void)unused;
        };
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj, obj, obj);
    }
}

static void multi_trivial_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct,
                         benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn = 
        [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b, 
           benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) { 
            volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; (void)unused;
        };
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj, obj, obj);
    }
}

static void multi_trivial_params_fu2(picobench::state& s) {
    fu2::unique_function<void(benchmark_call_trivial_struct, benchmark_call_trivial_struct,
                              benchmark_call_trivial_struct, benchmark_call_trivial_struct)> fn =
        [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b,
           benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) {
            volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; (void)unused;
        };
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj, obj, obj, obj);
    }
}

static void multi_trivial_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(
            benchmark_call_trivial_struct, benchmark_call_trivial_struct,
            benchmark_call_trivial_struct, benchmark_call_trivial_struct)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](benchmark_call_trivial_struct a, benchmark_call_trivial_struct b,
           benchmark_call_trivial_struct c, benchmark_call_trivial_struct d) {
            volatile int unused = 1; (void)a; (void)b; (void)c; (void)d; (void)unused;
        });
    benchmark_call_trivial_struct obj{};

    for (auto _ : s) {
        (*fn)(obj, obj, obj, obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.MultiTrivial);
BENCHMARK_BASELINE(multi_trivial_params_std);
BENCHMARK_NOTBASE(multi_trivial_params_ebd);
BENCHMARK_NOTBASE(multi_trivial_params_fu2);
BENCHMARK_NOTBASE(multi_trivial_params_pro);

// ----------------------------------------------------------------------------
// Test 4: Large Trivial Struct (not register-passable)
// ----------------------------------------------------------------------------

static void large_trivial_params_std(picobench::state& s) {
    std::move_only_function<void(benchmark_huge_trivial_struct)> fn = 
        [](benchmark_huge_trivial_struct a) { 
            volatile char c = a.huge[0];
            (void)c; 
        };
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void large_trivial_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(benchmark_huge_trivial_struct)> fn = 
        [](benchmark_huge_trivial_struct a) { 
            volatile char c = a.huge[0];
            (void)c; 
        };
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void large_trivial_params_fu2(picobench::state& s) {
    fu2::unique_function<void(benchmark_huge_trivial_struct)> fn =
        [](benchmark_huge_trivial_struct a) {
            volatile char c = a.huge[0];
            (void)c;
        };
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void large_trivial_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(benchmark_huge_trivial_struct)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](benchmark_huge_trivial_struct a) {
            volatile char c = a.huge[0];
            (void)c;
        });
    benchmark_huge_trivial_struct obj{};

    for (auto _ : s) {
        (*fn)(obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.LargeTrivial);
BENCHMARK_BASELINE(large_trivial_params_std);
BENCHMARK_NOTBASE(large_trivial_params_ebd);
BENCHMARK_NOTBASE(large_trivial_params_fu2);
BENCHMARK_NOTBASE(large_trivial_params_pro);

// ----------------------------------------------------------------------------
// Test 5: std::string Parameter
// ----------------------------------------------------------------------------

static void string_params_std(picobench::state& s) {
    std::move_only_function<void(const std::string&)> fn = 
        [](const std::string& str) { 
            volatile size_t len = str.size();
            (void)len; 
        };
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        fn(test_str);
    }
}

static void string_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(const std::string&)> fn = 
        [](const std::string& str) { 
            volatile size_t len = str.size();
            (void)len; 
        };
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        fn(test_str);
    }
}

static void string_params_fu2(picobench::state& s) {
    fu2::unique_function<void(const std::string&)> fn =
        [](const std::string& str) {
            volatile size_t len = str.size();
            (void)len;
        };
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        fn(test_str);
    }
}

static void string_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(const std::string&)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](const std::string& str) {
            volatile size_t len = str.size();
            (void)len;
        });
    std::string test_str = "Hello, World! This is a test string for benchmarking.";

    for (auto _ : s) {
        (*fn)(test_str);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.String);
BENCHMARK_BASELINE(string_params_std);
BENCHMARK_NOTBASE(string_params_ebd);
BENCHMARK_NOTBASE(string_params_fu2);
BENCHMARK_NOTBASE(string_params_pro);

// ----------------------------------------------------------------------------
// Test 6: Mixed Parameter Types (scalar + trivial + non-trivial)
// ----------------------------------------------------------------------------

static void mixed_params_std(picobench::state& s) {
    std::move_only_function<void(int, benchmark_trivial_struct, const std::vector<int>&)> fn = 
        [](int a, benchmark_trivial_struct b, const std::vector<int>& c) { 
            volatile int res = a + static_cast<int>(c.size());
            (void)b; (void)res; 
        };
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        fn(42, obj, vec);
    }
}

static void mixed_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(int, benchmark_trivial_struct, const std::vector<int>&)> fn = 
        [](int a, benchmark_trivial_struct b, const std::vector<int>& c) { 
            volatile int res = a + static_cast<int>(c.size());
            (void)b; (void)res; 
        };
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        fn(42, obj, vec);
    }
}

static void mixed_params_fu2(picobench::state& s) {
    fu2::unique_function<void(int, benchmark_trivial_struct, const std::vector<int>&)> fn =
        [](int a, benchmark_trivial_struct b, const std::vector<int>& c) {
            volatile int res = a + static_cast<int>(c.size());
            (void)b; (void)res;
        };
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        fn(42, obj, vec);
    }
}

static void mixed_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(
            int, benchmark_trivial_struct, const std::vector<int>&)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](int a, benchmark_trivial_struct b, const std::vector<int>& c) {
            volatile int res = a + static_cast<int>(c.size());
            (void)b; (void)res;
        });
    benchmark_trivial_struct obj{};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto _ : s) {
        (*fn)(42, obj, vec);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Mixed);
BENCHMARK_BASELINE(mixed_params_std);
BENCHMARK_NOTBASE(mixed_params_ebd);
BENCHMARK_NOTBASE(mixed_params_fu2);
BENCHMARK_NOTBASE(mixed_params_pro);

// ----------------------------------------------------------------------------
// Test 7: Return Value (int)
// ----------------------------------------------------------------------------

static void return_int_std(picobench::state& s) {
    std::move_only_function<int(int, int)> fn = 
        [](int a, int b) { return a + b; };

    for (auto _ : s) {
        volatile int res = fn(100, 200);
        (void)res;
    }
}

static void return_int_ebd(picobench::state& s) {
    ebd::unique_fn<int(int, int)> fn = 
        [](int a, int b) { return a + b; };

    for (auto _ : s) {
        volatile int res = fn(100, 200);
        (void)res;
    }
}

static void return_int_fu2(picobench::state& s) {
    fu2::unique_function<int(int, int)> fn =
        [](int a, int b) { return a + b; };

    for (auto _ : s) {
        volatile int res = fn(100, 200);
        (void)res;
    }
}

static void return_int_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, int(int, int)>
        ::restrict_layout<ebd::unique_fn<int()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](int a, int b) { return a + b; });

    for (auto _ : s) {
        volatile int res = (*fn)(100, 200);
        (void)res;
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Return.Int);
BENCHMARK_BASELINE(return_int_std);
BENCHMARK_NOTBASE(return_int_ebd);
BENCHMARK_NOTBASE(return_int_fu2);
BENCHMARK_NOTBASE(return_int_pro);

// ----------------------------------------------------------------------------
// Test 8: Return Struct (by value)
// ----------------------------------------------------------------------------

struct return_struct {
    int a;
    double b;
    char c[16];
};

static void return_struct_std(picobench::state& s) {
    std::move_only_function<return_struct(int, double)> fn = 
        [](int x, double y) { 
            return_struct r{};
            r.a = x;
            r.b = y;
            return r; 
        };

    for (auto _ : s) {
        volatile return_struct res = fn(42, 3.14);
        (void)res;
    }
}

static void return_struct_ebd(picobench::state& s) {
    ebd::unique_fn<return_struct(int, double)> fn = 
        [](int x, double y) { 
            return_struct r{};
            r.a = x;
            r.b = y;
            return r; 
        };

    for (auto _ : s) {
        volatile return_struct res = fn(42, 3.14);
        (void)res;
    }
}

static void return_struct_fu2(picobench::state& s) {
    fu2::unique_function<return_struct(int, double)> fn =
        [](int x, double y) {
            return_struct r{};
            r.a = x;
            r.b = y;
            return r;
        };

    for (auto _ : s) {
        volatile return_struct res = fn(42, 3.14);
        (void)res;
    }
}

static void return_struct_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, return_struct(int, double)>
        ::restrict_layout<ebd::unique_fn<return_struct()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](int x, double y) {
            return_struct r{};
            r.a = x;
            r.b = y;
            return r;
        });

    for (auto _ : s) {
        volatile return_struct res = (*fn)(42, 3.14);
        (void)res;
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Return.Struct);
BENCHMARK_BASELINE(return_struct_std);
BENCHMARK_NOTBASE(return_struct_ebd);
BENCHMARK_NOTBASE(return_struct_fu2);
BENCHMARK_NOTBASE(return_struct_pro);

// ----------------------------------------------------------------------------
// Test 9: std::array Params (fixed size)
// ----------------------------------------------------------------------------

static void array_params_std(picobench::state& s) {
    std::move_only_function<void(std::array<int, 4>)> fn = 
        [](std::array<int, 4> arr) { 
            volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
            (void)sum; 
        };
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        fn(arr);
    }
}

static void array_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(std::array<int, 4>)> fn = 
        [](std::array<int, 4> arr) { 
            volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
            (void)sum; 
        };
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        fn(arr);
    }
}

static void array_params_fu2(picobench::state& s) {
    fu2::unique_function<void(std::array<int, 4>)> fn =
        [](std::array<int, 4> arr) {
            volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
            (void)sum;
        };
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        fn(arr);
    }
}

static void array_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(std::array<int, 4>)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](std::array<int, 4> arr) {
            volatile int sum = arr[0] + arr[1] + arr[2] + arr[3];
            (void)sum;
        });
    std::array<int, 4> arr = {1, 2, 3, 4};

    for (auto _ : s) {
        (*fn)(arr);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Array);
BENCHMARK_BASELINE(array_params_std);
BENCHMARK_NOTBASE(array_params_ebd);
BENCHMARK_NOTBASE(array_params_fu2);
BENCHMARK_NOTBASE(array_params_pro);

// ----------------------------------------------------------------------------
// Test 10: Copy Hard Struct (non-trivial)
// ----------------------------------------------------------------------------

static void copy_hard_params_std(picobench::state& s) {
    std::move_only_function<void(benchmark_copy_hard_struct)> fn = 
        [](benchmark_copy_hard_struct a) { 
            volatile int dummy = 0;
            (void)a; (void)dummy; 
        };
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void copy_hard_params_ebd(picobench::state& s) {
    ebd::unique_fn<void(benchmark_copy_hard_struct)> fn = 
        [](benchmark_copy_hard_struct a) { 
            volatile int dummy = 0;
            (void)a; (void)dummy; 
        };
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void copy_hard_params_fu2(picobench::state& s) {
    fu2::unique_function<void(benchmark_copy_hard_struct)> fn =
        [](benchmark_copy_hard_struct a) {
            volatile int dummy = 0;
            (void)a; (void)dummy;
        };
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        fn(obj);
    }
}

static void copy_hard_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(benchmark_copy_hard_struct)>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](benchmark_copy_hard_struct a) {
            volatile int dummy = 0;
            (void)a; (void)dummy;
        });
    benchmark_copy_hard_struct obj{};

    for (auto _ : s) {
        (*fn)(obj);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.CopyHard);
BENCHMARK_BASELINE(copy_hard_params_std);
BENCHMARK_NOTBASE(copy_hard_params_ebd);
BENCHMARK_NOTBASE(copy_hard_params_fu2);
BENCHMARK_NOTBASE(copy_hard_params_pro);

// ----------------------------------------------------------------------------
// Test 11: Noexcept Qualified Function
// ----------------------------------------------------------------------------

static void noexcept_params_std(picobench::state& s) {
    std::move_only_function<void(int, double) noexcept> fn = 
        [](int a, double b) noexcept { 
            volatile int res = a + static_cast<int>(b);
            (void)res; 
        };

    for (auto _ : s) {
        fn(42, 3.14);
    }
}

static void noexcept_params_ebd(picobench::state& s) {
    ebd::basic_fn<void(int, double) noexcept, 2*sizeof(void*), false, false, false, false> fn = 
        [](int a, double b) noexcept { 
            volatile int res = a + static_cast<int>(b);
            (void)res; 
        };

    for (auto _ : s) {
        fn(42, 3.14);
    }
}

static void noexcept_params_fu2(picobench::state& s) {
    fu2::unique_function<void(int, double) noexcept> fn =
        [](int a, double b) noexcept {
            volatile int res = a + static_cast<int>(b);
            (void)res;
        };

    for (auto _ : s) {
        fn(42, 3.14);
    }
}

static void noexcept_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, void(int, double) noexcept>
        ::restrict_layout<ebd::unique_fn<void()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](int a, double b) noexcept {
            volatile int res = a + static_cast<int>(b);
            (void)res;
        });

    for (auto _ : s) {
        (*fn)(42, 3.14);
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Noexcept);
BENCHMARK_BASELINE(noexcept_params_std);
BENCHMARK_NOTBASE(noexcept_params_ebd);
BENCHMARK_NOTBASE(noexcept_params_fu2);
BENCHMARK_NOTBASE(noexcept_params_pro);

// ----------------------------------------------------------------------------
// Test 12: Const Qualified Function
// ----------------------------------------------------------------------------

static void const_params_std(picobench::state& s) {
    std::move_only_function<int(int, int) const> fn = 
        [](int a, int b) { return a * b; };

    for (auto _ : s) {
        volatile int res = fn(10, 20);
        (void)res;
    }
}

static void const_params_ebd(picobench::state& s) {
    ebd::unique_fn<int(int, int) const> fn = 
        [](int a, int b) { return a * b; };

    for (auto _ : s) {
        volatile int res = fn(10, 20);
        (void)res;
    }
}

static void const_params_fu2(picobench::state& s) {
    fu2::unique_function<int(int, int) const> fn =
        [](int a, int b) { return a * b; };

    for (auto _ : s) {
        volatile int res = fn(10, 20);
        (void)res;
    }
}

static void const_params_pro(picobench::state& s) {
    using Invoker = pro::facade_builder
        ::add_convention<pro::operator_dispatch<"()">, int(int, int) const>
        ::restrict_layout<ebd::unique_fn<int()>::get_buffer_size()>
        ::build;
    pro::proxy<Invoker> fn = pro::make_proxy_inplace<Invoker>(
        [](int a, int b) { return a * b; });

    for (auto _ : s) {
        volatile int res = (*fn)(10, 20);
        (void)res;
    }
}

BENCHMARK_UNIT(MoveOnlyFunction.Params.Const);
BENCHMARK_BASELINE(const_params_std);
BENCHMARK_NOTBASE(const_params_ebd);
BENCHMARK_NOTBASE(const_params_fu2);
BENCHMARK_NOTBASE(const_params_pro);

#endif
