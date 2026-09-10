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

}

// member function
static_assert(
    std::is_constructible_v<ebd::fn<int(int, int)>, std::constant_wrapper<&Normal::mem_fn_ii_add>, Normal>);
static_assert(
    !std::is_nothrow_constructible_v<ebd::fn<int(int, int)>,std::constant_wrapper<&Normal::mem_fn_ii_add>, Normal>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(int, int) const>, std::constant_wrapper<&Normal::mem_fn_ii_add>, Normal>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(int, int) noexcept>, std::constant_wrapper<&Normal::mem_fn_ii_add>, Normal>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(int, int) const noexcept>, std::constant_wrapper<&Normal::mem_fn_ii_add>, Normal>);

// non-const-invocable functor
static_assert(
    !std::is_constructible_v<ebd::fn<void()>, std::constant_wrapper<NonConstInvocable{}>, int*>);
static_assert(
    !std::is_constructible_v<ebd::fn<void() const>, std::constant_wrapper<NonConstInvocable{}>, int*>);
static_assert(
    !std::is_constructible_v<ebd::fn<void() noexcept>, std::constant_wrapper<NonConstInvocable{}>, int*>);

// in-place
static_assert(std::is_constructible_v<
    ebd::fn<int(), sizeof(ListInit)>,
    std::constant_wrapper<&ListInit::sum>, std::in_place_type_t<ListInit>, std::initializer_list<int>&>);
static_assert(
    !std::is_constructible_v<ebd::fn<int(), sizeof(ListInit)>, std::constant_wrapper<&ListInit::sum>, std::in_place_type_t<ListInit>>);
static_assert(!std::is_constructible_v<
    ebd::fn<int(), sizeof(ListInit)>,
    std::constant_wrapper<&ListInit::sum>, std::in_place_type_t<ListInit>, std::initializer_list<float>&>);

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
}

#endif // __cpp_lib_constant_wrapper >= 202603L
