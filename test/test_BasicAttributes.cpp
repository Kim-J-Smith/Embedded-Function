#include "test_function.hpp"

#include <type_traits>

// BasicAttributes[0]
TEST(BasicAttributes, SizeAndAlign) {
    using f_t = ebd::fn<void()>;
    using uf_t = ebd::unique_fn<void()>;
    using cf_t = ebd::classic_fn<void()>;
    using sf_t = ebd::__safe_fn<void()>;
    using fr_t = ebd::fn_ref<void()>;

    ASSERT_EQ(f_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size, true);
    ASSERT_EQ(uf_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size, true);
    ASSERT_EQ(cf_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size, true);
    ASSERT_EQ(sf_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size, true);
    ASSERT_EQ(fr_t::get_buffer_size() == ebd::detail::default_values::non_owning::buffer_size, true);

    ASSERT_EQ(alignof(f_t) == ebd::detail::default_values::owning::alignment, true);
    ASSERT_EQ(alignof(uf_t) == ebd::detail::default_values::owning::alignment, true);
    ASSERT_EQ(alignof(cf_t) == ebd::detail::default_values::owning::alignment, true);
    ASSERT_EQ(alignof(sf_t) == ebd::detail::default_values::owning::alignment, true);
    ASSERT_EQ(alignof(fr_t) == alignof(void(*)()), true);

    ASSERT_EQ(sizeof(f_t) - f_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(uf_t) - uf_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(cf_t) - cf_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(sf_t) - sf_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(fr_t) - fr_t::get_buffer_size(), sizeof(void*));
}

// BasicAttributes[1]
TEST(BasicAttributes, AbilityAndNoexcept) {
    using f_t = ebd::fn<void()>;
    using uf_t = ebd::unique_fn<void()>;
    using cf_t = ebd::classic_fn<void()>;
    using sf_t = ebd::__safe_fn<void()>;
    using fr_t = ebd::fn_ref<void()>;

    // f_t
    ASSERT_EQ(std::is_move_constructible<f_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<f_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<f_t>::value == true, true);
    ASSERT_EQ(std::is_copy_assignable<f_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_constructible<f_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_copy_assignable<f_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_destructible<f_t>::value == false, true);

    // uf_t
    ASSERT_EQ(std::is_move_constructible<uf_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<uf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<uf_t>::value == false, true);
    ASSERT_EQ(std::is_copy_assignable<uf_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_move_constructible<uf_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_move_assignable<uf_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_destructible<uf_t>::value == false, true);

    // cf_t (classic_fn is copyable, like fn)
    ASSERT_EQ(std::is_move_constructible<cf_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<cf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<cf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_assignable<cf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_constructible<cf_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_copy_assignable<cf_t>::value == false, true);
    ASSERT_EQ(std::is_nothrow_destructible<cf_t>::value == false, true);

    // sf_t (__safe_fn asserts nothrow, so copy/move/destroy are nothrow)
    ASSERT_EQ(std::is_move_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_destructible<sf_t>::value == true, true);

    // fr_t
    ASSERT_EQ(std::is_move_constructible<fr_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<fr_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<fr_t>::value == true, true);
    ASSERT_EQ(std::is_copy_assignable<fr_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_constructible<fr_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_assignable<fr_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_constructible<fr_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_assignable<fr_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_destructible<fr_t>::value == true, true);
}

// BasicAttributes[2]
TEST(BasicAttributes, TrivialityViewMode) {
    using view_t = ebd::fn_ref<void()>;
    ASSERT_EQ(std::is_trivially_destructible<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_move_constructible<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_move_assignable<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_copy_constructible<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_copy_assignable<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_copyable<view_t>::value == true, true);
}

static int test_const(const ebd::fn_ref<int(int, int)> f) { return f(3, 4); }

// BasicAttributes[3]
TEST(BasicAttributes, AlwaysConstFnView) {
    ASSERT_EQ(test_const(ebd_test_free_func_iii_add), 3 + 4);
}

// BasicAttributes[4]
TEST(BasicAttributes, DefaultConfig) {
    static constexpr auto obuf_align = ebd::detail::default_values::owning::alignment;
    static constexpr auto n_obuf_align = ebd::detail::default_values::non_owning::alignment;
    static_assert(
        std::is_same<
            ebd::fn<void(), 0>, 
            ebd::basic_fn<void(), 0, obuf_align, true, false, false, false>
        >::value, 
        "The default config of ebd::fn has been changed!");
    static_assert(
        std::is_same<
            ebd::unique_fn<void(), 0>, 
            ebd::basic_fn<void(), 0, obuf_align, false, false, false, false>
        >::value, 
        "The default config of ebd::unique_fn has been changed!");
    static_assert(
        std::is_same<
            ebd::classic_fn<void(), 0>, 
            ebd::basic_fn<void(), 0, obuf_align, true, false, true, false>
        >::value, 
        "The default config of ebd::classic_fn has been changed!");
    static_assert(
        std::is_same<
            ebd::fn_ref<void(), 0>, 
            ebd::basic_fn<void(), 0, n_obuf_align, true, true, false, false>
        >::value, 
        "The default config of ebd::fn_ref has been changed!");
}

// BasicAttributes[5]
// The `void` and `int[]` are not really incomplete type.
TEST(BasicAttributes, SeemsIncomplete) {
    {
        ebd::fn<int(void)> f1 = [] { return 42; };
        ebd::fn<int(int[])> f2 = [](int[]) { return 43; };

        ASSERT_EQ(f1(), 42);
        ASSERT_EQ(f2(nullptr), 43);
    }
    {
        ebd::unique_fn<int(void)> f1 = [] { return 42; };
        ebd::unique_fn<int(int[])> f2 = [](int[]) { return 43; };

        ASSERT_EQ(f1(), 42);
        ASSERT_EQ(f2(nullptr), 43);
    }
    {
        ebd::classic_fn<int(void)> f1 = [] { return 42; };
        ebd::classic_fn<int(int[])> f2 = [](int[]) { return 43; };

        ASSERT_EQ(f1(), 42);
        ASSERT_EQ(f2(nullptr), 43);
    }
    {
        ebd::fn_ref<int(void)> f1 = +[] { return 42; };
        ebd::fn_ref<int(int[])> f2 = +[](int[]) { return 43; };

        ASSERT_EQ(f1(), 42);
        ASSERT_EQ(f2(nullptr), 43);
    }
}
