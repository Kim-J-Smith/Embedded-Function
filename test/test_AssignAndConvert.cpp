#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

#include <type_traits>

// AssignAndConvert[0]
TEST(AssignAndConvert, small_to_big) {
    auto f_small = ebd::make_fn(ebd_test_free_func_iii_add);
    const size_t s_buf = f_small.buffer_size;
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

    ebd::safe_fn<int(int, int) const EBD_TEST_NOEXCEPT, s_buf> sf_small = 
        ebd_test_free_func_iii_add;
    ASSERT_EQ(sf_small != nullptr, true);
    ebd::safe_fn<int(int, int) const EBD_TEST_NOEXCEPT, b_buf> sf_big = sf_small;
    ASSERT_EQ(sf_big != nullptr, true);
    ASSERT_EQ(sf_big(1234, 5678), 1234 + 5678);

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
TEST(AssignAndConvert, SafeFnAssign) {
    ebd::safe_fn<int(int)> f1 = [](int x) noexcept { return x + 7; };
    ebd::safe_fn<int(int)> f2 = [](int x) noexcept { return x + 9; };
    ASSERT_EQ(f1(1), 8);
    f1 = f2;
    ASSERT_EQ(f1(1), 10);
}
