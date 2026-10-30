#include "test_function.hpp"

// const, volatile, {& | &&}, noexcept
#define EMBED_DETAIL_FN_EXPAND_IMPL(F, NOEXCEPT) \
  F(     ,         ,   , NOEXCEPT)\
  F(const,         ,   , NOEXCEPT)\
  F(     , volatile,   , NOEXCEPT)\
  F(     ,         , & , NOEXCEPT)\
  F(     ,         , &&, NOEXCEPT)\
  F(const, volatile,   , NOEXCEPT)\
  F(const,         , & , NOEXCEPT)\
  F(const,         , &&, NOEXCEPT)\
  F(     , volatile, & , NOEXCEPT)\
  F(     , volatile, &&, NOEXCEPT)\
  F(const, volatile, & , NOEXCEPT)\
  F(const, volatile, &&, NOEXCEPT)

#if (EMBED_CXX_VERSION >= 201703L && \
    defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L)
// The noexcept-specification is a part of the function type and
// may appear as part of any function declarator. (Since C++17)
// See <https://cppreference.com/w/cpp/language/noexcept_spec>.

# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, ) EMBED_DETAIL_FN_EXPAND_IMPL(F, noexcept)
#else
# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, )
#endif

namespace {
    struct TrivialMoveDelete {
        TrivialMoveDelete()                         = default;
        TrivialMoveDelete(const TrivialMoveDelete&) = default;
        ~TrivialMoveDelete()                        = default;
        TrivialMoveDelete(TrivialMoveDelete&&)      = delete;
        int operator()() const volatile noexcept { return 42; }
    };
    struct NonTrivialMoveDelete {
        NonTrivialMoveDelete()                              = default;
        NonTrivialMoveDelete(const NonTrivialMoveDelete&)   = default;
        ~NonTrivialMoveDelete() {}                          // non-trivial
        NonTrivialMoveDelete(NonTrivialMoveDelete&&)        = delete;
        int operator()() const volatile noexcept { return 42; }
    };

    int move_count = 0;
    int copy_count = 0;

    struct HasMove {
        HasMove()               = default;
        HasMove(const HasMove&) noexcept { copy_count++; };
        HasMove(HasMove&&)      noexcept { move_count++; };
        ~HasMove()              = default;
        int operator()() const volatile noexcept { return 42; }
    };
}

TEST(Conformance___safe_fn, move_pass) {
    // TrivialMoveDelete
#define EBD_TEST_CASE(C, V, REF, NOEXCEPT)\
    {\
        TrivialMoveDelete obj;\
        ebd::__safe_fn<int() C V REF NOEXCEPT> f = obj;\
        auto f2 = std::move(f);\
        ASSERT_TRUE(f.is_empty());\
        using fwd_t = typename std::remove_reference<decltype(f2)>::type REF;\
        ASSERT_EQ(std::forward<fwd_t>(f2)(), 42);\
    }

    EMBED_DETAIL_FN_EXPAND(EBD_TEST_CASE)

#undef EBD_TEST_CASE

    // NonTrivialMoveDelete
#define EBD_TEST_CASE(C, V, REF, NOEXCEPT)\
    {\
        NonTrivialMoveDelete obj;\
        ebd::__safe_fn<int() C V REF NOEXCEPT> f = obj;\
        auto f2 = std::move(f);\
        ASSERT_TRUE(f.is_empty());\
        using fwd_t = typename std::remove_reference<decltype(f2)>::type REF;\
        ASSERT_EQ(std::forward<fwd_t>(f2)(), 42);\
    }

    EMBED_DETAIL_FN_EXPAND(EBD_TEST_CASE)

#undef EBD_TEST_CASE

    // HasMove
#define EBD_TEST_CASE(C, V, REF, NOEXCEPT)\
    {\
        ebd::__safe_fn<int() C V REF NOEXCEPT> f = HasMove{};\
        move_count = 0;\
        copy_count = 0;\
        auto f2 = std::move(f);\
        ASSERT_EQ(move_count, 1);\
        ASSERT_EQ(copy_count, 0);\
        ASSERT_TRUE(f.is_empty());\
        f = f2;\
        ASSERT_EQ(move_count, 1);\
        ASSERT_EQ(copy_count, 1);\
        using fwd_t = typename std::remove_reference<decltype(f2)>::type REF;\
        ASSERT_EQ(std::forward<fwd_t>(f2)(), 42);\
    }

    EMBED_DETAIL_FN_EXPAND(EBD_TEST_CASE)

#undef EBD_TEST_CASE
}
