#include "test_function.hpp"

namespace {

    struct ThrowInDefaultCtor {
        ThrowInDefaultCtor() noexcept(false) {}
        void operator()() {}
    };

    struct InplaceThrowOrNot {
        void operator()() {}
        InplaceThrowOrNot(std::initializer_list<int>) {}
        InplaceThrowOrNot(std::initializer_list<double>) noexcept {}
        InplaceThrowOrNot(std::initializer_list<int>, int) {}
        InplaceThrowOrNot(std::initializer_list<double>, double) noexcept {}
    };
}

TEST(Conformance_fn, noexcept_make_fn_pass) {
    auto lbd1 = [] {};
    auto lbd2 = []() noexcept {};
    auto lbd3 = []() noexcept(false) {};
    static_assert(noexcept(ebd::make_fn(lbd1)), "BUG");
    static_assert(noexcept(ebd::make_fn(lbd2)), "BUG");
    static_assert(noexcept(ebd::make_fn(lbd3)), "BUG");

    ThrowInDefaultCtor throw_in_default_ctor;
    static_assert(!std::is_nothrow_default_constructible<ThrowInDefaultCtor>::value, "BUG");
    static_assert(!noexcept(ebd::make_fn(ThrowInDefaultCtor{})), "BUG");
    static_assert(noexcept(ebd::make_fn(throw_in_default_ctor)), "BUG");

#if EMBED_CXX_VERSION >= 201703L

    constexpr auto InPlace = std::in_place_type<InplaceThrowOrNot>;

    static_assert(!noexcept(ebd::make_fn(InPlace, {0})), "BUG");
    static_assert(noexcept(ebd::make_fn(InPlace, {0.0})), "BUG");
    static_assert(!noexcept(ebd::make_fn(InPlace, {0}, 1)), "BUG");
    static_assert(noexcept(ebd::make_fn(InPlace, {0.0}, 1.0)), "BUG");

#endif
}

