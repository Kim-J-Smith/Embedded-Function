#pragma once
#include <cstdio>
#define EMBED_FN_HOOK_DEBUG(message) do { fputs(message, stderr); fflush(stderr); } while(0)

#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

namespace ebd {

/// @attention TEST USE ONLY!
template <
    typename Signature,
    std::size_t BufferSize = detail::default_values::owning::buffer_size,
    std::size_t Alignment = detail::default_values::owning::alignment
>
using __safe_fn = basic_fn<Signature, BufferSize,
                           Alignment,
                           true,    // Is Copyable
                           false,   // Not View
                           false,   // Not Throwing on empty calls
                           true>;   // Assert Ctor/Dtor Nothrow

} // namespace ebd

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )
# define EBD_TEST_NOEXCEPT noexcept
#else
# define EBD_TEST_NOEXCEPT
#endif

#if ( EMBED_CXX_ENABLE_EXCEPTION == true )
# define EBD_EXPECT_THROW(statement, expected_exception) \
    EXPECT_THROW(statement, expected_exception)
# define EBD_TRY try
# define EBD_CATCH(exception) catch(exception)
# define EBD_THROW(x) throw x
#else
# define EBD_EXPECT_THROW(statement, unused) \
    EXPECT_DEATH(statement, "")
# define EBD_TRY while(0)
# define EBD_CATCH(exception) while(0)
# define EBD_THROW(x) std::terminate()
#endif

inline void ebd_test_free_func_v() {}

inline void ebd_test_free_func_vii(int, int) {}

inline int ebd_test_free_func_iii_add(int a, int b) { return a + b; }
inline int ebd_test_free_func_iii_add_noexcept(int& a, int& b) noexcept { return a + b; }
inline int ebd_test_free_func_iii_add_const(const int& a, const int& b) { return a + b; }

inline float ebd_test_free_func_fff_mul(float a, float b) { return a * b; }

inline int ebd_test_free_func_noexcept() noexcept { return 0; }
inline int ebd_test_free_func_maythrow() noexcept(false) { return 1; }
inline int ebd_test_free_func_noexcept_2() noexcept { return 2; }

enum OverloadRes {
    OVL_VOID = 0,
    OVL_INT,
    OVL_CHAR,
    OVL_FLOAT,
    OVL_DOUBLE,
    OVL_INT_INT,
    OVL_INT_FLOAT,
    OVL_INT_INT_INT,
};
inline int ebd_test_free_func_overload() { return OVL_VOID; }
inline int ebd_test_free_func_overload(int) { return OVL_INT; }
inline int ebd_test_free_func_overload(char) { return OVL_CHAR; }
inline int ebd_test_free_func_overload(float) { return OVL_FLOAT; }
inline int ebd_test_free_func_overload(double) { return OVL_DOUBLE; }
inline int ebd_test_free_func_overload(int, int) { return OVL_INT_INT; }
inline int ebd_test_free_func_overload(int, float) { return OVL_INT_FLOAT; }
inline int ebd_test_free_func_overload(int, int, int) noexcept { return OVL_INT_INT_INT; }

namespace ebd_test {

    inline void nest_free_function_v() {}

    inline int nest_free_function_ii_add123(int x) { return x + 123; }
}

template <typename T>
inline T ebd_test_free_func_template(T a, T b, T c) { return a + b + c; }

class ebd_test_member_fn {
public:
    static void static_mem_fn() {}

    static int static_mem_fn_ii_add(int a, int b) { return a + b; }

    static float static_mem_fn_fff_mul(float a, float b) { return a * b; }

    template <typename T>
    static T static_mem_fn_template(T a, T b, T c) { return a + b + c; }

    static int static_mem_fn_noexcept() noexcept { return 0; }

    static int static_mem_fn_overload() { return OVL_VOID; }
    static int static_mem_fn_overload(int) { return OVL_INT; }
    static int static_mem_fn_overload(char) { return OVL_CHAR; }
    static int static_mem_fn_overload(float) { return OVL_FLOAT; }
    static int static_mem_fn_overload(double) { return OVL_DOUBLE; }
    static int static_mem_fn_overload(int, int) { return OVL_INT_INT; }
    static int static_mem_fn_overload(int, float) { return OVL_INT_FLOAT; }

    void mem_fn() {}

    int mem_fn_ii_add(int a, int b) { return a + b; }

    float mem_fn_fff_mul(float a, float b) { return a * b; }

    template <typename T>
    T mem_fn_template(T a, T b, T c) { return a + b + c; }

    int mem_fn_noexcept() noexcept { return 0; }

    int mem_fn_overload() { return OVL_VOID; }
    int mem_fn_overload(int) { return OVL_INT; }
    int mem_fn_overload(char) { return OVL_CHAR; }
    int mem_fn_overload(float) { return OVL_FLOAT; }
    int mem_fn_overload(double) { return OVL_DOUBLE; }
    int mem_fn_overload(int, int) { return OVL_INT_INT; }
    int mem_fn_overload(int, float) { return OVL_INT_FLOAT; }

    int get_var_and_increase(int step) noexcept {
        auto tmp = member_var;
        member_var += step;
        return tmp;
    }
    int member_var{};
};


class ebd_test_operator_unambiguous {
public:
    int m_member_var = 0;
#if ( __cpp_explicit_this_parameter >= 202110L ) || ( EMBED_CXX_VERSION >= 202302L )
    int operator()(this ebd_test_operator_unambiguous& self, int change) noexcept {
        self.m_member_var += change;
        return self.m_member_var;
    }
#else
    int operator()(int change) & noexcept {
        m_member_var += change;
        return m_member_var; 
    }
#endif
};

class ebd_test_operator_ambiguous { 
public:
    int operator()(int) { return OVL_INT; }

    int operator()(char) { return OVL_CHAR; }

    int operator()(float) { return OVL_FLOAT; }

    int operator()(double) { return OVL_DOUBLE; }

    int operator()(int, int) { return OVL_INT_INT; }

    int operator()(int, float) { return OVL_INT_FLOAT; }
};

class ebd_test_virtual_operator_ambiguous_base { 
public:
    virtual int operator()(int) { return 0; }

    virtual int operator()(char) { return 0; }

    virtual ~ebd_test_virtual_operator_ambiguous_base() = default;
};

class ebd_test_virtual_operator_ambiguous_derived 
    : public ebd_test_virtual_operator_ambiguous_base { 
public:
    int operator()(int) { return OVL_INT; }

    int operator()(char) { return OVL_CHAR; }

    int operator()(float) { return OVL_FLOAT; }
};

class ebd_test_move_only_callable {
public:
    ebd_test_move_only_callable() = default;
    ebd_test_move_only_callable(const ebd_test_move_only_callable&) = delete;
    ebd_test_move_only_callable(ebd_test_move_only_callable&&) = default;
    ~ebd_test_move_only_callable() = default;

    int operator()(char) && { return OVL_CHAR; }
};

enum OverloadQualifier {
    OVL_CONST       = 0x01,
    OVL_VOLATILE    = 0x02,
    OVL_L_REF       = 0x04,
    OVL_R_REF       = 0x08,
};

class ebd_test_operator_qualifier { 
public:
    int operator()(int) const { return OVL_CONST; }

    int operator()(int) volatile { return OVL_VOLATILE; }

    int operator()(char) & { return OVL_L_REF; }

    int operator()(char) && { return OVL_R_REF; }

    int operator()(float) const & { return OVL_CONST | OVL_L_REF; }

    int operator()(float) const && { return OVL_CONST | OVL_R_REF; }

    int operator()(float) volatile & { return OVL_VOLATILE | OVL_L_REF; }

    int operator()(float) volatile && { return OVL_VOLATILE | OVL_R_REF; }

    int operator()(double) const volatile & { return OVL_CONST | OVL_VOLATILE | OVL_L_REF; }

    int operator()(double) const volatile && { return OVL_CONST | OVL_VOLATILE | OVL_R_REF; }
};

// Like `ebd_test_operator_qualifier`, but stateful (has a data member).
// A stateful functor is stored in the wrapper buffer, so invoking a volatile
// or const-volatile signature goes through the inplace invoker and reaches the
// volatile / const-volatile `access()` overloads of `Erasure`.
class ebd_test_stateful_operator_qualifier {
public:
    explicit ebd_test_stateful_operator_qualifier(int base) noexcept : m_base(base) {}

    int operator()(int) volatile { return m_base + OVL_VOLATILE; }

    int operator()(int) const volatile { return m_base + OVL_CONST + OVL_VOLATILE; }

private:
    int m_base;
};

inline int (ebd_test_member_fn::* ebd_test_return_ptr_class()) (int, int) {
    return &ebd_test_member_fn::mem_fn_ii_add;
} 

class ebd_test_non_move_non_copyable {
public:
    ebd_test_non_move_non_copyable() = default;
    ~ebd_test_non_move_non_copyable() = default;
    ebd_test_non_move_non_copyable(const ebd_test_non_move_non_copyable&) = delete;
    ebd_test_non_move_non_copyable(ebd_test_non_move_non_copyable&&) = delete;

    int operator()(char) const { return OVL_CHAR; }
};

#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)
struct ebd_test_static_call_operator {
    static int operator()(int a, int b) noexcept { return a + b; }
};
#endif

inline int ebd_test_safe_tmp_fn(ebd::fn_ref<int()> f) { return f(); }

struct ebd_test_large_alignment {
    alignas(std::max_align_t) int m_var;
    int operator()() const { return 42; }
};

struct ebd_test_counter {
    static int m_create_times;
    static int m_copy_times;
    static int m_move_times;
    static int m_delete_times;

    ebd_test_counter(int) noexcept { m_create_times++; }
    ebd_test_counter() noexcept { m_create_times++; }
    ebd_test_counter(const ebd_test_counter&) noexcept { m_copy_times++; }
    ebd_test_counter(ebd_test_counter&&) noexcept { m_move_times++; }
    ~ebd_test_counter() noexcept { m_delete_times++; }

    static void clear() noexcept {
        m_create_times = 0;
        m_copy_times = 0;
        m_move_times = 0;
        m_delete_times = 0;
    }

    int operator()(int n) const noexcept { return n + 42; }
};

struct ebd_test_implicit_func_ptr {
    using func_ptr = int (*) (int);

    operator func_ptr() const {
        return [](int n) { return n + 42; };
    }
};

struct ebd_test_great_alignment {
    alignas(128) int a;
    int operator()() { return a + 42; }
};
