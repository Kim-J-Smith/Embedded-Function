#include "__constant_wrapper.hpp"
#include "test_function.hpp"

#if __cpp_lib_constant_wrapper >= 202603L

namespace {
    
struct ExplicitThis {
    int add(this ExplicitThis&, int a, int b) {
        return a + b;
    }
};

struct ListInit {
    ListInit() = delete;
    ListInit(std::initializer_list<int>& il) : buf(il) {}
    std::vector<int> buf;
    int operator()() {
        int result = 0;
        for (auto& i : buf) { result += i; }
        return result;
    }
    int sum() { return this->operator()(); }
};

static int func_iii_add_noexcept(int a, int b) noexcept {
    return a + b;
}

struct NonConstInvocable {
    void operator()(int*) noexcept {}
};

struct MoveOnly {
    MoveOnly() = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) = default;

    int add_42(int n) const { return 42 + n; }
};

using Normal = ebd_test_member_fn;

struct Int {
  int i;
  constexpr Int(int ii) noexcept : i(ii) {}
};

struct NeedsConversion {
  int operator()(Int x, Int y, Int z) const noexcept { return x.i + y.i + z.i; }
};

struct LeftRightCallable {
    int add_42_left(int n) const & noexcept { return n + 42; };
    int add_42_right(int n) const && noexcept { return n + 42; };
};

static_assert(std::is_invocable_v<decltype(&LeftRightCallable::add_42_left), LeftRightCallable, int>);
static_assert(std::is_invocable_v<decltype(&LeftRightCallable::add_42_left), LeftRightCallable&, int>);
static_assert(std::is_invocable_v<decltype(&LeftRightCallable::add_42_left), LeftRightCallable&&, int>);

using CwNeedConversion = std::constant_wrapper<NeedsConversion{}>;
using CwMemberFunction = std::constant_wrapper<&Normal::mem_fn_ii_add>;
using CwNonConstInvocable = std::constant_wrapper<NonConstInvocable{}>;
using CwLeftRightCallable = std::constant_wrapper<LeftRightCallable{}>;

}

// member function
static_assert(
    std::is_constructible_v<ebd::fn<int(int, int)>, CwMemberFunction, Normal>);
static_assert(
    !std::is_nothrow_constructible_v<ebd::fn<int(int, int)>,CwMemberFunction, Normal>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(int, int) const>, CwMemberFunction, Normal>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(int, int) noexcept>, CwMemberFunction, Normal>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(int, int) const noexcept>, CwMemberFunction, Normal>);

// non-const-invocable functor
static_assert(
    !std::is_constructible_v<ebd::fn<void()>, CwNonConstInvocable, int*>);
static_assert(
    !std::is_constructible_v<ebd::fn<void() const>, CwNonConstInvocable, int*>);
static_assert(
    !std::is_constructible_v<ebd::fn<void() noexcept>, CwNonConstInvocable, int*>);

// in-place
static_assert(std::is_constructible_v<
    ebd::fn<int(), sizeof(ListInit)>,
    std::constant_wrapper<&ListInit::sum>, std::in_place_type_t<ListInit>, std::initializer_list<int>&>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(), sizeof(ListInit)>, std::constant_wrapper<&ListInit::sum>, std::in_place_type_t<ListInit>>);
static_assert(!std::is_constructible_v<
    ebd::fn<int(), sizeof(ListInit)>,
    std::constant_wrapper<&ListInit::sum>, std::in_place_type_t<ListInit>, std::initializer_list<float>&>);

/// need-conversion

// non-qualifier
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int)>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int)>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int)>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int)>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int)>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int)>, CwNeedConversion, std::in_place_type_t<Int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int)>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int)>, CwNeedConversion, std::in_place_type_t<Int>, int>);

// const-qualifier
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int) const>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int) const>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const>, CwNeedConversion, std::in_place_type_t<Int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int) const>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int) const>, CwNeedConversion, std::in_place_type_t<Int>, int>);

// noexcept-qualifier
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) noexcept>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) noexcept>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int) noexcept>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int) noexcept>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) noexcept>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) noexcept>, CwNeedConversion, std::in_place_type_t<Int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int) noexcept>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int) noexcept>, CwNeedConversion, std::in_place_type_t<Int>, int>);

// const & noexcept-qualifier
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const noexcept>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const noexcept>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int) const noexcept>, CwNeedConversion, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int) const noexcept>, CwNeedConversion, Int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const noexcept>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(int, int) const noexcept>, CwNeedConversion, std::in_place_type_t<Int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, int) const noexcept>, CwNeedConversion, std::in_place_type_t<int>, int>);
static_assert(
    std::is_constructible_v<ebd::fn<Int(Int, Int) const noexcept>, CwNeedConversion, std::in_place_type_t<Int>, int>);

TEST(Conformance_fn, constant_wrapper_pass) {
    {
        using Class = ebd_test_member_fn;
        Class obj{};
        auto* ptr = &obj;

        {
            ebd::fn<int(int, int)> f1(std::cw<&Class::mem_fn_ii_add>, obj);
            ASSERT_EQ(f1(0, 42), 42);
            ASSERT_EQ(f1(1, 42), 43);

            ebd::fn<int(int, int)> f2(std::cw<&Class::mem_fn_ii_add>, ptr);
            ASSERT_EQ(f2(0, 42), 42);
            ASSERT_EQ(f2(1, 42), 43);
        }
        {
            ebd::fn<int() noexcept> f1(std::cw<&Class::mem_fn_noexcept>, obj);
            ASSERT_EQ(f1(), 0);
            ebd::fn<int() noexcept> f2(std::cw<&Class::mem_fn_noexcept>, ptr);
            ASSERT_EQ(f2(), 0);
        }
        {
            ebd::fn<int() &&> f1(std::cw<&Class::mem_fn_rref_callable>, obj);
            ASSERT_EQ(std::move(f1)(), OVL_R_REF);

            // ERROR
            // ebd::fn<int() &&> f2(std::cw<&Class::mem_fn_rref_callable>, ptr);
            // ASSERT_EQ(std::move(f2)(), OVL_R_REF);
        }
        {
            // non-qualifier
            int a = 42;
            ebd::fn<int(int)> f1(std::cw<&ebd_test_free_func_iii_add>, a);
            ASSERT_EQ(f1(1), 43);

            ebd::fn<int(int)> f2(std::cw<[](int a, int b) { return a + b; }>, a);
            ASSERT_EQ(f2(2), 44);

            ebd::fn<int(int)> f3(std::cw<+[](int a, int b) { return a + b; }>, a);
            ASSERT_EQ(f3(3), 45);
        }
        {
            // const-qualifier
            int a = 42;
            ebd::fn<int(int) const> f1(std::cw<&ebd_test_free_func_iii_add>, a);
            ASSERT_EQ(f1(1), 43);

            ebd::fn<int(int) const> f2(std::cw<[](int a, int b) { return a + b; }>, a);
            ASSERT_EQ(f2(2), 44);

            ebd::fn<int(int) const> f3(std::cw<+[](int a, int b) { return a + b; }>, a);
            ASSERT_EQ(f3(3), 45);
        }
        {
            // noexcept-qualifier
            int a = 42;
            ebd::fn<int(int) noexcept> f1(std::cw<&func_iii_add_noexcept>, a);
            ASSERT_EQ(f1(1), 43);

            ebd::fn<int(int) noexcept> f2(std::cw<[](int a, int b) noexcept { return a + b; }>, a);
            ASSERT_EQ(f2(2), 44);

            ebd::fn<int(int) noexcept> f3(std::cw<+[](int a, int b) noexcept { return a + b; }>, a);
            ASSERT_EQ(f3(3), 45);
        }
        {
            // const & noexcept-qualifier
            int a = 42;
            ebd::fn<int(int) const noexcept> f1(std::cw<&func_iii_add_noexcept>, a);
            ASSERT_EQ(f1(1), 43);

            ebd::fn<int(int) const noexcept> f2(std::cw<[](int a, int b) noexcept { return a + b; }>, a);
            ASSERT_EQ(f2(2), 44);

            ebd::fn<int(int) const noexcept> f3(std::cw<+[](int a, int b) noexcept { return a + b; }>, a);
            ASSERT_EQ(f3(3), 45);
        }
    }
    {
        {
            // move-only
            auto f = ebd::make_fn(std::cw<&MoveOnly::add_42>, MoveOnly{});
            static_assert(std::is_invocable_v<decltype(f), int>);
            static_assert(std::is_invocable_v<decltype(f) const, int>);
            static_assert(std::is_invocable_v<decltype(f) &, int>);
            static_assert(std::is_invocable_v<decltype(f) &&, int>);
            static_assert(std::is_invocable_v<decltype(f) const&, int>);
            static_assert(std::is_invocable_v<decltype(f) const&&, int>);
            static_assert(!std::is_nothrow_invocable_v<decltype(f), int>);
            static_assert(!std::is_nothrow_invocable_v<decltype(f) const, int>);
            static_assert(!std::is_nothrow_invocable_v<decltype(f) &, int>);
            static_assert(!std::is_nothrow_invocable_v<decltype(f) &&, int>);
            static_assert(!std::is_nothrow_invocable_v<decltype(f) const&, int>);
            static_assert(!std::is_nothrow_invocable_v<decltype(f) const&&, int>);
            static_assert(!f.is_copyable());
            ASSERT_EQ(f(1), 43);
            ASSERT_EQ(f(2), 44);
            ASSERT_EQ(f(3), 45);
            ASSERT_EQ(f(4), 46);
        }
    }

#if !defined(__clang__) || defined(EBD_TEST_TRY_BUG__Clang_106660)
    // Clang bug <https://github.com/llvm/llvm-project/issues/106660>
    {
        ExplicitThis obj;

        auto f1 = ebd::make_fn(std::cw<&ExplicitThis::add>, obj);
        ASSERT_EQ(f1(0, 42), 42);
        ASSERT_EQ(f1(1, 42), 43);

        // ERROR
        // auto f1 = ebd::make_fn(std::cw<&ExplicitThis::add>, &obj);
        // ASSERT_EQ(f1(0, 42), 42);
        // ASSERT_EQ(f1(1, 42), 43);
    }
#endif

    {
        {
            ebd::fn<int(int) const noexcept, sizeof(int)> f(std::cw<&func_iii_add_noexcept>, std::in_place_type<int>, 42);
            ASSERT_EQ(f(0), 42);
            ASSERT_EQ(f(1), 43);

            auto f_auto = ebd::make_fn(std::cw<&func_iii_add_noexcept>, std::in_place_type<int>, 42);
            ASSERT_EQ(f_auto(0), 42);
            ASSERT_EQ(f_auto(1), 43);

            static_assert(std::is_same_v<decltype(f_auto), decltype(f)>);
        }
        {
            ebd::fn<int(int, int), sizeof(Normal)> f(std::cw<&Normal::mem_fn_ii_add>, std::in_place_type<Normal>);
            ASSERT_EQ(f(42, 0), 42);
            ASSERT_EQ(f(42, 1), 43);

            auto f_auto = ebd::make_fn(std::cw<&Normal::mem_fn_ii_add>, std::in_place_type<Normal>);
            ASSERT_EQ(f_auto(42, 0), 42);
            ASSERT_EQ(f_auto(42, 1), 43);

            static_assert(std::is_same_v<decltype(f_auto), decltype(f)>);
        }
    }
    {
        ebd::fn<int(), sizeof(ListInit)> f(std::cw<&ListInit::sum>, std::in_place_type<ListInit>, {1, 3, 42});
        ASSERT_EQ(f(), 46);

        auto f_auto = ebd::make_fn(std::cw<&ListInit::sum>, std::in_place_type<ListInit>, {1, 3, 42});
        ASSERT_EQ(f_auto(), 46);

        static_assert(std::is_same_v<decltype(f_auto), decltype(f)>);
    }

    {
        {
            ebd::fn<Int(int, int)> f(std::cw<NeedsConversion{}>, 3);
            ASSERT_EQ(f(42, -3).i, 42);
            ASSERT_EQ(f(42, 0).i, 45);
        }
        {
            ebd::fn<Int(int, int) const> f(std::cw<NeedsConversion{}>, 3);
            ASSERT_EQ(f(42, -3).i, 42);
            ASSERT_EQ(f(42, 0).i, 45);
        }
        {
            ebd::fn<Int(int, int) noexcept> f(std::cw<NeedsConversion{}>, 3);
            ASSERT_EQ(f(42, -3).i, 42);
            ASSERT_EQ(f(42, 0).i, 45);
        }
        {
            ebd::fn<Int(int, int) const noexcept> f(std::cw<NeedsConversion{}>, 3);
            ASSERT_EQ(f(42, -3).i, 42);
            ASSERT_EQ(f(42, 0).i, 45);
        }
        {
            ebd::fn<int(Int, Int) const noexcept, sizeof(int)> f(std::cw<NeedsConversion{}>, 3);
            ASSERT_EQ(f(42, -3), 42);
            ASSERT_EQ(f(42, 0), 45);

            auto f_auto = ebd::make_fn(std::cw<NeedsConversion{}>, 3);
            ASSERT_EQ(f_auto(42, -3), 42);
            ASSERT_EQ(f_auto(42, 0), 45);

            static_assert(std::is_same_v<decltype(f), decltype(f_auto)>);
        }
    }

    {
        {
            ebd::fn<int(int) &> f(std::cw<&LeftRightCallable::add_42_left>, LeftRightCallable{});
            ASSERT_EQ(f(0), 42);
            ASSERT_EQ(f(1), 43);
            ASSERT_EQ(f(2), 44);
        }
        {
            ebd::fn<int(int) &&> f(std::cw<&LeftRightCallable::add_42_right>, LeftRightCallable{});
            ASSERT_EQ(std::move(f)(0), 42);
            ASSERT_EQ(std::move(f)(1), 43);
            ASSERT_EQ(std::move(f)(2), 44);
        }
        {
            ebd::fn<int(int) const &> f(std::cw<&LeftRightCallable::add_42_left>, LeftRightCallable{});
            ASSERT_EQ(f(0), 42);
            ASSERT_EQ(f(1), 43);
            ASSERT_EQ(f(2), 44);
        }
        {
            ebd::fn<int(int) const &&> f(std::cw<&LeftRightCallable::add_42_right>, LeftRightCallable{});
            ASSERT_EQ(std::move(f)(0), 42);
            ASSERT_EQ(std::move(f)(1), 43);
            ASSERT_EQ(std::move(f)(2), 44);
        }
        {
            ebd::fn<int(int) & noexcept> f(std::cw<&LeftRightCallable::add_42_left>, LeftRightCallable{});
            ASSERT_EQ(f(0), 42);
            ASSERT_EQ(f(1), 43);
            ASSERT_EQ(f(2), 44);
        }
        {
            ebd::fn<int(int) && noexcept> f(std::cw<&LeftRightCallable::add_42_right>, LeftRightCallable{});
            ASSERT_EQ(std::move(f)(0), 42);
            ASSERT_EQ(std::move(f)(1), 43);
            ASSERT_EQ(std::move(f)(2), 44);
        }
        {
            ebd::fn<int(int) const & noexcept, sizeof(LeftRightCallable)> f(std::cw<&LeftRightCallable::add_42_left>, LeftRightCallable{});
            ASSERT_EQ(f(0), 42);
            ASSERT_EQ(f(1), 43);
            ASSERT_EQ(f(2), 44);

            auto f_auto = ebd::make_fn(std::cw<&LeftRightCallable::add_42_left>, LeftRightCallable{});
            ASSERT_EQ(f_auto(0), 42);
            ASSERT_EQ(f_auto(1), 43);
            ASSERT_EQ(f_auto(2), 44);

            static_assert(std::is_same_v<decltype(f_auto), decltype(f)>);
        }
        {
            ebd::fn<int(int) const && noexcept, sizeof(LeftRightCallable)> f(std::cw<&LeftRightCallable::add_42_right>, LeftRightCallable{});
            ASSERT_EQ(std::move(f)(0), 42);
            ASSERT_EQ(std::move(f)(1), 43);
            ASSERT_EQ(std::move(f)(2), 44);

            auto f_auto = ebd::make_fn(std::cw<&LeftRightCallable::add_42_right>, LeftRightCallable{});
            ASSERT_EQ(std::move(f_auto)(0), 42);
            ASSERT_EQ(std::move(f_auto)(1), 43);
            ASSERT_EQ(std::move(f_auto)(2), 44);

            static_assert(std::is_same_v<decltype(f_auto), decltype(f)>);
        }
    }
}

#endif // __cpp_lib_constant_wrapper >= 202603L
