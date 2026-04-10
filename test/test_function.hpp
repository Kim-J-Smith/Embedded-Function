#pragma once
#include "embed/embed_function.hpp"
#include "gtest/gtest.h"

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

inline float ebd_test_free_func_fff_mul(float a, float b) { return a * b; }

inline int ebd_test_free_func_noexcept() noexcept { return 0; }

enum OverloadRes {
    OVL_VOID = 0,
    OVL_INT,
    OVL_CHAR,
    OVL_FLOAT,
    OVL_DOUBLE,
    OVL_INT_INT,
    OVL_INT_FLOAT,
};
inline int ebd_test_free_func_overload() { return OVL_VOID; }
inline int ebd_test_free_func_overload(int) { return OVL_INT; }
inline int ebd_test_free_func_overload(char) { return OVL_CHAR; }
inline int ebd_test_free_func_overload(float) { return OVL_FLOAT; }
inline int ebd_test_free_func_overload(double) { return OVL_DOUBLE; }
inline int ebd_test_free_func_overload(int, int) { return OVL_INT_INT; }
inline int ebd_test_free_func_overload(int, float) { return OVL_INT_FLOAT; }

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
    int operator()(this ebd_test_operator_unambiguous& self, int change) {
        self.m_member_var += change;
        return self.m_member_var;
    }
#else
    int operator()(int change) {
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

inline int (ebd_test_member_fn::* ebd_test_return_ptr_class()) (int, int) {
    return &ebd_test_member_fn::mem_fn_ii_add;
} 

class ebd_test_non_move_non_copyable {
public:
    ebd_test_non_move_non_copyable() = default;
    ~ebd_test_non_move_non_copyable() = default;
    ebd_test_non_move_non_copyable(const ebd_test_non_move_non_copyable&) = delete;
    ebd_test_non_move_non_copyable(ebd_test_non_move_non_copyable&&) = delete;

    int operator()(char) { return OVL_CHAR; }
};
