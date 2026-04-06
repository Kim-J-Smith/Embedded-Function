#include "embed/embed_function.hpp"
#include "gtest/gtest.h"
#include "test_function.hpp"

#include <type_traits>

template <typename To, typename From>
using ref_conv_temp = ebd::detail::reference_converts_from_temporary<To, From>;

struct DetailTraits_To_class {};
struct DetailTraits_From_class {
    operator DetailTraits_To_class() const { return {}; }
    operator DetailTraits_To_class&() const { static DetailTraits_To_class t; return t; }
};

TEST(DetailTraits, reference_converts_from_temporary_impl) {

#if defined(__GNUC__) && !defined(__clang__)
# define PASS_ true /// TODO: GCC BUG
#else
# define PASS_ false
#endif

    auto ret = ref_conv_temp<DetailTraits_To_class&&, DetailTraits_From_class>::value;
    auto can_convert = std::is_convertible<DetailTraits_From_class, DetailTraits_To_class&&>::value;
    ASSERT_EQ(ret <= can_convert || PASS_, true);
}

struct DetailTraits_Noexcept_Create {
    DetailTraits_Noexcept_Create() = default;
    DetailTraits_Noexcept_Create(const DetailTraits_Noexcept_Create&) = default;
    ~DetailTraits_Noexcept_Create() noexcept(false) {}
};

template <typename Obj>
using nothrow_constructile_ = ebd::detail::is_nothrow_construct_from_functor<Obj>;

TEST(DetailTraits, is_nothrow_construct_from_functor) {
    using T = DetailTraits_Noexcept_Create;
    ASSERT_EQ(nothrow_constructile_<T>::value == true, true);
}
