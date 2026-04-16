#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

#include <type_traits>

// BasicAttributes[0]
TEST(BasicAttributes, SizeAndAlign) {
    using f_t = ebd::fn<void()>;
    using uf_t = ebd::unique_fn<void()>;
    using sf_t = ebd::safe_fn<void()>;
    using fv_t = ebd::fn_view<void()>;

    ASSERT_EQ(f_t::get_buffer_size() == ebd::detail::default_buffer_size::value, true);
    ASSERT_EQ(uf_t::get_buffer_size() == ebd::detail::default_buffer_size::value, true);
    ASSERT_EQ(sf_t::get_buffer_size() == ebd::detail::default_buffer_size::value, true);
    ASSERT_EQ(fv_t::get_buffer_size() == ebd::detail::default_buffer_size::view_buf, true);

    ASSERT_EQ(alignof(f_t) >= alignof(void*), true);
    ASSERT_EQ(alignof(uf_t) >= alignof(void*), true);
    ASSERT_EQ(alignof(sf_t) >= alignof(void*), true);
    ASSERT_EQ(alignof(fv_t) >= alignof(void*), true);

    ASSERT_EQ(sizeof(f_t) - f_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(uf_t) - uf_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(sf_t) - sf_t::get_buffer_size(), 2 * sizeof(void*));
    ASSERT_EQ(sizeof(fv_t) - fv_t::get_buffer_size(), sizeof(void*));
}

// BasicAttributes[1]
TEST(BasicAttributes, AbilityAndNoexcept) {
    using f_t = ebd::fn<void()>;
    using uf_t = ebd::unique_fn<void()>;
    using sf_t = ebd::safe_fn<void()>;
    using fv_t = ebd::fn_view<void()>;

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

    // sf_t
    ASSERT_EQ(std::is_move_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_copy_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_constructible<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_assignable<sf_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_destructible<sf_t>::value == true, true);

    // fv_t
    ASSERT_EQ(std::is_move_constructible<fv_t>::value == true, true);
    ASSERT_EQ(std::is_move_assignable<fv_t>::value == true, true);
    ASSERT_EQ(std::is_copy_constructible<fv_t>::value == true, true);
    ASSERT_EQ(std::is_copy_assignable<fv_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_constructible<fv_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_copy_assignable<fv_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_constructible<fv_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_move_assignable<fv_t>::value == true, true);
    ASSERT_EQ(std::is_nothrow_destructible<fv_t>::value == true, true);
}

// BasicAttributes[2]
TEST(BasicAttributes, TrivialityViewMode) {
    using view_t = ebd::fn_view<void()>;
    ASSERT_EQ(std::is_trivially_destructible<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_move_constructible<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_move_assignable<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_copy_constructible<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_copy_assignable<view_t>::value == true, true);
    ASSERT_EQ(std::is_trivially_copyable<view_t>::value == true, true);
}

static int test_const(const ebd::fn_view<int(int, int)> f) { return f(3, 4); }

// BasicAttributes[3]
TEST(BasicAttributes, AlwaysConstFnView) {
    ASSERT_EQ(test_const(ebd_test_free_func_iii_add), 3 + 4);
}
