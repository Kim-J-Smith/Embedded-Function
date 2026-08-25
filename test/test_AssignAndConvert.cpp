#include "test_function.hpp"

#include <type_traits>

// AssignAndConvert[0]
TEST(AssignAndConvert, small_to_big) {
    auto f_small = ebd::make_fn(ebd_test_free_func_iii_add);
    const size_t s_buf = f_small.get_buffer_size();
    const size_t s_size = sizeof(f_small);
    const size_t b_buf = s_size - sizeof(void*);
    ASSERT_EQ(s_buf < b_buf, true);
    ASSERT_EQ(b_buf < s_size, true);

    ebd::fn<int(int, int) const, b_buf> f_big = f_small;
    ASSERT_EQ(f_big.is_empty(), false);
    ASSERT_EQ(f_big(23665, 8427), 23665 + 8427);

    ebd::unique_fn<int(int, int) const, s_buf> uf_small = f_small;
    ebd::unique_fn<int(int, int) const, b_buf> uf_big = std::move(uf_small);
    ASSERT_EQ(uf_small == nullptr, true);
    ASSERT_EQ(uf_big == nullptr, false);
    ASSERT_EQ(uf_big(23665, 666), 23665 + 666);

    ebd::classic_fn<int(int, int) const, s_buf> cf_small = ebd_test_free_func_iii_add;
    ASSERT_EQ(cf_small != nullptr, true);
    ebd::classic_fn<int(int, int) const, b_buf> cf_big = cf_small;
    ASSERT_EQ(cf_big != nullptr, true);
    ASSERT_EQ(cf_big(23665, 8427), 23665 + 8427);

    ebd::__safe_fn<int() const EBD_TEST_NOEXCEPT, s_buf> sf_small = ebd_test_free_func_noexcept;
    ASSERT_EQ(sf_small != nullptr, true);
    ebd::__safe_fn<int() const EBD_TEST_NOEXCEPT, b_buf> sf_big = sf_small;
    ASSERT_EQ(sf_big != nullptr, true);
    ASSERT_EQ(sf_big(), 0);

    auto small = ebd::make_fn<int(int)>([](int v){ return v * 2; });
    using Big = ebd::fn<int(int), 8 * sizeof(void*)>;
    Big big = small;
    ASSERT_EQ(big(11), 22);
}

// AssignAndConvert[1]
TEST(AssignAndConvert, normal_to_unique) {
    ebd::fn<float(float, float) const> f = ebd_test_free_func_fff_mul;
    ebd::unique_fn<float(float, float) const> uf = f;
    const bool is_copyable_u = std::is_copy_constructible<decltype(uf)>::value;
    ASSERT_EQ(is_copyable_u, false);
    ASSERT_FLOAT_EQ(uf(3.1415926f, 2.7183f), 3.1415926f * 2.7183f);

    auto uf2 = ebd::make_fn(std::move(uf));
    ASSERT_EQ(uf == nullptr, true);
    ASSERT_EQ(uf2 != nullptr, true);
    ASSERT_FLOAT_EQ(uf2(3.1415926f, 2.7183f), 3.1415926f * 2.7183f);
}

// AssignAndConvert[2]
TEST(AssignAndConvert, SameTypeAssign) {
    ebd::fn<int(int)> f1 = [](int x){ return x + 1; };
    ebd::fn<int(int)> f2 = [](int x){ return x + 100; };
    ASSERT_EQ(f1.is_empty(), false);
    ASSERT_EQ(f2.is_empty(), false);
    ASSERT_EQ(f1(2), 3);
    ASSERT_EQ(f2(2), 102);
    f1 = f2;
    ASSERT_EQ(f1(3), 103);
}

// AssignAndConvert[3]
TEST(AssignAndConvert, ClassicFnAssign) {
    ebd::classic_fn<int(int)> f1 = [](int x) { return x + 7; };
    ebd::classic_fn<int(int)> f2 = [](int x) { return x + 9; };
    ASSERT_EQ(f1(1), 8);
    f1 = f2;
    ASSERT_EQ(f1(1), 10);
}

// AssignAndConvert[3.5]
TEST(AssignAndConvert, __SafeFnAssign) {
    ebd::__safe_fn<int(int)> f1 = [](int x) noexcept { return x + 7; };
    ebd::__safe_fn<int(int)> f2 = [](int x) noexcept { return x + 9; };
    ASSERT_EQ(f1(1), 8);
    f1 = f2;
    ASSERT_EQ(f1(1), 10);
}

// AssignAndConvert[4]
TEST(AssignAndConvert, ConstToNonConst) {
    ebd::fn<void()> f_non_const;
    ebd::fn<void() const> f_const;

    f_non_const = f_const; // OK
    // f_const = f_non_const; // Error
}

// AssignAndConvert[5]
TEST(AssignAndConvert, StatelessAssign) {
    {
        ebd::fn<bool(int, int)> f1 = std::less<int>{};
        auto f2 = f1;
        ASSERT_EQ(f1(1, 2), true);
        ASSERT_EQ(f1(2, 1), false);
        ASSERT_EQ(f2(1, 2), true);
        ASSERT_EQ(f2(2, 1), false);
        auto f3 = std::move(f2);
        ASSERT_EQ(f3(1, 2), true);
        ASSERT_EQ(f3(2, 1), false);
    }
    {
        ebd::classic_fn<bool(int, int)> f1 = std::less<int>{};
        auto f2 = f1;
        ASSERT_EQ(f1(1, 2), true);
        ASSERT_EQ(f1(2, 1), false);
        ASSERT_EQ(f2(1, 2), true);
        ASSERT_EQ(f2(2, 1), false);
        auto f3 = std::move(f2);
        ASSERT_EQ(f3(1, 2), true);
        ASSERT_EQ(f3(2, 1), false);
    }
    {
        ebd::__safe_fn<bool(int, int)> f1 = std::less<int>{};
        auto f2 = f1;
        ASSERT_EQ(f1(1, 2), true);
        ASSERT_EQ(f1(2, 1), false);
        ASSERT_EQ(f2(1, 2), true);
        ASSERT_EQ(f2(2, 1), false);
        auto f3 = std::move(f2);
        ASSERT_EQ(f3(1, 2), true);
        ASSERT_EQ(f3(2, 1), false);
    }
    {
        ebd::fn_ref<bool(int, int)> f1 = std::less<int>{};
        auto f2 = f1;
        ASSERT_EQ(f1(1, 2), true);
        ASSERT_EQ(f1(2, 1), false);
        ASSERT_EQ(f2(1, 2), true);
        ASSERT_EQ(f2(2, 1), false);
        auto f3 = std::move(f2);
        ASSERT_EQ(f3(1, 2), true);
        ASSERT_EQ(f3(2, 1), false);
    }
    {
        ebd::unique_fn<bool(int, int)> f1 = std::less<int>{};
        auto f2 = std::move(f1);
        ASSERT_EQ(f2(1, 2), true);
        ASSERT_EQ(f2(2, 1), false);
    }
}

// AssignAndConvert[6]
TEST(AssignAndConvert, VolatileToNonVolatile) {
    {
        ebd::fn<void()> f_non_volatile;
        ebd::fn<void() volatile> f_volatile;

        f_non_volatile = f_volatile; // OK
        // f_volatile = f_non_volatile; // Error
    }
    {
        ebd::unique_fn<void()> f_non_volatile;
        ebd::unique_fn<void() volatile> f_volatile;

        f_non_volatile = std::move(f_volatile); // OK
        // f_volatile = f_non_volatile; // Error
    }
    {
        ebd::classic_fn<void()> f_non_volatile;
        ebd::classic_fn<void() volatile> f_volatile;

        f_non_volatile = f_volatile; // OK
        // f_volatile = f_non_volatile; // Error
    }
    {
        ebd::__safe_fn<void()> f_non_volatile;
        ebd::__safe_fn<void() volatile> f_volatile;

        f_non_volatile = f_volatile; // OK
        // f_volatile = f_non_volatile; // Error
    }
    {
        ebd::fn_ref<void()> f_non_volatile = +[]{};
        ebd::fn_ref<void() volatile> f_volatile = +[]{};

        f_non_volatile = f_volatile; // OK
        // f_volatile = f_non_volatile; // Error
    }
}

// AssignAndConvert[7]
TEST(AssignAndConvert, NonRefToLeftValueRef) {
    {
        ebd::fn<void()> f_non_ref;
        ebd::fn<void() &> f_ref;

        f_ref = f_non_ref; // OK
        // f_non_ref = f_ref; // Error
    }
    {
        ebd::unique_fn<void()> f_non_ref;
        ebd::unique_fn<void() &> f_ref;

        f_ref = std::move(f_non_ref); // OK
        // f_non_ref = f_ref; // Error
    }
    {
        ebd::classic_fn<void()> f_non_ref;
        ebd::classic_fn<void() &> f_ref;

        f_ref = f_non_ref; // OK
        // f_non_ref = f_ref; // Error
    }
    {
        ebd::__safe_fn<void()> f_non_ref;
        ebd::__safe_fn<void() &> f_ref;

        f_ref = f_non_ref; // OK
        // f_non_ref = f_ref; // Error
    }
}

// AssignAndConvert[8]
TEST(AssignAndConvert, NonRefToRightValueRef) {
    {
        ebd::fn<void()> f_non_ref;
        ebd::fn<void() &&> f_ref;

        f_ref = f_non_ref; // OK
        // f_non_ref = f_ref; // Error
    }
    {
        ebd::unique_fn<void()> f_non_ref;
        ebd::unique_fn<void() &&> f_ref;

        f_ref = std::move(f_non_ref); // OK
        // f_non_ref = f_ref; // Error
    }
    {
        ebd::classic_fn<void()> f_non_ref;
        ebd::classic_fn<void() &&> f_ref;

        f_ref = f_non_ref; // OK
        // f_non_ref = f_ref; // Error
    }
    {
        ebd::__safe_fn<void()> f_non_ref;
        ebd::__safe_fn<void() &&> f_ref;

        f_ref = f_non_ref; // OK
        // f_non_ref = f_ref; // Error
    }
}

int ebd_test_counter::m_create_times = 0;
int ebd_test_counter::m_copy_times = 0;
int ebd_test_counter::m_move_times = 0;
int ebd_test_counter::m_delete_times = 0;

// AssignAndConvert[9]
TEST(AssignAndConvert, SelfAssign) {
#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wself-move"
# ifdef __clang__
#  pragma GCC diagnostic ignored "-Wself-assign-overloaded" // only clang
# endif
#elif defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable : 26800) // for /analyze
#endif
    {
        ebd::fn<int(int, int) const> f1 = ebd_test_free_func_iii_add;
        ASSERT_EQ(f1(42, 0), 42);
        f1 = f1;
        ASSERT_EQ(f1(42, 1), 43);
        f1 = std::move(f1);
        ASSERT_EQ(f1(42, 2), 44);

        ebd_test_counter::clear();
        auto f2 = ebd::make_fn<ebd::fn>(ebd_test_counter{});
        ASSERT_EQ(ebd_test_counter::m_create_times, 1);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 1);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 1);

        ebd_test_counter::clear();
        f2 = f2;
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);

        ebd_test_counter::clear();
        f2 = std::move(f2);
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);
    }
    {
        ebd::unique_fn<int(int, int) const> f1 = ebd_test_free_func_iii_add;
        ASSERT_EQ(f1(42, 0), 42);
        f1 = std::move(f1);
        ASSERT_EQ(f1(42, 2), 44);

        ebd_test_counter::clear();
        auto f2 = ebd::make_fn<ebd::unique_fn>(ebd_test_counter{});
        ASSERT_EQ(ebd_test_counter::m_create_times, 1);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 1);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 1);

        ebd_test_counter::clear();
        f2 = std::move(f2);
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);
    }
    {
        ebd::fn_ref<int(int, int) const> f1 = ebd_test_free_func_iii_add;
        ASSERT_EQ(f1(42, 0), 42);
        f1 = f1;
        ASSERT_EQ(f1(42, 1), 43);
        f1 = std::move(f1);
        ASSERT_EQ(f1(42, 2), 44);

        ebd_test_counter::clear();
        auto obj = ebd_test_counter{};
        auto f2 = ebd::make_fn<ebd::fn_ref>(obj);
        ASSERT_EQ(ebd_test_counter::m_create_times, 1);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);

        ebd_test_counter::clear();
        f2 = f2;
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);

        ebd_test_counter::clear();
        f2 = std::move(f2);
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);
    }
    {
        ebd::__safe_fn<int(int, int) const> f1 = ebd_test_free_func_iii_add;
        ASSERT_EQ(f1(42, 0), 42);
        f1 = f1;
        ASSERT_EQ(f1(42, 1), 43);
        f1 = std::move(f1);
        ASSERT_EQ(f1(42, 2), 44);

        ebd_test_counter::clear();
        auto f2 = ebd::make_fn<ebd::__safe_fn>(ebd_test_counter{});
        ASSERT_EQ(ebd_test_counter::m_create_times, 1);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 1);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 1);

        ebd_test_counter::clear();
        f2 = f2;
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);

        ebd_test_counter::clear();
        f2 = std::move(f2);
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);
    }
    {
        ebd::classic_fn<int(int, int) const> f1 = ebd_test_free_func_iii_add;
        ASSERT_EQ(f1(42, 0), 42);
        f1 = f1;
        ASSERT_EQ(f1(42, 1), 43);
        f1 = std::move(f1);
        ASSERT_EQ(f1(42, 2), 44);

        ebd_test_counter::clear();
        auto f2 = ebd::make_fn<ebd::classic_fn>(ebd_test_counter{});
        ASSERT_EQ(ebd_test_counter::m_create_times, 1);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 1);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 1);

        ebd_test_counter::clear();
        f2 = f2;
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);

        ebd_test_counter::clear();
        f2 = std::move(f2);
        ASSERT_EQ(ebd_test_counter::m_create_times, 0);
        ASSERT_EQ(ebd_test_counter::m_delete_times, 0);
        ASSERT_EQ(ebd_test_counter::m_copy_times, 0);
        ASSERT_EQ(ebd_test_counter::m_move_times, 0);
    }
#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic pop
#elif defined(_MSC_VER)
# pragma warning(pop)
#endif
}

// AssignAndConvert[10]
TEST(AssignAndConvert, InterConvert) {
    {
        ebd::fn<int(int, int) const> f = ebd_test_free_func_iii_add;
        auto f1 = ebd::make_fn<ebd::unique_fn>(f);
        auto f2 = ebd::make_fn<ebd::classic_fn>(f);
        auto f3 = ebd::make_fn<ebd::fn_ref>(f);
        static_assert(f.get_buffer_size() == f1.get_buffer_size(), "BUG");
        static_assert(f.get_buffer_size() < f2.get_buffer_size(), "BUG");
        ASSERT_EQ(f(42, 0), 42);
        ASSERT_EQ(f1(42, 0), 42);
        ASSERT_EQ(f2(42, 0), 42);
        ASSERT_EQ(f3(42, 0), 42);
    }
    {
        ebd::unique_fn<int(int, int) const> f = ebd_test_free_func_iii_add;
        auto f3 = ebd::make_fn<ebd::fn_ref>(f);
        ASSERT_EQ(f(42, 0), 42);
        ASSERT_EQ(f3(42, 0), 42);
    }
    {
        ebd::classic_fn<int(int, int) const> f = ebd_test_free_func_iii_add;
        auto f1 = ebd::make_fn<ebd::fn>(f);
        auto f2 = ebd::make_fn<ebd::unique_fn>(f);
        auto f3 = ebd::make_fn<ebd::fn_ref>(f);
        static_assert(f.get_buffer_size() < f1.get_buffer_size(), "BUG");
        static_assert(f.get_buffer_size() < f2.get_buffer_size(), "BUG");
        ASSERT_EQ(f(42, 0), 42);
        ASSERT_EQ(f1(42, 0), 42);
        ASSERT_EQ(f2(42, 0), 42);
        ASSERT_EQ(f3(42, 0), 42);
    }
    {
        ebd::fn_ref<int(int, int) const> f = ebd_test_free_func_iii_add;
        auto f1 = ebd::make_fn<ebd::fn>(f);
        auto f2 = ebd::make_fn<ebd::unique_fn>(f);
        auto f3 = ebd::make_fn<ebd::classic_fn>(f);
        static_assert(f.get_buffer_size() < f1.get_buffer_size(), "BUG");
        static_assert(f.get_buffer_size() < f2.get_buffer_size(), "BUG");
        static_assert(f.get_buffer_size() < f3.get_buffer_size(), "BUG");
        ASSERT_EQ(f(42, 0), 42);
        ASSERT_EQ(f1(42, 0), 42);
        ASSERT_EQ(f2(42, 0), 42);
        ASSERT_EQ(f3(42, 0), 42);
    }
}
