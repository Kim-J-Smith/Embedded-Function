#include "__constant_wrapper.hpp"
#include "test_function.hpp"

#if __cpp_lib_constant_wrapper >= 202603L

namespace {
    
struct ExplicitThis {
    int add(this ExplicitThis&, int a, int b) {
        return a + b;
    }
};

}


TEST(Conformance_fn, constant_wrapper_pass) {
    {
        using Class = ebd_test_member_fn;
        Class obj{};
        auto* ptr = &obj;

        {
            ebd::fn<int(int, int)> f1(std::cw<&Class::mem_fn_ii_add>, obj);
            ASSERT_EQ(f1(0, 42), 42);
            ASSERT_EQ(f1(1, 42), 43);

            ebd::fn<int(int, int)> f2(std::cw<&Class::mem_fn_ii_add>, ptr);
            ASSERT_EQ(f2(0, 42), 42);
            ASSERT_EQ(f2(1, 42), 43);
        }
        {
            ebd::fn<int() noexcept> f1(std::cw<&Class::mem_fn_noexcept>, obj);
            ASSERT_EQ(f1(), 0);
            ebd::fn<int() noexcept> f2(std::cw<&Class::mem_fn_noexcept>, ptr);
            ASSERT_EQ(f2(), 0);
        }
        {
            ebd::fn<int() &&> f1(std::cw<&Class::mem_fn_rref_callable>, obj);
            ASSERT_EQ(std::move(f1)(), OVL_R_REF);

            // ERROR
            // ebd::fn<int() &&> f2(std::cw<&Class::mem_fn_rref_callable>, ptr);
            // ASSERT_EQ(std::move(f2)(), OVL_R_REF);
        }
    }

#if !defined(__clang__) || defined(EBD_TEST_TRY_BUG__Clang_106660)
    // Clang bug <https://github.com/llvm/llvm-project/issues/106660>
    {
        ExplicitThis obj;

        auto f1 = ebd::make_fn(std::cw<&ExplicitThis::add>, obj);
        ASSERT_EQ(f1(0, 42), 42);
        ASSERT_EQ(f1(1, 42), 43);

        // ERROR
        // auto f1 = ebd::make_fn(std::cw<&ExplicitThis::add>, &obj);
        // ASSERT_EQ(f1(0, 42), 42);
        // ASSERT_EQ(f1(1, 42), 43);
    }
#endif

}

#endif // __cpp_lib_constant_wrapper >= 202603L
