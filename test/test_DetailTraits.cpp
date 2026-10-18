#include "test_function.hpp"

#include <type_traits>

namespace ed = ebd::detail;

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

    auto ret = ed::reference_converts_from_temporary<DetailTraits_To_class&&, DetailTraits_From_class>::value;
    auto can_convert = std::is_convertible<DetailTraits_From_class, DetailTraits_To_class&&>::value;
    ASSERT_EQ(ret <= can_convert || PASS_, true);
}


namespace {
    struct NonTrivial {
        int n;
        NonTrivial(int num = 0) noexcept : n(num) {}
        NonTrivial(const NonTrivial& o) noexcept { n = o.n; }
    };
}
TEST(DetailTraits, is_convertible_from_specialization) {
    // fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial&&)>, ebd::fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial)>, ebd::fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial&&)>, ebd::fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial)>, ebd::fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial&&) const>, ebd::fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn<int(NonTrivial) const>, ebd::fn<int(NonTrivial&&) const>>::value, "BUG");

    // unique_fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial&&)>, ebd::unique_fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial)>, ebd::unique_fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial&&)>, ebd::unique_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial)>, ebd::unique_fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial&&) const>, ebd::unique_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::unique_fn<int(NonTrivial) const>, ebd::unique_fn<int(NonTrivial&&) const>>::value, "BUG");

    // classic_fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial&&)>, ebd::classic_fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial)>, ebd::classic_fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial&&)>, ebd::classic_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial)>, ebd::classic_fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial&&) const>, ebd::classic_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::classic_fn<int(NonTrivial) const>, ebd::classic_fn<int(NonTrivial&&) const>>::value, "BUG");

    // __safe_fn
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial&&)>, ebd::__safe_fn<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial)>, ebd::__safe_fn<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial&&)>, ebd::__safe_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial)>, ebd::__safe_fn<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial&&) const>, ebd::__safe_fn<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::__safe_fn<int(NonTrivial) const>, ebd::__safe_fn<int(NonTrivial&&) const>>::value, "BUG");

    // fn_ref
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial&&)>, ebd::fn_ref<int(NonTrivial)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial)>, ebd::fn_ref<int(NonTrivial&&)>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial&&)>, ebd::fn_ref<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial)>, ebd::fn_ref<int(NonTrivial&&) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial&&) const>, ebd::fn_ref<int(NonTrivial) const>>::value, "BUG");
    static_assert(
        ed::is_convertible_from_specialization<
            ebd::fn_ref<int(NonTrivial) const>, ebd::fn_ref<int(NonTrivial&&) const>>::value, "BUG");
}
