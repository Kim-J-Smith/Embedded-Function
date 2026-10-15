#include "test_function.hpp"

#include <type_traits>

// BasicAttributes[0]
TEST(BasicAttributes, SizeAndAlign) {
    using f_t = ebd::fn<void()>;
    using uf_t = ebd::unique_fn<void()>;
    using cf_t = ebd::classic_fn<void()>;
    using sf_t = ebd::__safe_fn<void()>;
    using fr_t = ebd::fn_ref<void()>;

    static_assert(f_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size,
        "The buffer size of ebd::fn has been changed!");
    static_assert(uf_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size,
        "The buffer size of ebd::unique_fn has been changed!");
    static_assert(cf_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size,
        "The buffer size of ebd::classic_fn has been changed!");
    static_assert(sf_t::get_buffer_size() == ebd::detail::default_values::owning::buffer_size,
        "The buffer size of ebd::__safe_fn has been changed!");
    static_assert(fr_t::get_buffer_size() == ebd::detail::default_values::non_owning::buffer_size,
        "The buffer size of ebd::fn_ref has been changed!");

    static_assert(alignof(f_t) == ebd::detail::default_values::owning::alignment,
        "The alignment of ebd::fn has been changed!");
    static_assert(alignof(uf_t) == ebd::detail::default_values::owning::alignment,
        "The alignment of ebd::unique_fn has been changed!");
    static_assert(alignof(cf_t) == ebd::detail::default_values::owning::alignment,
        "The alignment of ebd::classic_fn has been changed!");
    static_assert(alignof(sf_t) == ebd::detail::default_values::owning::alignment,
        "The alignment of ebd::__safe_fn has been changed!");
    static_assert(alignof(fr_t) == alignof(void(*)()),
        "The alignment of ebd::fn_ref has been changed!");

    static_assert(sizeof(f_t) - f_t::get_buffer_size() == 2 * sizeof(void*),
        "The non-buffer overhead of ebd::fn has been changed!");
    static_assert(sizeof(uf_t) - uf_t::get_buffer_size() == 2 * sizeof(void*),
        "The non-buffer overhead of ebd::unique_fn has been changed!");
    static_assert(sizeof(cf_t) - cf_t::get_buffer_size() == 2 * sizeof(void*),
        "The non-buffer overhead of ebd::classic_fn has been changed!");
    static_assert(sizeof(sf_t) - sf_t::get_buffer_size() == 2 * sizeof(void*),
        "The non-buffer overhead of ebd::__safe_fn has been changed!");
    static_assert(sizeof(fr_t) - fr_t::get_buffer_size() == sizeof(void*),
        "The non-buffer overhead of ebd::fn_ref has been changed!");
}

// BasicAttributes[1]
TEST(BasicAttributes, AbilityAndNoexcept) {
    using f_t = ebd::fn<void()>;
    using uf_t = ebd::unique_fn<void()>;
    using cf_t = ebd::classic_fn<void()>;
    using sf_t = ebd::__safe_fn<void()>;
    using fr_t = ebd::fn_ref<void()>;

    // f_t
    static_assert(std::is_move_constructible<f_t>::value,
        "ebd::fn should be move constructible!");
    static_assert(std::is_move_assignable<f_t>::value,
        "ebd::fn should be move assignable!");
    static_assert(std::is_copy_constructible<f_t>::value,
        "ebd::fn should be copy constructible!");
    static_assert(std::is_copy_assignable<f_t>::value,
        "ebd::fn should be copy assignable!");
    static_assert(!std::is_nothrow_copy_constructible<f_t>::value,
        "ebd::fn's copy constructor should NOT be noexcept!");
    static_assert(!std::is_nothrow_copy_assignable<f_t>::value,
        "ebd::fn's copy assignment should NOT be noexcept!");
    static_assert(std::is_nothrow_destructible<f_t>::value,
        "ebd::fn's destructor should be noexcept!");

    // uf_t
    static_assert(std::is_move_constructible<uf_t>::value,
        "ebd::unique_fn should be move constructible!");
    static_assert(std::is_move_assignable<uf_t>::value,
        "ebd::unique_fn should be move assignable!");
    static_assert(!std::is_copy_constructible<uf_t>::value,
        "ebd::unique_fn should NOT be copy constructible!");
    static_assert(!std::is_copy_assignable<uf_t>::value,
        "ebd::unique_fn should NOT be copy assignable!");
    static_assert(!std::is_nothrow_move_constructible<uf_t>::value,
        "ebd::unique_fn's move constructor should NOT be noexcept!");
    static_assert(!std::is_nothrow_move_assignable<uf_t>::value,
        "ebd::unique_fn's move assignment should NOT be noexcept!");
    static_assert(std::is_nothrow_destructible<uf_t>::value,
        "ebd::unique_fn's destructor should be noexcept!");

    // cf_t (classic_fn is copyable, like fn)
    static_assert(std::is_move_constructible<cf_t>::value,
        "ebd::classic_fn should be move constructible!");
    static_assert(std::is_move_assignable<cf_t>::value,
        "ebd::classic_fn should be move assignable!");
    static_assert(std::is_copy_constructible<cf_t>::value,
        "ebd::classic_fn should be copy constructible!");
    static_assert(std::is_copy_assignable<cf_t>::value,
        "ebd::classic_fn should be copy assignable!");
    static_assert(!std::is_nothrow_copy_constructible<cf_t>::value,
        "ebd::classic_fn's copy constructor should NOT be noexcept!");
    static_assert(!std::is_nothrow_copy_assignable<cf_t>::value,
        "ebd::classic_fn's copy assignment should NOT be noexcept!");
    static_assert(std::is_nothrow_destructible<cf_t>::value,
        "ebd::classic_fn's destructor should be noexcept!");

    // sf_t (__safe_fn asserts nothrow, so copy/move/destroy are nothrow)
    static_assert(std::is_move_constructible<sf_t>::value,
        "ebd::__safe_fn should be move constructible!");
    static_assert(std::is_move_assignable<sf_t>::value,
        "ebd::__safe_fn should be move assignable!");
    static_assert(std::is_copy_constructible<sf_t>::value,
        "ebd::__safe_fn should be copy constructible!");
    static_assert(std::is_copy_assignable<sf_t>::value,
        "ebd::__safe_fn should be copy assignable!");
    static_assert(std::is_nothrow_copy_constructible<sf_t>::value,
        "ebd::__safe_fn's copy constructor should be noexcept!");
    static_assert(std::is_nothrow_copy_assignable<sf_t>::value,
        "ebd::__safe_fn's copy assignment should be noexcept!");
    static_assert(std::is_nothrow_move_constructible<sf_t>::value,
        "ebd::__safe_fn's move constructor should be noexcept!");
    static_assert(std::is_nothrow_move_assignable<sf_t>::value,
        "ebd::__safe_fn's move assignment should be noexcept!");
    static_assert(std::is_nothrow_destructible<sf_t>::value,
        "ebd::__safe_fn's destructor should be noexcept!");

    // fr_t
    static_assert(std::is_move_constructible<fr_t>::value,
        "ebd::fn_ref should be move constructible!");
    static_assert(std::is_move_assignable<fr_t>::value,
        "ebd::fn_ref should be move assignable!");
    static_assert(std::is_copy_constructible<fr_t>::value,
        "ebd::fn_ref should be copy constructible!");
    static_assert(std::is_copy_assignable<fr_t>::value,
        "ebd::fn_ref should be copy assignable!");
    static_assert(std::is_nothrow_copy_constructible<fr_t>::value,
        "ebd::fn_ref's copy constructor should be noexcept!");
    static_assert(std::is_nothrow_copy_assignable<fr_t>::value,
        "ebd::fn_ref's copy assignment should be noexcept!");
    static_assert(std::is_nothrow_move_constructible<fr_t>::value,
        "ebd::fn_ref's move constructor should be noexcept!");
    static_assert(std::is_nothrow_move_assignable<fr_t>::value,
        "ebd::fn_ref's move assignment should be noexcept!");
    static_assert(std::is_nothrow_destructible<fr_t>::value,
        "ebd::fn_ref's destructor should be noexcept!");
}

// BasicAttributes[2]
TEST(BasicAttributes, TrivialityViewMode) {
    using view_t = ebd::fn_ref<void()>;
    static_assert(std::is_trivially_destructible<view_t>::value,
        "ebd::fn_ref should be trivially destructible!");
    static_assert(std::is_trivially_move_constructible<view_t>::value,
        "ebd::fn_ref should be trivially move constructible!");
    static_assert(std::is_trivially_move_assignable<view_t>::value,
        "ebd::fn_ref should be trivially move assignable!");
    static_assert(std::is_trivially_copy_constructible<view_t>::value,
        "ebd::fn_ref should be trivially copy constructible!");
    static_assert(std::is_trivially_copy_assignable<view_t>::value,
        "ebd::fn_ref should be trivially copy assignable!");
    static_assert(std::is_trivially_copyable<view_t>::value,
        "ebd::fn_ref should be trivially copyable!");
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
