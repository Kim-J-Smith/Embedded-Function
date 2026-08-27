#include "test_function.hpp"

#include <iostream>

TEST(UnaryDereference, fn_freeFunction) {
    auto f1 = ebd::make_fn(ebd_test_free_func_v);
    ASSERT_EQ(*f1, &ebd_test_free_func_v);

    auto f2 = ebd::make_fn(ebd_test_free_func_vii);
    ASSERT_EQ(*f2, &ebd_test_free_func_vii);

    const auto f3 = ebd::make_fn(ebd_test_free_func_iii_add);
    ASSERT_EQ(*f3, &ebd_test_free_func_iii_add);

    auto f4 = ebd::make_fn([]{ return 1; });
    ASSERT_EQ(*f4 == nullptr, true);

    auto f5 = ebd::make_fn(+[]{ return 1; });
    ASSERT_EQ(*f5 == nullptr, false);

    auto f6 = ebd::make_fn(&ebd_test_member_fn::mem_fn_ii_add);
    ASSERT_EQ(*f6 == nullptr, true);

    auto f7 = ebd::unique_fn<void()>(ebd_test_free_func_v);
    ASSERT_EQ(*f7 == nullptr, false);
}

TEST(UnaryDereference, fn_ref_freeFunction) {
    ebd::fn_ref<void()> f1 = ebd_test_free_func_v;
    ASSERT_EQ(*f1, &ebd_test_free_func_v);
}

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )

TEST(UnaryDereference, noexcept_qualifier) {
    {
        auto f1 = ebd::make_fn<ebd::fn>(ebd_test_free_func_iii_add_noexcept);
        static_assert(std::is_same_v<decltype(*f1), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f1, &ebd_test_free_func_iii_add_noexcept);

        auto f2 = ebd::make_fn<ebd::fn, int(int&, int&)>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f2), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f2, nullptr);

        auto f3 = ebd::make_fn<ebd::fn, int(int&, int&) const>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f3), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f3, nullptr);

        auto f4 = ebd::make_fn<ebd::fn, int(int&, int&) volatile>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f4), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f4, nullptr);
    }
    {
        auto f1 = ebd::make_fn<ebd::unique_fn>(ebd_test_free_func_iii_add_noexcept);
        static_assert(std::is_same_v<decltype(*f1), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f1, &ebd_test_free_func_iii_add_noexcept);

        auto f2 = ebd::make_fn<ebd::unique_fn, int(int&, int&)>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f2), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f2, nullptr);

        auto f3 = ebd::make_fn<ebd::unique_fn, int(int&, int&) const>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f3), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f3, nullptr);

        auto f4 = ebd::make_fn<ebd::unique_fn, int(int&, int&) volatile>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f4), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f4, nullptr);
    }
    {
        // classic_fn is special.
        auto f1 = ebd::make_fn<ebd::classic_fn>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f1), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f1, nullptr);

        auto f2 = ebd::make_fn<ebd::classic_fn, int(int&, int&)>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f2), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f2, nullptr);

        auto f3 = ebd::make_fn<ebd::classic_fn, int(int&, int&) const>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f3), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f3, nullptr);

        auto f4 = ebd::make_fn<ebd::classic_fn, int(int&, int&) volatile>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f4), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f4, nullptr);
    }
    {
        auto f1 = ebd::make_fn<ebd::fn_ref>(ebd_test_free_func_iii_add_noexcept);
        static_assert(std::is_same_v<decltype(*f1), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f1, &ebd_test_free_func_iii_add_noexcept);

        auto f2 = ebd::make_fn<ebd::fn_ref, int(int&, int&)>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f2), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f2, nullptr);

        auto f3 = ebd::make_fn<ebd::fn_ref, int(int&, int&) const>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f3), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f3, nullptr);

        auto f4 = ebd::make_fn<ebd::fn_ref, int(int&, int&) volatile>(ebd_test_free_func_iii_add_noexcept);
        static_assert(!std::is_same_v<decltype(*f4), decltype(&ebd_test_free_func_iii_add_noexcept)>, "BUG");
        ASSERT_EQ(*f4, nullptr);
    }
}

#endif
