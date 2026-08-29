#include "__constant_wrapper.hpp"
#include "test_function.hpp"

#if __cpp_lib_constant_wrapper >= 202603L

TEST(Conformance_fn, constant_wrapper_pass) {
    {
        using Class = ebd_test_member_fn;
        ebd::fn<int(int, int)> f1(std::cw<&Class::mem_fn_ii_add>, Class{});
        ASSERT_EQ(f1(0, 42), 42);
        ASSERT_EQ(f1(1, 42), 43);

        ebd::fn<int() noexcept> f2(std::cw<&Class::mem_fn_noexcept>, Class{});
        ASSERT_EQ(f2(), 0);

        ebd::fn<int() &&> f3(std::cw<&Class::mem_fn_rref_callable>, Class{});
        ASSERT_EQ(std::move(f3)(), OVL_R_REF);
    }
}

#endif // __cpp_lib_constant_wrapper >= 202603L
