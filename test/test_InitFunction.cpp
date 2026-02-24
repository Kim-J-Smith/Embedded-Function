#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

#include <iostream>

// InitFunction[0]
TEST(InitFunction, fn_freeFunction_v) {
    ebd::fn<void(int, int)> f = ebd_test_free_func_vii;
    const std::size_t buf_size = f.buffer_size;
    const std::size_t buf_expect = ebd::detail::default_buffer_size::value;
    ASSERT_EQ(buf_size, buf_expect);
    ASSERT_EQ(f != nullptr, true);
    ASSERT_EQ(f == nullptr, false);
    ASSERT_EQ(static_cast<bool>(f), true);

    f = nullptr;
    ASSERT_EQ(f != nullptr, false);
    ASSERT_EQ(f == nullptr, true);
    ASSERT_EQ(static_cast<bool>(f), false);

    auto f2 = ebd::make_fn(ebd_test_free_func_v);
    ASSERT_EQ(f2.is_empty(), false);
}

// InitFunction[1]
TEST(InitFunction, fn_freeFunction_i) {
    ebd::fn<int(int,int)> f = ebd_test_free_func_iii_add;
    ASSERT_EQ(f.is_empty(), false);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            ASSERT_EQ(f(i, j), i + j);
        }
    }

    auto f2 = ebd::make_fn(ebd_test_free_func_iii_add);
    ASSERT_EQ(f2.is_empty(), false);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            ASSERT_EQ(f2(i, j), i + j);
        }
    }
}

// InitFunction[2]
TEST(InitFunction, fn_freeFunction_f) {
    ebd::fn<float(float,float)> f = ebd_test_free_func_fff_mul;
    ASSERT_EQ(f.is_empty(), false);
    for (float i = 0; i < 100; i++) {
        for (float j = 0; j < 100; j++) {
            ASSERT_FLOAT_EQ(f(i, j), i * j);
        }
    }

    auto f2 = ebd::make_fn(ebd_test_free_func_fff_mul);
    ASSERT_EQ(f2.is_empty(), false);
    for (float i = 0; i < 100; i++) {
        for (float j = 0; j < 100; j++) {
            ASSERT_FLOAT_EQ(f2(i, j), i * j);
        }
    }
}

// InitFunction[3]
TEST(InitFunction, fn_freeFunction_template) { 
    ebd::fn<int(int, int, int)> f = ebd_test_free_func_template<int>;
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(1, 2, 3), 6);

    auto f2 = ebd::make_fn(ebd_test_free_func_template<int>);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(1, 2, 3), 6);
}

// InitFunction[4]
TEST(InitFunction, fn_freeFunction_noexcept) { 
    ebd::safe_fn<int() EBD_TEST_NOEXCEPT> f = ebd_test_free_func_noexcept;
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(), 0);

    auto f2 = ebd::make_fn(ebd_test_free_func_noexcept);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(), 0);
}

// InitFunction[5]
TEST(InitFunction, fn_freeFunction_overload) { 
    auto f1 = ebd::make_fn<int()>(ebd_test_free_func_overload);
    auto f2 = ebd::make_fn<int(char)>(ebd_test_free_func_overload);
    auto f3 = ebd::make_fn<int(double)>(ebd_test_free_func_overload);
    auto f4 = ebd::make_fn<int(float)>(ebd_test_free_func_overload);
    auto f5 = ebd::make_fn<int(int)>(ebd_test_free_func_overload);
    auto f6 = ebd::make_fn<int(int, float)>(ebd_test_free_func_overload);       
    auto f7 = ebd::make_fn<int(int, int)>(ebd_test_free_func_overload);

    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f3.is_empty(), false);
    ASSERT_EQ(f4.is_empty(), false);
    ASSERT_EQ(f5.is_empty(), false);
    ASSERT_EQ(f6.is_empty(), false);
    ASSERT_EQ(f7.is_empty(), false);

    ASSERT_EQ(f1(), OVL_VOID);
    ASSERT_EQ(f2('a'), OVL_CHAR);
    ASSERT_EQ(f3(1.0), OVL_DOUBLE);
    ASSERT_EQ(f4(1.0f), OVL_FLOAT);
    ASSERT_EQ(f5(1), OVL_INT);
    ASSERT_EQ(f6(1, 1.0f), OVL_INT_FLOAT);
    ASSERT_EQ(f7(1, 1), OVL_INT_INT);
}

// InitFunction[6]
TEST(InitFunction, fn_memberFunction_static_v) {
    ebd::fn<void()> f = ebd_test_member_fn::static_mem_fn;
    ASSERT_EQ(f.is_empty(), false);
    
    auto f2 = ebd::make_fn(ebd_test_member_fn::static_mem_fn);
    ASSERT_EQ(f2.is_empty(), false);
}

// InitFunction[7]
TEST(InitFunction, fn_memberFunction_static_i) { 
    ebd::fn<int(int, int)> f = ebd_test_member_fn::static_mem_fn_ii_add;
    ASSERT_EQ(f.is_empty(), false);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            ASSERT_EQ(f(i, j), i + j);
        }
    }

    auto f2 = ebd::make_fn(ebd_test_member_fn::static_mem_fn_ii_add);
    ASSERT_EQ(f2.is_empty(), false);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            ASSERT_EQ(f2(i, j), i + j);
        }
    }
}

// InitFunction[8]
TEST(InitFunction, fn_memberFunction_static_f) { 
    ebd::fn<float(float, float)> f = ebd_test_member_fn::static_mem_fn_fff_mul;
    ASSERT_EQ(f.is_empty(), false);
    for (float i = 0; i < 100; i++) {
        for (float j = 0; j < 100; j++) {
            ASSERT_FLOAT_EQ(f(i, j), i * j);
        }
    }

    auto f2 = ebd::make_fn(ebd_test_member_fn::static_mem_fn_fff_mul);
    ASSERT_EQ(f2.is_empty(), false);
    for (float i = 0; i < 100; i++) {
        for (float j = 0; j < 100; j++) {
            ASSERT_FLOAT_EQ(f2(i, j), i * j);
        }
    }
}

// InitFunction[9]
TEST(InitFunction, fn_memberFunction_static_template) { 
    ebd::fn<int(int, int, int)> f = ebd_test_member_fn::static_mem_fn_template<int>;
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(1, 2, 3), 6);

    auto f2 = ebd::make_fn(ebd_test_member_fn::static_mem_fn_template<int>);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(1, 2, 3), 6);
}

// InitFunction[10]
TEST(InitFunction, fn_memberFunction_static_noexcept) { 
    // ebd::fn<int() noexcept> is invalid
    ebd::safe_fn<int() EBD_TEST_NOEXCEPT> f = ebd_test_member_fn::static_mem_fn_noexcept;
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(), 0);

    auto f2 = ebd::make_fn(ebd_test_member_fn::static_mem_fn_noexcept);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(), 0);
}

// InitFunction[11]
TEST(InitFunction, fn_memberFunction_static_overload) { 
    auto f1 = ebd::make_fn<int()>(ebd_test_member_fn::static_mem_fn_overload);
    auto f2 = ebd::make_fn<int(char)>(ebd_test_member_fn::static_mem_fn_overload);
    auto f3 = ebd::make_fn<int(double)>(ebd_test_member_fn::static_mem_fn_overload);
    auto f4 = ebd::make_fn<int(float)>(ebd_test_member_fn::static_mem_fn_overload);
    auto f5 = ebd::make_fn<int(int)>(ebd_test_member_fn::static_mem_fn_overload);
    auto f6 = ebd::make_fn<int(int, float)>(ebd_test_member_fn::static_mem_fn_overload);
    auto f7 = ebd::make_fn<int(int, int)>(ebd_test_member_fn::static_mem_fn_overload);

    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f3.is_empty(), false);
    ASSERT_EQ(f4.is_empty(), false);
    ASSERT_EQ(f5.is_empty(), false);
    ASSERT_EQ(f6.is_empty(), false);
    ASSERT_EQ(f7.is_empty(), false);

    ASSERT_EQ(f1(), OVL_VOID);
    ASSERT_EQ(f2('a'), OVL_CHAR);
    ASSERT_EQ(f3(1.0), OVL_DOUBLE);
    ASSERT_EQ(f4(1.0f), OVL_FLOAT);
    ASSERT_EQ(f5(1), OVL_INT);
    ASSERT_EQ(f6(1, 1.0f), OVL_INT_FLOAT);
    ASSERT_EQ(f7(1, 1), OVL_INT_INT);
}

// InitFunction[12]
TEST(InitFunction, fn_memberFunction_v) { 
    using C_t = ebd_test_member_fn;
    C_t c;
    ebd::fn<void(C_t&)> f = &C_t::mem_fn;
    ASSERT_EQ(f.is_empty(), false);
    f(c);

    auto f2 = ebd::make_fn(&C_t::mem_fn);
    ASSERT_EQ(f2.is_empty(), false);
    f2(c);
}

// InitFunction[13]
TEST(InitFunction, fn_memberFunction_i) { 
    using C_t = ebd_test_member_fn;
    C_t c;
    ebd::fn<int(C_t&, int, int)> f = &C_t::mem_fn_ii_add;
    ASSERT_EQ(f.is_empty(), false);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            ASSERT_EQ(f(c, i, j), i + j);
        }
    }

    auto f2 = ebd::make_fn(&C_t::mem_fn_ii_add);
    ASSERT_EQ(f2.is_empty(), false);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            ASSERT_EQ(f2(c, i, j), i + j);
        }
    }
}

// InitFunction[14]
TEST(InitFunction, fn_memberFunction_f) { 
    using C_t = ebd_test_member_fn;
    C_t c;
    ebd::fn<float(C_t&, float, float)> f = &C_t::mem_fn_fff_mul;
    ASSERT_EQ(f.is_empty(), false);
    for (float i = 0; i < 100; i++) {
        for (float j = 0; j < 100; j++) {
            ASSERT_FLOAT_EQ(f(c, i, j), i * j);
        }
    }

    auto f2 = ebd::make_fn(&C_t::mem_fn_fff_mul);
    ASSERT_EQ(f2.is_empty(), false);
    for (float i = 0; i < 100; i++) {
        for (float j = 0; j < 100; j++) {
            ASSERT_FLOAT_EQ(f2(c, i, j), i * j);
        }
    }
}

// InitFunction[15]
TEST(InitFunction, fn_memberFunction_template) { 
    using C_t = ebd_test_member_fn;
    C_t c;
    ebd::fn<int(C_t&, int, int, int)> f = &C_t::mem_fn_template<int>;
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(c, 1, 2, 3), 6);

    auto f2 = ebd::make_fn(&C_t::mem_fn_template<int>);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(c, 1, 2, 3), 6);
}

// InitFunction[16]
TEST(InitFunction, fn_memberFunction_noexcept) { 
    using C_t = ebd_test_member_fn;
    C_t c;
    ebd::safe_fn<int(C_t&) EBD_TEST_NOEXCEPT> f = &C_t::mem_fn_noexcept;
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(c), 0);

    auto f2 = ebd::make_fn(&C_t::mem_fn_noexcept);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(c), 0);
}

// InitFunction[17]
TEST(InitFunction, fn_memberFunction_overload) { 
    using C_t = ebd_test_member_fn;
    C_t c;
    auto f1 = ebd::make_fn<int(C_t&)>(&C_t::mem_fn_overload);
    auto f2 = ebd::make_fn<int(C_t&, char)>(&C_t::mem_fn_overload);
    auto f3 = ebd::make_fn<int(C_t&, double)>(&C_t::mem_fn_overload);
    auto f4 = ebd::make_fn<int(C_t&, float)>(&C_t::mem_fn_overload);
    auto f5 = ebd::make_fn<int(C_t&, int)>(&C_t::mem_fn_overload);
    auto f6 = ebd::make_fn<int(C_t&, int, float)>(&C_t::mem_fn_overload);
    auto f7 = ebd::make_fn<int(C_t&, int, int)>(&C_t::mem_fn_overload);

    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f3.is_empty(), false);
    ASSERT_EQ(f4.is_empty(), false);
    ASSERT_EQ(f5.is_empty(), false);
    ASSERT_EQ(f6.is_empty(), false);
    ASSERT_EQ(f7.is_empty(), false);

    ASSERT_EQ(f1(c), OVL_VOID);
    ASSERT_EQ(f2(c, 'a'), OVL_CHAR);
    ASSERT_EQ(f3(c, 1.0), OVL_DOUBLE);
    ASSERT_EQ(f4(c, 1.0f), OVL_FLOAT);
    ASSERT_EQ(f5(c, 1), OVL_INT);
    ASSERT_EQ(f6(c, 1, 1.0f), OVL_INT_FLOAT);
    ASSERT_EQ(f7(c, 1, 1), OVL_INT_INT);
}

// InitFunction[18]
TEST(InitFunction, fn_Functor) { 
    ebd::fn<int(int)> f = ebd_test_operator_unambiguous{};
    ASSERT_EQ(f.is_empty(), false);
    ASSERT_EQ(f(1), 1);
    ASSERT_EQ(f(2), 3);
    ASSERT_EQ(f(1), 4);

    auto f2 = ebd::make_fn(ebd_test_operator_unambiguous{});
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f2(1), 1);
    ASSERT_EQ(f2(2), 3);
    ASSERT_EQ(f2(1), 4);
}

// InitFunction[19]
TEST(InitFunction, fn_Functor_ambiguous) { 
    auto f1 = ebd::make_fn<int(int)>(ebd_test_operator_ambiguous{});
    auto f2 = ebd::make_fn<int(char)>(ebd_test_operator_ambiguous{});
    auto f3 = ebd::make_fn<int(float)>(ebd_test_operator_ambiguous{});
    auto f4 = ebd::make_fn<int(double)>(ebd_test_operator_ambiguous{});
    auto f5 = ebd::make_fn<int(int, int)>(ebd_test_operator_ambiguous{});
    auto f6 = ebd::make_fn<int(int, float)>(ebd_test_operator_ambiguous{});

    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f3.is_empty(), false);
    ASSERT_EQ(f4.is_empty(), false);
    ASSERT_EQ(f5.is_empty(), false);
    ASSERT_EQ(f6.is_empty(), false);

    ASSERT_EQ(f1(1), OVL_INT);
    ASSERT_EQ(f2('a'), OVL_CHAR);
    ASSERT_EQ(f3(1.0f), OVL_FLOAT);
    ASSERT_EQ(f4(1.0), OVL_DOUBLE);
    ASSERT_EQ(f5(1, 1), OVL_INT_INT);
    ASSERT_EQ(f6(1, 1.0f), OVL_INT_FLOAT);
}

// InitFunction[20]
TEST(InitFunction, VirtualOverloadOperator) {
    using base = ebd_test_virtual_operator_ambiguous_base;
    using derived = ebd_test_virtual_operator_ambiguous_derived;

    auto f1 = ebd::make_fn<int(char)>(base{});
    auto f2 = ebd::make_fn<int(int)>(base{});

    auto f3 = ebd::make_fn<int(char)>(derived{});
    auto f4 = ebd::make_fn<int(int)>(derived{});
    auto f5 = ebd::make_fn<int(float)>(derived{});

    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f3.is_empty(), false);
    ASSERT_EQ(f4.is_empty(), false);
    ASSERT_EQ(f5.is_empty(), false);

    ASSERT_EQ(f1('a'), 0);
    ASSERT_EQ(f2(1), 0);
    ASSERT_EQ(f3('b'), OVL_CHAR);
    ASSERT_EQ(f4(2), OVL_INT);
    ASSERT_EQ(f5(3.f), OVL_FLOAT);
}

// InitFunction[21]
TEST(InitFunction, fn_Lambda) {
    int a = 100;
    ebd::fn<int() const> f1 = []{ return 1; };
    ebd::fn<int(int)> f2 = [a](int x){ return a + x; };
    auto f3 = ebd::make_fn([](float a, float b) -> float {
        return a * b;
    });
    auto f4 = ebd::make_fn([a](int z){
        return a * z;
    });

    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2 == nullptr, false);
    ASSERT_EQ(f3 != nullptr, true);
    ASSERT_EQ(f4 != nullptr, true);

    ASSERT_EQ(f1(), 1);
    ASSERT_EQ(f2(23), 123);
    ASSERT_FLOAT_EQ(f3(1.1f, 3.14f), 1.1f * 3.14f);
    ASSERT_EQ(f4(8), 100 * 8);
}

// InitFunction[22]
TEST(InitFunction, unique_fn_MoveOnly_Functor) {
    ebd::unique_fn<int(char) &&> f1 = ebd_test_move_only_callable{};
    ASSERT_EQ(f1 != nullptr, true);
    ASSERT_EQ(std::move(f1)('A'), OVL_CHAR);

    auto f2 = ebd::make_fn(ebd_test_move_only_callable{});
    ASSERT_EQ(f2 != nullptr, true);
    ASSERT_EQ(std::move(f2)('B'), OVL_CHAR);
}

// InitFunction[23]
TEST(InitFunction, fn_view_MoveOnly_Functor) {
    ebd_test_move_only_callable obj;
    ebd::fn_view<int(char) &&> f1 = obj;
    ASSERT_EQ(f1 != nullptr, true);
    ASSERT_EQ(std::move(f1)('A'), OVL_CHAR);
}

// InitFunction[24]
TEST(InitFunction, testQualifier) {
    using C_t = ebd_test_operator_qualifier;
    const C_t c;
    c(1);
    auto f1 = ebd::make_fn<int(int) const>(c);
    auto f2 = ebd::make_fn<int(int) volatile>(c);
    auto f3 = ebd::make_fn<int(char) &>(c);
    auto f4 = ebd::make_fn<int(char) &&>(c);

    ASSERT_EQ(f1(1), OVL_CONST);
    ASSERT_EQ(f2(1), OVL_VOLATILE);
    ASSERT_EQ(f3('a'), OVL_L_REF);
    ASSERT_EQ(std::move(f4)('a'), OVL_R_REF);

    auto f5 = ebd::make_fn<int(float) const &>(c);
    auto f6 = ebd::make_fn<int(float) const &&>(c);
    auto f7 = ebd::make_fn<int(float) volatile &>(c);
    auto f8 = ebd::make_fn<int(float) volatile &&>(c);

    ASSERT_EQ(f5(1.0f), OVL_CONST | OVL_L_REF);
    ASSERT_EQ(std::move(f6)(1.0f), OVL_CONST | OVL_R_REF);
    ASSERT_EQ(f7(1.0f), OVL_VOLATILE | OVL_L_REF);
    ASSERT_EQ(std::move(f8)(1.0f), OVL_VOLATILE | OVL_R_REF);

    auto f9 = ebd::make_fn<int(double) const volatile &>(c);
    auto f10 = ebd::make_fn<int(double) const volatile &&>(c);

    ASSERT_EQ(f9(3.14), OVL_CONST | OVL_VOLATILE | OVL_L_REF);
    ASSERT_EQ(std::move(f10)(3.14), OVL_CONST | OVL_VOLATILE | OVL_R_REF);
}
