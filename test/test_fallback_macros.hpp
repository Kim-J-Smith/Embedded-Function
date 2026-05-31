#ifndef TEST_FALLBACK_MACRO_HPP_
#define TEST_FALLBACK_MACRO_HPP_

#if defined(EBD_TEST_USE_FALLBACK)

# define EMBED_HAS_BUILTIN(x) 0
# define EMBED_HAS_ATTRIBUTE(x) 0
# define EMBED_HAS_CXX_ATTRIBUTE(x) 0
# define EMBED_ABI_VISIBILITY(x)
# define EMBED_INLINE inline
# define EMBED_RESTRICT
# define EMBED_NODISCARD
# define EMBED_FALLTHROUGH()
# define EMBED_DEPRECATED(x)

#endif // defined(EBD_TEST_USE_FALLBACK)

#if defined(__GNUC__) && __GNUC__ >= 16
# define EBD_TEST_TRY_BUG__GCC_106067
#endif

#if defined(__clang__) && __clang_major__ >= 22
# define EBD_TEST_TRY_BUG__Clang_SameNameStaticFunction
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1950
# define EBD_TEST_TRY_BUG__MSVC_1944_1
#endif

#endif // TEST_FALLBACK_MACRO_HPP_
