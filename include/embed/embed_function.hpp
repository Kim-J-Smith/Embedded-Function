/**
 * @file        embed_function.hpp
 * 
 * @date        2026-2-7
 * 
 * @version     2.1.6
 * 
 * @copyright   Copyright (c) 2026 Kim-J-Smith
 *              All rights reserved.
 *              <https://github.com/Kim-J-Smith/Embedded-Function>
 * 
 * @attention   This source is released under the MIT license
 *              SPDX-License-Identifier: MIT
 *              <http://opensource.org/licenses/MIT>
 */

// Just like function pointers, it is quick and efficient.

/// @b EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER
/// If this macro is defined, bigger default buffer size will be used.

/// @b EMBED_FN_CONFIG_DISABLE_SMART_FORWARD
/// If this macro is defined, `smart_forward_t` will fall back to Perfect Forwarding.

/// @b EMBED_FN_CONFIG_UNDEF_MACROS
/// If this macro is defined, EMBED_* macros will be undefined at the end of this file.

/// @b EMBED_FN_HOOK_DEBUG(message)
/// If this macro is defined, it will be called to print debug message in debug mode.

#ifndef EMBED_INCLUDED_EMBED_FUNCTION_HPP_
#define EMBED_INCLUDED_EMBED_FUNCTION_HPP_

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable: 4514 4668 4710 26495)
#endif

#ifndef EMBED_CXX_VERSION
# if defined(_MSVC_LANG) && ( _MSVC_LANG > __cplusplus )
#  define EMBED_CXX_VERSION _MSVC_LANG
# else
#  define EMBED_CXX_VERSION __cplusplus
# endif
#endif

#ifndef EMBED_HAS_BUILTIN
# if defined(__has_builtin) && defined(__is_identifier)
#  define EMBED_HAS_BUILTIN(x) (__has_builtin(x) || !__is_identifier(x))
# elif defined(__has_builtin)
#  define EMBED_HAS_BUILTIN(x) __has_builtin(x)
# else
#  define EMBED_HAS_BUILTIN(x) 0
# endif
#endif

#ifndef EMBED_HAS_ATTRIBUTE
# if defined(__has_attribute)
#  define EMBED_HAS_ATTRIBUTE(x) __has_attribute(x)
# else
#  define EMBED_HAS_ATTRIBUTE(x) 0
# endif
#endif

#ifndef EMBED_HAS_CXX_ATTRIBUTE
# if defined(__has_cpp_attribute)
#  define EMBED_HAS_CXX_ATTRIBUTE(x) __has_cpp_attribute(x)
# else
#  define EMBED_HAS_CXX_ATTRIBUTE(x) 0
# endif
#endif

#ifndef EMBED_CXX_ENABLE_EXCEPTION
# if defined(__cpp_exceptions)
#  define EMBED_CXX_ENABLE_EXCEPTION (__cpp_exceptions != 0)
# elif defined(_MSC_VER) && defined(_HAS_EXCEPTIONS)
#  define EMBED_CXX_ENABLE_EXCEPTION (_HAS_EXCEPTIONS != 0)
# elif (defined(__EXCEPTIONS) && __EXCEPTIONS == 1)
#  define EMBED_CXX_ENABLE_EXCEPTION 1
# else
#  define EMBED_CXX_ENABLE_EXCEPTION 0
# endif
#endif

#ifndef EMBED_ABI_VISIBILITY
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_ABI_VISIBILITY(x) __attribute__((visibility(#x)))
# else
#  define EMBED_ABI_VISIBILITY(x) 
# endif
#endif

#ifndef EMBED_CXX14_CONSTEXPR
# if (EMBED_CXX_VERSION >= 201402L && __cpp_constexpr >= 201304L)
#  define EMBED_CXX14_CONSTEXPR constexpr
# else
#  define EMBED_CXX14_CONSTEXPR
# endif
#endif

#ifndef EMBED_CXX20_CONSTEXPR
# if (EMBED_CXX_VERSION >= 202002L && __cpp_constexpr >= 202002L)
#  define EMBED_CXX20_CONSTEXPR constexpr
# else
#  define EMBED_CXX20_CONSTEXPR
# endif
#endif

#ifndef EMBED_INLINE
# if EMBED_HAS_ATTRIBUTE(always_inline)
#  define EMBED_INLINE inline __attribute__((always_inline))
# elif defined(_MSC_VER) || defined(__IAR_SYSTEMS_ICC__)
#  define EMBED_INLINE __forceinline
# else
#  define EMBED_INLINE inline
# endif
#endif

#ifndef EMBED_RESTRICT
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_RESTRICT __restrict__
# elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
#  define EMBED_RESTRICT __restrict
# else
#  define EMBED_RESTRICT
# endif
#endif

#ifndef EMBED_NODISCARD
# if (EMBED_CXX_VERSION >= 201703L && EMBED_HAS_CXX_ATTRIBUTE(nodiscard))
#  define EMBED_NODISCARD [[nodiscard]]
# elif EMBED_HAS_ATTRIBUTE(warn_unused_result)
#  define EMBED_NODISCARD __attribute__((warn_unused_result))
# else
#  define EMBED_NODISCARD
# endif
#endif

#ifndef EMBED_FALLTHROUGH
# if (EMBED_CXX_VERSION >= 201703L && EMBED_HAS_CXX_ATTRIBUTE(fallthrough))
#  define EMBED_FALLTHROUGH() [[fallthrough]]
# elif EMBED_HAS_CXX_ATTRIBUTE(gnu::fallthrough)
#  define EMBED_FALLTHROUGH() [[gnu::fallthrough]]
# elif EMBED_HAS_ATTRIBUTE(fallthrough)
#  define EMBED_FALLTHROUGH() __attribute__((fallthrough))
# else
#  define EMBED_FALLTHROUGH()
# endif
#endif

#ifndef EMBED_DEPRECATED
# if (EMBED_CXX_VERSION >= 201402L && EMBED_HAS_CXX_ATTRIBUTE(deprecated))
#  define EMBED_DEPRECATED(msg) [[deprecated(msg)]]
# elif EMBED_HAS_CXX_ATTRIBUTE(gnu::deprecated)
#  define EMBED_DEPRECATED(msg) [[gnu::deprecated(msg)]]
# elif EMBED_HAS_ATTRIBUTE(deprecated)
#  define EMBED_DEPRECATED(msg) __attribute__((deprecated(msg)))
# else
#  define EMBED_DEPRECATED(msg)
# endif
#endif

#if EMBED_CXX_VERSION >= 201103L
# include <cstddef>     // std::size_t
# include <cstring>     // std::memcpy, std::memset
# include <new>         // IWYU pragma: keep (placement new, std::launder(C++17))
# include <utility>     // std::move, std::forward, std::addressof, std::unreachable(C++23)
# include <functional>  // std::bad_function_call
# include <exception>   // std::terminate
# include <type_traits> // std::enable_if, ...
# include <tuple>       // std::tuple
# include <initializer_list>
#else
# error The 'embed_function.hpp' requires the support of syntax features of C++11.\
 You can use the '-std=c++11' compilation option, or simply switch to a newer compiler.
#endif

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

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )
// The noexcept-specification is a part of the function type and
// may appear as part of any function declarator. (Since C++17)
// See <https://en.cppreference.com/w/cpp/language/noexcept_spec>.

# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, ) EMBED_DETAIL_FN_EXPAND_IMPL(F, noexcept)
#else
# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, )
#endif

/// @brief Similar to `requires` in C++20.
/// Using SFINAE trait `enable_if_t` to require the template arguments.
#define EMBED_DETAIL_REQUIRES(...) \
  ::ebd::detail::enable_if_t<(__VA_ARGS__), int> = 0

#if defined(_MSC_VER)
# define EMBED_DETAIL_FORCE_EBO __declspec(empty_bases)
#else
# define EMBED_DETAIL_FORCE_EBO
#endif

#if defined(_MSC_VER)
# define EMBED_DETAIL_VIRTUAL_INHERITANCE __virtual_inheritance
#else
# define EMBED_DETAIL_VIRTUAL_INHERITANCE
#endif

// Generate the default/delete move constructors and move assignment for specified class.
#define EMBED_DETAIL_MOVE_FUNCTION(class_name, default_or_delete) \
  class_name(class_name&&)            = default_or_delete;\
  class_name& operator=(class_name&&) = default_or_delete;

// Generate the default/delete copy constructors and copy assignment for specified class.
#define EMBED_DETAIL_COPY_FUNCTION(class_name, default_or_delete) \
  class_name(const class_name&)             = default_or_delete;\
  class_name& operator=(const class_name&)  = default_or_delete;

// Generate default destructor and empty default constructor.
#define EMBED_DETAIL_DTOR_ECTOR_DEFAULT(class_name) \
  ~class_name() = default; \
  class_name()  = default;

// Generate all default functions (Ctor, Dtor, and assignment) for specified class.
#define EMBED_DETAIL_ALL_DEFAULT(class_name)      \
  EMBED_DETAIL_DTOR_ECTOR_DEFAULT(class_name)     \
  EMBED_DETAIL_COPY_FUNCTION(class_name, default) \
  EMBED_DETAIL_MOVE_FUNCTION(class_name, default)

/// @brief Unify the two SFINAE writing methods of "enable_if" and "requires",
/// eliminating the need to maintain two sets of code.
/// @attention @b EMBED_DETAIL_TEMPLATE_BEGIN and @b EMBED_DETAIL_REQUIRES_END
/// MUST be used simultaneously and cannot be used separately.
#if !defined(__cpp_concepts) || ( __cpp_concepts < 201907L )
# define EMBED_DETAIL_TEMPLATE_BEGIN(...) template <__VA_ARGS__,
# define EMBED_DETAIL_REQUIRES_END(...) EMBED_DETAIL_REQUIRES(__VA_ARGS__)>
#else
# define EMBED_DETAIL_TEMPLATE_BEGIN(...) template <__VA_ARGS__>
# define EMBED_DETAIL_REQUIRES_END(...) requires __VA_ARGS__
#endif

#define EMBED_DETAIL_TEXT(text) EMBED_DETAIL_TEXT_IMPL(text)
#define EMBED_DETAIL_TEXT_IMPL(text) #text

#if __cpp_lib_launder >= 201606L
# define EMBED_DETAIL_LAUNDER(x) ( ::std::launder(x) )
#elif EMBED_HAS_BUILTIN(__builtin_launder)
# define EMBED_DETAIL_LAUNDER(x) ( ::ebd::detail::launder(x) )
# define EMBED_DETAIL__NEED_LAUNDER
#else
# define EMBED_DETAIL_LAUNDER(x) ( x )
#endif

#if EMBED_HAS_ATTRIBUTE(may_alias)
# define EMBED_DETAIL_ALIAS __attribute__((may_alias))
#else
# define EMBED_DETAIL_ALIAS
#endif

#if defined(__OPTIMIZE__) || defined(NDEBUG) || !defined(EMBED_FN_HOOK_DEBUG)
# define EMBED_DETAIL_FAIL_MESSAGE(message)
# define EMBED_DETAIL_ASSERT_MESSAGE(expression, message)
#else
# define EMBED_DETAIL_FAIL_MESSAGE(message) do { EMBED_FN_HOOK_DEBUG(\
  __FILE__ ":" EMBED_DETAIL_TEXT(__LINE__) " " message); } while(0)
# define EMBED_DETAIL_ASSERT_MESSAGE(expression, message) \
  do { if (!(expression)) { \
    EMBED_FN_HOOK_DEBUG(__FILE__ ":" EMBED_DETAIL_TEXT(__LINE__) " " message); \
    std::terminate(); \
  } } while(0)
#endif

#if __cpp_lib_unreachable >= 202202L
# define EMBED_DETAIL_UNREACHABLE() std::unreachable()
#elif EMBED_HAS_BUILTIN(__builtin_unreachable)
# define EMBED_DETAIL_UNREACHABLE() __builtin_unreachable()
#elif defined(__GNUC__) && (__GNUC__ >= 5)
# define EMBED_DETAIL_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
# define EMBED_DETAIL_UNREACHABLE() __assume(false)
#else
# define EMBED_DETAIL_UNREACHABLE()
#endif

// Guidelines for reporting internal errors.
#define EMBED_DETAIL_REPORT_IE(error) \
  "An internal error has occurred: " error " This is unexpected. " \
  "Please report this bug at <https://github.com/Kim-J-Smith/Embedded-Function/issues>."

namespace ebd EMBED_ABI_VISIBILITY(default) {
namespace detail {

/// @brief Here are some standard traits that are not supported in C++11.
inline namespace cxx_traits {

  // See <https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1558>.
  template <typename... Args> struct make_void { using type = void; };

  /// @brief Types from <type_traits> have been implemented,
  /// consistent with the standard behavior (C++14 ~ C++23).
  /// See <https://en.cppreference.com/w/cpp/header/type_traits.html>.

  template <typename... Args> using void_t = typename make_void<Args...>::type;

  template <typename T>
  using remove_cvref_t = typename std::remove_cv<
    typename std::remove_reference<T>::type
  >::type;

  template <bool Cond, typename T = void>
  using enable_if_t = typename std::enable_if<Cond, T>::type;

  template <typename T>
  using remove_reference_t = typename std::remove_reference<T>::type;

  template <typename T>
  using remove_cv_t = typename std::remove_cv<T>::type;

  template <bool Cond, typename IfTrue, typename IfFalse>
  using conditional_t = typename std::conditional<Cond, IfTrue, IfFalse>::type;

  template <typename T>
  using decay_t = typename std::decay<T>::type;

  template <typename T>
  using remove_const_t = typename std::remove_const<T>::type;

  template <typename T>
  using remove_volatile_t = typename std::remove_volatile<T>::type;

  template <bool Val>
  using bool_constant = std::integral_constant<bool, Val>;

  // (undocumented) Tags that used in `invoke_result`, `invoke`, `invoke_r`, etc.
  class invoke_tag_normal {};
  class invoke_tag_memfn_ref_like {};
  class invoke_tag_memfn_pointer_like {};
  class invoke_tag_memobj_ref_like {};
  class invoke_tag_memobj_pointer_like {};

  // (undocumented) Unwrap the `std::reference_wrapper` recursively.
  template <typename T, typename U = remove_cvref_t<T>>
  struct inv_unwrap {
    using type = T;
    using unwrap_once = T;
  };

  template <typename T, typename UnderType>
  struct inv_unwrap<T, std::reference_wrapper<UnderType>> {
    using type = typename inv_unwrap<UnderType&>::type;
    using unwrap_once = UnderType&;
  };

  template <typename T>
  using inv_unwrap_t = typename inv_unwrap<T>::type;

  template <typename T>
  using unwrap_once_t = typename inv_unwrap<T>::unwrap_once;

  // (undocumented) Unwrap and forward std::reference_wrapper.
  template <typename T>
  EMBED_NODISCARD EMBED_INLINE constexpr enable_if_t<
    std::is_same<T, unwrap_once_t<T>>::value, T&&
  > unwrap_forward(remove_reference_t<T>&& obj) noexcept
  { return static_cast<T&&>(obj); }

  template <typename T>
  EMBED_NODISCARD EMBED_INLINE constexpr enable_if_t<
    std::is_same<T, unwrap_once_t<T>>::value, T&&
  > unwrap_forward(remove_reference_t<T>& obj) noexcept
  { return static_cast<T&&>(obj); }

  template <typename T, typename Under = unwrap_once_t<T>,
    EMBED_DETAIL_REQUIRES(!std::is_same<T, Under>::value)
  > EMBED_NODISCARD EMBED_INLINE constexpr inv_unwrap_t<T>&&
  unwrap_forward(remove_reference_t<T>&& obj) noexcept {
    return unwrap_forward<Under>(obj.get());
  }

  template <typename T, typename Under = unwrap_once_t<T>,
    EMBED_DETAIL_REQUIRES(!std::is_same<T, Under>::value)
  > EMBED_NODISCARD EMBED_INLINE constexpr inv_unwrap_t<T>&&
  unwrap_forward(remove_reference_t<T>& obj) noexcept {
    return unwrap_forward<Under>(obj.get());
  }

  // (undocumented) Provide success type for invoke_result.
  template <typename T, typename Tag>
  struct success_type {
    using type  = T;
    using tag   = Tag;
  };

  // (undocumented) Trigger the SFINAE.
  struct failure_type {};

  // (undocumented) Get the invoke result of pointer to member
  // object with the given reference-like argument.
  template <typename MemObj, typename Arg>
  struct invoke_result_of_memobj_ref_like_helper {
    template<typename> static failure_type test(...) { return {}; }
    template<typename T> static success_type<
      /* type = */ decltype(std::declval<T>().*std::declval<MemObj>()),
      /* tag = */ invoke_tag_memobj_ref_like
    > test(int) { return {}; }

    using type = decltype(test<Arg>(0));
  };

  // (undocumented) Get the invoke result of pointer to member
  // object with the given pointer-like argument.
  template <typename MemObj, typename Arg>
  struct invoke_result_of_memobj_pointer_like_helper {
    template<typename> static failure_type test(...) { return {}; }
    template<typename T> static success_type<
      /* type = */ decltype((*std::declval<T>()).*std::declval<MemObj>()),
      /* tag = */ invoke_tag_memobj_pointer_like
    > test(int) { return {}; }

    using type = decltype(test<Arg>(0));
  };

  // (undocumented) Get the invoke result of pointer to member
  // object with the given argument (reference-like or pointer-like).
  template <typename T, typename U>
  struct invoke_result_of_memobj;  // Undefined

  template <typename Class, typename RetT, typename Arg>
  struct invoke_result_of_memobj<RetT Class::*, Arg> {
    using MemberObj = RetT Class::*;
    using ThisClass = remove_cvref_t<Arg>;

    using type = typename conditional_t<
      (std::is_same<Class, ThisClass>::value || std::is_base_of<Class, ThisClass>::value),
      invoke_result_of_memobj_ref_like_helper<MemberObj, Arg>,
      invoke_result_of_memobj_pointer_like_helper<MemberObj, Arg>
    >::type;
  };

  // (undocumented) Get the invoke result of pointer to member
  // function with the given arguments. And the first argument is
  // reference-like object.
  template <typename MemFunc, typename Arg, typename... ArgsType>
  struct invoke_result_of_memfunc_ref_like_helper {
    template<typename> static failure_type test(...) { return {}; }
    template<typename T> static success_type<
      /* type = */ decltype((std::declval<T>().*std::declval<MemFunc>())(
        std::declval<ArgsType>()...
      )),
      /* tag = */ invoke_tag_memfn_ref_like
    > test(int) { return {}; }

    using type = decltype(test<Arg>(0));
  };

  // (undocumented) Get the invoke result of pointer to member
  // function with the given arguments. And the first argument is
  // pointer-like object.
  template <typename MemFunc, typename Arg, typename... ArgsType>
  struct invoke_result_of_memfunc_pointer_like_helper {
    template<typename> static failure_type test(...) { return {}; }
    template<typename T> static success_type<
      /* type = */ decltype(((*std::declval<T>()).*std::declval<MemFunc>())(
        std::declval<ArgsType>()...
      )),
      /* tag = */ invoke_tag_memfn_pointer_like
    > test(int) { return {}; }

    using type = decltype(test<Arg>(0));
  };

  // (undocumented) Get the invoke result of pointer to member
  // function with the given arguments.
  template <typename... T>
  struct invoke_result_of_memfunc;  // Undefined

  template <typename Class, typename RetT, typename Arg, typename... ArgsType>
  struct invoke_result_of_memfunc<RetT Class::*, Arg, ArgsType...> {
    using MemberFunc = RetT Class::*;
    using ThisClass = remove_cvref_t<Arg>;

    using type = typename conditional_t<
      (std::is_same<Class, ThisClass>::value || std::is_base_of<Class, ThisClass>::value),
      invoke_result_of_memfunc_ref_like_helper<MemberFunc, Arg, ArgsType...>,
      invoke_result_of_memfunc_pointer_like_helper<MemberFunc, Arg, ArgsType...>
    >::type;
  };

  // (undocumented) Get invoke result of normal function with the given arguments.
  template <typename Functor, typename... ArgsType>
  struct invoke_result_of_normal {
    template<typename> static failure_type test(...) { return {}; }
    template<typename T> static success_type<
      /* type = */ decltype(std::declval<T>()(
        std::declval<ArgsType>()...)),
      /* tag = */ invoke_tag_normal
    > test(int) { return {}; }

    using type = decltype(test<Functor>(0));
  };

  // (undocumented) Implement the `invoke_result`.
  template <bool, bool, typename Func, typename... Args>
  struct invoke_result_impl {
    using type = failure_type;
  };

  template <typename PointerToMemObj, typename Arg>
  struct invoke_result_impl<
    /* is_memfunc_ptr = */ false,
    /* is_memobj_ptr = */ true,
    PointerToMemObj, Arg
  > {
    using type = typename invoke_result_of_memobj<
      typename std::decay<PointerToMemObj>::type,
      inv_unwrap_t<Arg>
    >::type;
  };

  template <typename PointerToMemFunc, typename Arg, typename... ArgsType>
  struct invoke_result_impl<
    /* is_memfunc_ptr = */ true,
    /* is_memobj_ptr = */ false,
    PointerToMemFunc, Arg, ArgsType...
  > {
    using type = typename invoke_result_of_memfunc<
      typename std::decay<PointerToMemFunc>::type,
      inv_unwrap_t<Arg>, ArgsType...
    >::type;
  };

  template <typename NormalFunc, typename... ArgsType>
  struct invoke_result_impl<
    /* is_memfunc_ptr = */ false,
    /* is_memobj_ptr = */ false,
    NormalFunc, ArgsType...
  > {
    using type = typename invoke_result_of_normal<
      NormalFunc, ArgsType...
    >::type;
  };

  // Get the invoke result and invoke tag.
  // See <https://en.cppreference.com/w/cpp/types/result_of.html>.
  template <typename Func, typename... ArgsT>
  struct invoke_result : public invoke_result_impl<
    std::is_member_function_pointer<
      remove_reference_t<Func>
    >::value,
    std::is_member_object_pointer<
      remove_reference_t<Func>
    >::value,
    Func, ArgsT...
  >::type {};

  // (undocumented) Check whether the INVOKE expression itself can throw.
  template <typename Tag, typename...>
  struct call_is_nothrow_impl {
    static constexpr bool value = false;
  };

  template <typename NormalFunc, typename... Args>
  struct call_is_nothrow_impl<invoke_tag_normal, NormalFunc, Args...> {
    static constexpr bool value = noexcept(
      std::declval<NormalFunc>()(std::declval<Args>()...));
  };

  template <typename MemObj, typename Arg>
  struct call_is_nothrow_impl<invoke_tag_memobj_ref_like, MemObj, Arg> {
    using U = inv_unwrap_t<Arg>;
    static constexpr bool value = noexcept(
      std::declval<U>().*std::declval<MemObj>());
  };

  template <typename MemObj, typename Arg>
  struct call_is_nothrow_impl<invoke_tag_memobj_pointer_like, MemObj, Arg> {
    static constexpr bool value = noexcept(
      (*std::declval<Arg>()).*std::declval<MemObj>());
  };

  template <typename Memfunc, typename Arg, typename... Args>
  struct call_is_nothrow_impl<
    invoke_tag_memfn_ref_like, Memfunc, Arg, Args...> {
    using U = inv_unwrap_t<Arg>;
    static constexpr bool value = noexcept(
      (std::declval<U>().*std::declval<Memfunc>()) (std::declval<Args>()...));
  };

  template <typename Memfunc, typename Arg, typename... Args>
  struct call_is_nothrow_impl<
    invoke_tag_memfn_pointer_like, Memfunc, Arg, Args...> {
    static constexpr bool value = noexcept(
      ((*std::declval<Arg>()).*std::declval<Memfunc>()) (std::declval<Args>()...));
  };

  template <typename Func, typename ArgsTuple, typename = void>
  struct call_is_nothrow_helper : std::false_type {};

  template <typename Func, typename... Args>
  struct call_is_nothrow_helper<Func, std::tuple<Args...>, 
    void_t<typename invoke_result<Func, Args...>::tag>>
  : call_is_nothrow_impl<typename invoke_result<Func, Args...>::tag, Func, Args...>
  {};

  template <typename Func, typename... Args>
  using call_is_nothrow = call_is_nothrow_helper<Func, std::tuple<Args...>>;

  // See <https://en.cppreference.com/w/cpp/types/reference_converts_from_temporary.html>.
  template <typename To, typename From>
  struct reference_converts_from_temporary
  : public bool_constant<
#if __cpp_lib_reference_from_temporary >= 202202L
    std::reference_converts_from_temporary_v<To, From>
#elif EMBED_HAS_BUILTIN(__reference_converts_from_temporary)
    __reference_converts_from_temporary(To, From)
#else
    false // After research, there is no better fall-back scheme.
#endif
  > {};

  // (undocumented) Implement the is_invocable, is_nothrow_invocable, etc.
  template <typename Res, typename Ret, 
    bool RetIsVoid = std::is_void<Ret>::value, typename Enable = void>
  struct is_invocable_impl : public std::false_type
  { using nothrow = std::false_type; };

  template <typename Res, typename Ret>
  struct is_invocable_impl<Res, Ret, 
    /* is_void<Ret>::value = */ true, 
    /* Enable = */ void_t<typename Res::type>>
  : public std::true_type
  { using nothrow = std::true_type; };

#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
# pragma GCC diagnostic ignored "-Wreturn-type"
#endif

  template <typename Res, typename Ret>
  struct is_invocable_impl<Res, Ret, 
    /* is_void<Ret>::value = */ false, 
    /* Enable = */ void_t<typename Res::type>
  > {
    using invoke_t = typename Res::type;

    static invoke_t testGet() noexcept { return std::declval<invoke_t>(); }
    template <typename T>
    static void testConv(T) noexcept {}

    template <typename, bool = true>
    static std::false_type test(...) noexcept { return {}; }

    template <typename Rt, 
      bool NoThrow = noexcept(testConv<Rt>(testGet())),
      typename Enable = decltype(testConv<Rt>(testGet()))
    >
    static bool_constant<NoThrow>
    test(int) noexcept { return {}; }

    using type = decltype(test<Ret, true>(1));
    using nothrow = decltype(test<Ret>(1));
  };

#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

  // See <https://en.cppreference.com/w/cpp/types/is_invocable.html>.
  template <typename Ret, typename Func, typename... Args>
  struct is_invocable_r : public bool_constant<
    is_invocable_impl<invoke_result<Func, Args...>, Ret>::type::value
  > {};

  template <typename Ret, typename Func, typename... Args>
  struct is_nothrow_invocable_r : public bool_constant<
    call_is_nothrow<Func, Args...>::value
    && is_invocable_impl<invoke_result<Func, Args...>, Ret>::nothrow::value
  > {};

  /// @fn invoke_impl
  // (undocumented) Distribute the call of callable objects, including normal 
  // functions, pointer to member functions, and pointer to member objects 
  // (distinguish reference-like/pointer-like class object callers).

  // Invokes the callable object directly with the given arguments.
  // Used for free function, static member function, and functors (classes that overload operator()).
  template <typename RetT, typename Func, typename... Args>
  inline EMBED_CXX14_CONSTEXPR RetT
  invoke_impl(invoke_tag_normal, Func&& fn, Args&&... args)
    noexcept(is_nothrow_invocable_r<RetT, Func, Args...>::value)
  { return std::forward<Func>(fn)(std::forward<Args>(args)...); }

  // Invokes the pointer to member object by the given "reference" of class object.
  // Note: The `std::reference_wrapper` is also regarded as "reference".
  template <typename RetT, typename MemObj, typename Arg>
  inline EMBED_CXX14_CONSTEXPR RetT
  invoke_impl(invoke_tag_memobj_ref_like, MemObj&& obj, Arg&& arg)
    noexcept(is_nothrow_invocable_r<RetT, MemObj, Arg>::value)
  { return unwrap_forward<Arg>(arg).*std::forward<MemObj>(obj); }

  // Invokes the pointer to member object by the given "pointer" of class object.
  // Note: The `std::unique_ptr`, `std::shared_ptr` are also regarded as "pointer".
  template <typename RetT, typename MemObj, typename Arg>
  inline EMBED_CXX14_CONSTEXPR RetT
  invoke_impl(invoke_tag_memobj_pointer_like, MemObj&& obj, Arg&& arg)
    noexcept(is_nothrow_invocable_r<RetT, MemObj, Arg>::value)
  { return (*std::forward<Arg>(arg)).*std::forward<MemObj>(obj); }

  // Invokes the pointer to member function by the given "reference" of class object.
  // Note: The `std::reference_wrapper` is also regarded as "reference".
  template <typename RetT, typename MemFunc, typename Arg, typename... ArgsType>
  inline EMBED_CXX14_CONSTEXPR RetT
  invoke_impl(invoke_tag_memfn_ref_like, MemFunc&& memfn, Arg&& arg, ArgsType&&... args)
  noexcept(is_nothrow_invocable_r<RetT, MemFunc, Arg, ArgsType...>::value) {
    return (unwrap_forward<Arg>(arg).*std::forward<MemFunc>(memfn))(
      std::forward<ArgsType>(args)...
    );
  }

  // Invokes the pointer to member function by the given "pointer" of class object.
  // Note: The `std::unique_ptr`, `std::shared_ptr` are also regarded as "pointer".
  template <typename RetT, typename MemFunc, typename Arg, typename... ArgsType>
  inline EMBED_CXX14_CONSTEXPR RetT
  invoke_impl(invoke_tag_memfn_pointer_like, MemFunc&& memfn, Arg&& arg, ArgsType&&... args)
  noexcept(is_nothrow_invocable_r<RetT, MemFunc, Arg, ArgsType...>::value) {
    return ((*std::forward<Arg>(arg)).*std::forward<MemFunc>(memfn))(
      std::forward<ArgsType>(args)...
    );
  }

  // See <https://en.cppreference.com/w/cpp/utility/functional/invoke.html>.
  template <typename Result, typename Callee, typename... Args>
  inline EMBED_CXX14_CONSTEXPR enable_if_t<
    is_invocable_r<Result, Callee, Args...>::value 
    && std::is_void<Result>::value>
  invoke_r(Callee&& fn, Args&&... args)
  noexcept(is_nothrow_invocable_r<Result, Callee, Args...>::value) {
    using invoke_t  = typename invoke_result<Callee, Args...>::type;
    using tag_t     = typename invoke_result<Callee, Args...>::tag;

    // The `Result` is void, so there is no return.
    invoke_impl<invoke_t>(tag_t{}, std::forward<Callee>(fn),
      std::forward<Args>(args)...);
  }
  
  template <typename Result, typename Callee, typename... Args>
  inline EMBED_CXX14_CONSTEXPR enable_if_t<
    is_invocable_r<Result, Callee, Args...>::value 
    && !std::is_void<Result>::value, Result>
  invoke_r(Callee&& fn, Args&&... args)
  noexcept(is_nothrow_invocable_r<Result, Callee, Args...>::value) {
    using invoke_t  = typename invoke_result<Callee, Args...>::type;
    using tag_t     = typename invoke_result<Callee, Args...>::tag;

    // Assert no dangling.
    static_assert(!reference_converts_from_temporary<Result, invoke_t>::value,
      "Returning from invoke_r would bind a temporary object to the reference return type,"
      " which would result in a dangling reference.");

    return invoke_impl<invoke_t>(tag_t{}, std::forward<Callee>(fn),
      std::forward<Args>(args)...);
  }

#ifdef EMBED_DETAIL__NEED_LAUNDER

  // [ptr.launder]
  template <typename T> EMBED_NODISCARD EMBED_INLINE constexpr
  T* launder(T* ptr) noexcept { return __builtin_launder(ptr); }

#undef EMBED_DETAIL__NEED_LAUNDER
#endif

} // end namespace cxx_traits

  // Forward declaration.
  template <std::size_t BufferSize, typename Config, typename Signature>
  class EMBED_DETAIL_FORCE_EBO function;

/// @brief Here are some self-defined traits.
inline namespace fn_traits {

  // The value is always false.
  template <typename... Args>
  struct always_false { static constexpr bool value = false; };

  // Is trivial for the purposes of calls. (trivially destruct, copy and move)
  // See <https://itanium-cxx-abi.github.io/cxx-abi/abi.html#non-trivial-parameters>.
  template <typename T>
  struct is_call_trivial : public bool_constant<
    std::is_trivially_destructible<T>::value
      && std::is_trivially_copy_constructible<T>::value
      && std::is_trivially_move_constructible<T>::value
  > {};

  // std::is_trivial is deprecated in C++26. But we need it.
  template <typename T>
  struct is_traditional_trivial : public bool_constant<
    std::is_trivially_default_constructible<T>::value
    && std::is_trivially_copyable<T>::value
  > {};

  // Check self.
  template <typename A, typename B>
  using is_self = std::is_same<remove_cvref_t<A>, remove_cvref_t<B>>;

  // Configuration parameter package.
  template <
    bool IsCopyable, 
    bool IsView,
    bool IsThrowing,
    bool AssertObjectNoThrow
  >
  struct config_package {
    // Whether the function wrapper is copyable.
    static constexpr bool isCopyable = IsCopyable;
    // Whether the function wrapper is actually a view.
    static constexpr bool isView = IsView;
    // Whether the function wrapper is throwing `std::bad_function_call`
    // when it is called in an empty state.
    static constexpr bool isThrowing = IsThrowing;
    // Whether the function wrapper asserts that the callable object is not 
    // throwing exceptions when it is created, copied, moved, and called.
    static constexpr bool assertNoThrow = AssertObjectNoThrow;
  };

  // Check whether the type is config_package or not.
  template <typename T>
  struct is_config_package : public std::false_type {};

  // MSVC 19.33 and earlier have a bug where a bool parameter pack is not
  // handled correctly. To work around this issue, we avoid using bool...
  // in the template parameter list.
  template <bool IsCopyable, bool IsView, bool IsThrowing, bool AssertObjectNoThrow>
  struct is_config_package<
    config_package<IsCopyable, IsView, IsThrowing, AssertObjectNoThrow>>
  : public std::true_type {};

  // Uses `std::tuple` as the package of arguments.
  template <typename... Args>
  using args_package = std::tuple<Args...>;

  // Unwrap the function signature.
  template <typename T>
  struct unwrap_signature {
    static constexpr bool isSignature = false;
    using ret   = void;
    using args  = args_package<>;
    using pure_sig = void();
    static constexpr bool hasConst = false;
    static constexpr bool hasVolatile = false;
    static constexpr bool hasRRef = false;
    static constexpr bool hasLRef = false;
    static constexpr bool isNoexcept = false;

    template <typename U>
    using add_cv_like = U;
    template <typename U>
    using add_cvref_like = U;
  };

#define EMBED_DETAIL_UNWRAP_SIGNATURE_DEFINE(C, V, REF, NOEXCEPT)             \
  template <typename Ret, typename... Args>                                   \
  struct unwrap_signature<Ret(Args...) C V REF NOEXCEPT> {                    \
  private:                                                                    \
    using is_ = std::false_type;                                              \
    using is_noexcept = std::true_type;                                       \
  public:                                                                     \
    static constexpr bool isSignature = true;                                 \
    using ret   = Ret;                                                        \
    using args  = args_package<Args...>;                                      \
    using pure_sig = Ret(Args...);                                            \
    static constexpr bool hasConst = std::is_const<int C>::value;             \
    static constexpr bool hasVolatile = std::is_volatile<int V>::value;       \
    static constexpr bool hasRRef = std::is_rvalue_reference<int REF>::value; \
    static constexpr bool hasLRef = std::is_lvalue_reference<int REF>::value; \
    static constexpr bool isNoexcept = is_ ## NOEXCEPT::value;                \
                                                                              \
    template <typename T>                                                     \
    using add_cv_like = T C V;                                                \
    template <typename T>                                                     \
    using add_cvref_like = T C V REF;                                         \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_UNWRAP_SIGNATURE_DEFINE)

#undef EMBED_DETAIL_UNWRAP_SIGNATURE_DEFINE

  // Implement the "is_ebd_fn" trait.
  template <typename T>
  struct is_ebd_fn_impl : public std::false_type
  { using signature = void; };

  template <std::size_t Buf, typename Cfg, typename Sig>
  struct is_ebd_fn_impl<function<Buf, Cfg, Sig>>
  : public bool_constant<
    unwrap_signature<Sig>::isSignature
    && is_config_package<Cfg>::value
  > { using signature = Sig; };

  // Check whether the type is `ebd::detail::function` or not.
  template <typename T>
  using is_ebd_fn = is_ebd_fn_impl<remove_cvref_t<T>>;

  // Throw std::bad_function_call or just call std::terminate().
  template<bool IsThrowing>
  [[noreturn]] inline enable_if_t<!IsThrowing>
  throw_or_terminate() noexcept {
    EMBED_DETAIL_FAIL_MESSAGE("[Embedded Function]: Empty function has been called!");
    std::terminate();
  }

  template<bool IsThrowing>
  [[noreturn]] inline enable_if_t<IsThrowing>
  throw_or_terminate() noexcept(!EMBED_CXX_ENABLE_EXCEPTION) {
    EMBED_DETAIL_FAIL_MESSAGE("[Embedded Function]: Empty function has been called!");
#if EMBED_CXX_ENABLE_EXCEPTION != 0
    throw std::bad_function_call{};
#else
    std::terminate();
#endif
  }

  // Check whether the callable object is function pointer or not.
  template <typename T>
  struct is_function_ptr : public std::false_type {};

  template <typename Ret, typename... Args>
  struct is_function_ptr<Ret(*)(Args...)>
  : public std::true_type {};

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )
  template <typename Ret, typename... Args>
  struct is_function_ptr<Ret(*)(Args...) noexcept>
  : public std::true_type {};
#endif

  // Check to store origin type or not (store the pointer).
  template <typename T, bool IsView, 
    typename DecT = decay_t<T>,
    bool IsStoredOrigin = IsView ? is_function_ptr<DecT>::value : true
  >
  struct is_stored_origin
  : public bool_constant<IsStoredOrigin> {
    static constexpr bool isTrivial = is_traditional_trivial<DecT>::value;
    static_assert(!(IsView && IsStoredOrigin && !isTrivial),
      EMBED_DETAIL_REPORT_IE(
        "Stored origin type in view mode must be trivially"
        " copyable/destructible. Here Functor is stored originally,"
        " but it is NOT trivial."));
  };

  // Get the really stored type.
  template <typename T, bool IsView>
  struct get_stored_type {
    using type = conditional_t<is_stored_origin<T, IsView>::value,
      decay_t<T>, typename std::add_pointer<decay_t<T>>::type>;
  };

  template <typename T, bool IsView = true>
  using get_stored_type_t = typename get_stored_type<T, IsView>::type;

  // Implement the `fn_can_convert`.
  template <typename To, typename From>
  struct fn_can_convert_impl : public std::false_type {};

  template <std::size_t BufTo, typename CfgTo, typename SigTo,
    std::size_t BufFrom, typename CfgFrom, typename SigFrom>
  struct fn_can_convert_impl<
    function<BufTo, CfgTo, SigTo>, function<BufFrom, CfgFrom, SigFrom>
  > {
    // Get the unwrap trait.
    using unwrap_to = unwrap_signature<SigTo>;
    using unwrap_from = unwrap_signature<SigFrom>;

    // Get the return type and arguments package.
    using sig_to_ret = typename unwrap_to::ret;
    using sig_from_ret = typename unwrap_from::ret;
    using sig_to_args = typename unwrap_to::args;
    using sig_from_args = typename unwrap_from::args;

    // Check the arguments of `From` and `To` are same.
    static constexpr bool sig_ret_ok = std::is_same<sig_to_ret, sig_from_ret>::value;
    static constexpr bool sig_args_ok = std::is_same<sig_to_args, sig_from_args>::value;

    // Check the buffer size of `To` is bigger `From` or equals.
    static constexpr bool buf_ok = BufTo >= BufFrom;

    // Check the Configuration.
    static constexpr bool cfg_ok = 
      CfgTo::isCopyable <= CfgFrom::isCopyable // Copyable to Move-only is OK.
      && CfgTo::isView == CfgFrom::isView
      && CfgTo::isThrowing == CfgFrom::isThrowing
      && CfgTo::assertNoThrow <= CfgFrom::assertNoThrow; // Assert to non-assert is OK.

    // In view mode, the requires is special.
    // See <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3961r1.html>.
    static constexpr bool noexcept_qualifier_ok = 
      (unwrap_to::isNoexcept == unwrap_from::isNoexcept)
      || (
        CfgTo::isView == true 
        && CfgFrom::isView == true 
        && unwrap_to::isNoexcept < unwrap_from::isNoexcept
      );

    /// TODO: Finalize the details of the conversion of the qualifiers
    // Check the qualifiers.
    static constexpr bool qualifier_ok = 
      (unwrap_to::hasConst <= unwrap_from::hasConst)
      && (unwrap_to::hasVolatile == unwrap_from::hasVolatile)
      && (unwrap_to::hasRRef == unwrap_from::hasRRef)
      && (unwrap_to::hasLRef == unwrap_from::hasLRef)
      && noexcept_qualifier_ok;

    static constexpr bool value = 
      buf_ok && cfg_ok && sig_ret_ok && sig_args_ok && qualifier_ok;
  };

  // Check ebd::detail::function are similar or not.
  template <typename To, typename From>
  struct fn_can_convert : public bool_constant<
    fn_can_convert_impl<
      remove_reference_t<To>, remove_reference_t<From>
    >::value
  >::type {};

  // Check whether Functor can be constructed as decay_t<Functor>
  // without throwing an exception. And `std::is_nothrow_constructible`
  // has bug. (It will also check the destructor)
  // See <https://cplusplus.github.io/LWG/issue2116>.
  template <typename Functor, typename Class = decay_t<Functor>,
    typename = void>
  struct is_nothrow_construct_from_functor
  : public bool_constant<false> {};

  template <typename Functor, typename Class>
  struct is_nothrow_construct_from_functor<
    Functor, Class, void_t<decltype( Class(std::declval<Functor>()) )>>
  : public bool_constant<
    noexcept(::new (static_cast<void*>(0)) Class(std::declval<Functor>()))
  > {};

  // Get invoke result with arguments package.
  template <typename Fn, typename ArgsPackage>
  struct invoke_result_package {
    static_assert(always_false<Fn>::value,
      "The input is not arguments package!");
  };

  template <typename Fn, typename... Args>
  struct invoke_result_package<Fn, args_package<Args...>>
  : public invoke_result<Fn, Args...> {};

  // [func.wrap.move.ctor]/1
  template <typename Signature, typename Fn, typename Ret, typename ArgsPackage>
  struct is_callable_from_pkg;

  template <typename Signature, typename Fn, typename Ret, typename... Args>
  struct is_callable_from_pkg<Signature, Fn, Ret, args_package<Args...>> {
    using unwrap_sig  = unwrap_signature<Signature>;
    using f_cv        = typename unwrap_sig::template add_cv_like<Fn>;
    using f_cvref     = typename unwrap_sig::template add_cvref_like<Fn>;
    using f_inv_quals = conditional_t<unwrap_sig::hasRRef, f_cv&&, f_cv&>;

    static constexpr bool value = unwrap_sig::isNoexcept
      ? is_nothrow_invocable_r<Ret, f_cvref, Args...>::value
        && is_nothrow_invocable_r<Ret, f_inv_quals, Args...>::value
      : is_invocable_r<Ret, f_cvref, Args...>::value
        && is_invocable_r<Ret, f_inv_quals, Args...>::value;
  };

  // Check the functor is callable with given arguments.
  // [func.wrap.move.ctor]/1
  template <typename Functor, typename Signature>
  struct is_callable_from {
    using unwrap_sig = unwrap_signature<Signature>;
    using ret       = typename unwrap_sig::ret;
    using args_pack = typename unwrap_sig::args;
    using dec_func  = decay_t<Functor>;

    static constexpr bool value = 
      is_callable_from_pkg<Signature, dec_func, ret, args_pack>::value;
  };

  // Check the align and size of functor.
  template <typename Functor, typename Config, std::size_t BufSize, typename Erasure,
    typename DecFunctor = decay_t<Functor>>
  struct align_size_is_ok {
    static constexpr bool is_ok = sizeof(DecFunctor) <= sizeof(Erasure)
      && alignof(DecFunctor) <= alignof(Erasure)
      && (sizeof(DecFunctor) % alignof(DecFunctor) == 0);

    static constexpr bool value = 
      !is_stored_origin<DecFunctor, Config::isView>::value || is_ok;
  };

  // Get aligned size. Rounds up to the nearest word.
  template <std::size_t MinAlign = sizeof(void (*) ())>
  constexpr std::size_t get_aligned_size(std::size_t size)
  { return size == 0 ? MinAlign : ((size - 1) / MinAlign + 1) * MinAlign; }

  /// @brief Undefined class.
  /// @e EMBED_DETAIL_VIRTUAL_INHERITANCE - This macro is used to inform the MSVC
  /// compiler that this is a declaration of a virtual inheritance class, in order
  /// to obtain the theoretically maximum size of "pointers to member functions".
  class EMBED_DETAIL_VIRTUAL_INHERITANCE UndefinedClass;

  // The default buffer size. Usually is 2 * sizeof(void*).
  struct default_buffer_size {
    // The buffer size for ebd::fn_ref. Stop supporting pointer-to-members
    static constexpr std::size_t ref_buf = sizeof(void (*) ());
    static constexpr std::size_t view_buf = ref_buf;
#if defined(EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER)
    // The CommandTable size plus the buffer size is about 8 * sizeof(void*).
    // TODO: The size of this buffer zone needs further examination.
    static constexpr std::size_t value_c1 = 6 * sizeof(void*);
    static constexpr std::size_t value_c2 = sizeof(::std::function<void()>);
    static constexpr std::size_t value = value_c1 > value_c2 ? value_c1 : value_c2;
#else
    static constexpr std::size_t value = sizeof(void (UndefinedClass::*) ());
#endif

    static constexpr std::size_t align_value = alignof(void (UndefinedClass::*) ());
  };

  // Check whether throwing operations are acceptable.
  template <typename Functor, typename Object, typename Config,
    typename DecFunctor = decay_t<Functor>>
  struct assert_throwing_is_ok {
    // The `is_ok` means the `DecFunctor` is nothrow-destructible and 
    // nothrow-constructible from `Object`. If it is copy-constructible, 
    // it should be nothrow-copy-constructible. And if it is move
    // -constructible, it should be nothrow-move-constructible.
    static constexpr bool is_ok = std::is_nothrow_destructible<DecFunctor>::value
      && (std::is_nothrow_copy_constructible<DecFunctor>::value || 
        !std::is_copy_constructible<DecFunctor>::value)
      && (std::is_nothrow_move_constructible<DecFunctor>::value || 
        !std::is_move_constructible<DecFunctor>::value)
      && std::is_nothrow_constructible<DecFunctor, Object>::value;

    // If `Config::isView` is true, then all restrictions are ignored.
    // Otherwise, if the `Config::assertNoThrow` is true as well
    // as the `is_ok` is false, then the `value` will be false to
    // trigger the static_assert.
    static constexpr bool value = 
      Config::isView || !(Config::assertNoThrow && !is_ok);
  };

  // Utility struct to check if a callable object is not empty.
  struct check_not_empty {
    template <typename T>
    static constexpr bool not_empty(T* f) noexcept { return f != nullptr; }
    template <typename Class, typename T>
    static constexpr bool not_empty(T Class::* f) noexcept { return f != nullptr; }
    template <typename T>
    static constexpr bool not_empty(const T&) noexcept { return true; }

    template <typename Sig>
    static bool not_empty(const ::std::function<Sig>& f) noexcept
    { return static_cast<bool>(f); }

    template <std::size_t Buf, typename Cfg, typename Sig,
      EMBED_DETAIL_REQUIRES(!Cfg::isView) /*OWNING*/> static
    EMBED_CXX14_CONSTEXPR bool not_empty(const function<Buf, Cfg, Sig>& f) noexcept
    { return static_cast<bool>(f); }

#if __cpp_lib_move_only_function >= 202110L

    template <typename Sig>
    static bool not_empty(const ::std::move_only_function<Sig>& f) noexcept
    { return static_cast<bool>(f); }

#endif // ^^^ __cpp_lib_move_only_function >= 202110L

#if __cpp_lib_copyable_function >= 202306L

    template <typename Sig>
    static bool not_empty(const ::std::copyable_function<Sig>& f) noexcept
    { return static_cast<bool>(f); }

#endif // ^^^ __cpp_lib_copyable_function >= 202306L

  };

  // Trait to check if a functor's copy/move capabilities match the configuration.
  template <typename Functor, typename Config, 
    typename DecFunctor = decay_t<Functor>>
  struct copyable_is_ok {
    static constexpr bool copy_ok = std::is_copy_constructible<DecFunctor>::value;
    static constexpr bool move_ok = std::is_move_constructible<DecFunctor>::value;
    static constexpr bool no_view_ok = Config::isCopyable ? copy_ok : move_ok;
    static constexpr bool value = Config::isView ? true : no_view_ok;
  };

  // Check the move-constructor be deleted or not.
  template <typename Functor, typename = void>
  struct move_constructor_is_deleted : public std::true_type {};

  template <typename Functor>
  struct move_constructor_is_deleted<
    Functor, void_t<decltype(Functor(std::declval<Functor&&>()))>
  > : public std::false_type {};

  // `true` if the operator() is overloaded only once.
  template <typename Functor, typename = void>
  struct is_unique_callable : public std::false_type {};

  template <typename Functor>
  struct is_unique_callable<
    Functor, void_t<decltype(&Functor::operator())>>
  : public std::true_type {};

  // Check static callable functor.
#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)
  template <typename Fn, typename... Args>
  struct is_static_callable_functor : bool_constant<
    requires (Args&&... args) { Fn::operator()(std::forward<Args>(args)...); }
  > {};
#else
  template <typename Fn, typename... Args>
  struct is_static_callable_functor : std::false_type {};
#endif

  // Trait to add qualifiers (const, volatile, &, &&, noexcept) to a function
  // signature by mapping a `This` type and a base signature to a qualified function type.
  template <typename This, typename Signature>
  struct add_qualifier_like;

#define EMBED_DETAIL_ADD_QUALIFIER_WITH_THIS_DEFINE(C, V, REF, NOEXCEPT)  \
  template <typename This, typename Ret, typename... Args>                \
  struct add_qualifier_like<This C V REF, Ret(Args...) NOEXCEPT> {        \
    using type = Ret(Args...) C V REF;                                    \
    using sig_with_noexcept = Ret(Args...) C V REF NOEXCEPT;              \
    using sig_without_ref = Ret(Args...) C V NOEXCEPT;                    \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_ADD_QUALIFIER_WITH_THIS_DEFINE)

#undef EMBED_DETAIL_ADD_QUALIFIER_WITH_THIS_DEFINE

  // Implement the `get_unique_signature`.
  template <typename Fn, typename T, typename = void>
  struct get_unique_signature_impl {
    static_assert(always_false<T>::value,
      "T must be a function pointer or pointer to member function.");
  };

  // The Config::isThrowing of ebd::fn and ebd::unique_fn is true.
  // So `get_unique_signature_impl` will ignore the `noexcept` specifier.
#define EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE(C, V, REF, NOEXCEPT)        \
  template <typename Fn, typename Class, typename Ret, typename... Args>          \
  struct get_unique_signature_impl<Fn, Ret(Class::*)(Args...) C V REF NOEXCEPT> { \
    using type = Ret(Args...) C V REF;                                            \
    using sig_with_noexcept = Ret(Args...) C V REF NOEXCEPT;                      \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE)

#undef EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE

#if ( __cpp_explicit_this_parameter >= 202110L ) || ( EMBED_CXX_VERSION >= 202302L )

  // [dcl.fct]/6 Deducing this.
  // See <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html>.

  // `true` if the operator() of Fn is deducing this.
  template <typename Fn, typename This, typename Sig, typename... Args>
  concept deducing_this_call = requires (Fn f, Args&&... args) {
    static_cast<typename unwrap_signature<
      typename add_qualifier_like<This, Sig>::type
    >::template add_cvref_like<Fn>>(f)(std::forward<Args>(args)...);
  };

  // noexcept(false)
  template <typename Fn, typename This, typename Ret, typename... Args>
    requires deducing_this_call<Fn, This, Ret(Args...), Args...>
  struct get_unique_signature_impl<Fn, Ret(*)(This, Args...)>
  : public add_qualifier_like<This, Ret(Args...)> {};

  // noexcept(true)
  template <typename Fn, typename This, typename Ret, typename... Args>
    requires deducing_this_call<Fn, This, Ret(Args...) noexcept, Args...>
  struct get_unique_signature_impl<Fn, Ret(*)(This, Args...) noexcept>
  : public add_qualifier_like<This, Ret(Args...) noexcept> {};

#endif

#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)

  // [func.wrap.func.con]/16.2 Static operator().
  // See <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p1169r4.html>.

  // noexcept(false)
  template <typename Fn, typename Ret, typename... Args>
    requires is_static_callable_functor<Fn, Args...>::value
  struct get_unique_signature_impl<Fn, Ret(*)(Args...)> {
    using type = Ret(Args...) const;
    using sig_with_noexcept = Ret(Args...) const;
  };

  // noexcept(true)
  template <typename Fn, typename Ret, typename... Args>
    requires is_static_callable_functor<Fn, Args...>::value
  struct get_unique_signature_impl<Fn, Ret(*)(Args...) noexcept> {
    using type = Ret(Args...) const;
    using sig_with_noexcept = Ret(Args...) const noexcept;
  };

#endif

  // Get the signature of unique callable class.
  template <typename Functor, 
    bool Unique = is_unique_callable<Functor>::value>
  struct get_unique_signature {
    using type = void;
    using sig_with_noexcept = void;
  };

  template <typename Functor>
  struct get_unique_signature<Functor, /* Unique = */ true> {
    using impl_t = get_unique_signature_impl<Functor, decltype(&Functor::operator())>;
    using type = typename impl_t::type;
    using sig_with_noexcept = typename impl_t::sig_with_noexcept;
  };

  template <typename T>
  using get_unique_signature_t = typename get_unique_signature<T>::type;

  // Get class parameter type with qualifier.
  template <typename With, typename T, 
    bool IsClass = std::is_class<remove_cvref_t<T>>::value /* false */>
  struct get_qualified_with {
    using type = void;
  };

  template <typename With, typename T>
  struct get_qualified_with<With, T, /* IsClass = */ true> {
    using type = conditional_t<
      std::is_rvalue_reference<With>::value,
      remove_reference_t<T>&&, remove_reference_t<T>&
    >;
  };

  template <typename With, typename T>
  using get_qualified_with_t = typename get_qualified_with<With, T>::type;

  // Check is class and qualifier of call operator.
  template <typename Package, typename Fn, 
    bool IsClass = std::is_class<decay_t<Fn>>::value>
  struct is_class_call_operator {
    static constexpr bool sigCannotBeConst = false;
    static constexpr bool sigCannotBeVolatile = false;
  };

  template <typename Package, typename Fn>
  struct is_class_call_operator<Package, Fn, /* IsClass = */ true> {
    using call_const_res = invoke_result_package<const Fn, Package>;
    using call_volatile_res = invoke_result_package<volatile Fn, Package>;
    using call_lref_res = invoke_result_package<Fn&, Package>;
    using call_rref_res = invoke_result_package<Fn&&, Package>;

    // When const call is ill-formed, but one of rref_call, 
    // lref_call is valid, then the signature cannot be const.
    static constexpr bool sigCannotBeConst = 
      !is_invocable_impl<call_const_res, void>::type::value
      && (
        is_invocable_impl<call_rref_res, void>::type::value
        || is_invocable_impl<call_lref_res, void>::type::value
      );

    // When volatile call is ill-formed, but one of rref_call, 
    // lref_call is valid, then the signature cannot be volatile.
    static constexpr bool sigCannotBeVolatile = 
      !is_invocable_impl<call_volatile_res, void>::type::value
      && (
        is_invocable_impl<call_rref_res, void>::type::value
        || is_invocable_impl<call_lref_res, void>::type::value
      );
  };

  // Check the qualifier of signature and functor is matching.
  // The verification of the "&" and "&&" qualifier is in trait `is_callable_from`.
  // Here is the verification of "const" and "volatile" qualifier.
  template <typename Signature, typename Functor>
  struct qualifier_of_signature_match_functor {
    using base_fn = remove_cvref_t<Functor>;
    using unwrap_sig = unwrap_signature<Signature>;
    using call_op = is_class_call_operator<typename unwrap_sig::args, base_fn>;

    // The qualifier information of `Signature`.
    static constexpr bool sig_has_const = unwrap_sig::hasConst;
    static constexpr bool sig_has_volatile = unwrap_sig::hasVolatile;

    static constexpr bool const_match = 
      !(sig_has_const && call_op::sigCannotBeConst);
    static constexpr bool volatile_match = 
      !(sig_has_volatile && call_op::sigCannotBeVolatile);

    static constexpr bool value = const_match && volatile_match;
  };

  // Implement the `get_member_fn_type`
  template <typename Class, typename Signature, 
    bool IsLRef, bool IsRRef, 
    bool IsClass = std::is_class<remove_cvref_t<Class>>::value
  >
  struct get_member_fn_type_impl { using type = void; };

#define EMBED_DETAIL_GET_MEMBER_FN_TYPE_IMPL_DEFINE(C, V, REF, NOEXCEPT)  \
  template <typename Ret, typename Class,                                 \
    bool IsLRef, bool IsRRef, typename... Args>                           \
  struct get_member_fn_type_impl<Class C V REF,                           \
    Ret(Args...) NOEXCEPT, IsLRef, IsRRef, /* IsClass = */ true           \
  > {                                                                     \
    using type = conditional_t<IsLRef,                                    \
      Ret (Class::*) (Args...) C V & NOEXCEPT,                            \
      conditional_t<IsRRef || std::is_rvalue_reference<int REF>::value,   \
      Ret (Class::*) (Args...) C V && NOEXCEPT,                           \
      Ret (Class::*) (Args...) C V NOEXCEPT>>;                            \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_GET_MEMBER_FN_TYPE_IMPL_DEFINE)

#undef EMBED_DETAIL_GET_MEMBER_FN_TYPE_IMPL_DEFINE

  // From signature deduce the type of pointer to member function.
  template <typename Signature>
  struct get_member_fn_type {};

#define EMBED_DETAIL_GET_MEMBER_FN_TYPE_DEFINE(C, V, REF, NOEXCEPT) \
  template <typename Ret, typename Class, typename... Args>         \
  struct get_member_fn_type<Ret(Class, Args...) C V REF NOEXCEPT> { \
    using type = typename get_member_fn_type_impl<                  \
      Class, Ret(Args...) NOEXCEPT,                                 \
      std::is_lvalue_reference<int REF>::value,                     \
      std::is_rvalue_reference<int REF>::value                      \
    >::type;                                                        \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_GET_MEMBER_FN_TYPE_DEFINE)

#undef EMBED_DETAIL_GET_MEMBER_FN_TYPE_DEFINE

  // Get the type of pointer to member function.
  template <typename Signature>
  using get_member_fn_type_t = typename get_member_fn_type<Signature>::type;

  // MSVC 19.21 and earlier have a bug when using `sizeof(T) <= sizeof(void*)`
  // in `conditional_t`. To work around this issue and facilitate targeted
  // optimization for each platform, we create `is_reg_passable`.
  template <typename T>
  struct is_reg_passable {
    static constexpr std::size_t reg_size = sizeof(void*);
    static constexpr std::size_t obj_size = sizeof(T);
    static constexpr bool is_trivial_obj = is_call_trivial<T>::value;
    static constexpr bool is_scalar_obj = std::is_scalar<T>::value;
#if defined(__sparc_v8__) || defined(__sparcv8)
    // class and union object are not allowed to pass by reg in SPARC V8 (32bit).
    static constexpr bool value = is_scalar_obj;
#else
    static constexpr bool value =
      is_scalar_obj || (obj_size <= 2 * reg_size && is_trivial_obj);
#endif
  };

  // Used to choose either perfect forwarding or pass-by-value.
  // Pass-by-value is faster for scalar types because they can
  // be passed by the register rather than the stack.
#if !defined(EMBED_FN_CONFIG_DISABLE_SMART_FORWARD)
  template <typename T>
  using smart_forward_t = // vvv MSVC 19.10~19.14 workaround: avoid using `conditional_t`.
    typename std::conditional<is_reg_passable<T>::value, T, T&&>::type;
#else
  template <typename T>
  using smart_forward_t = T&&;
#endif

  // If the Config::isView is true, it cannot be qualified with '&' or '&&'. [P0792]
  template <typename Config, typename Signature>
  struct view_mode_qualifier_is_ok {
    static constexpr bool no_ref_qualifier = 
      !(unwrap_signature<Signature>::hasRRef || unwrap_signature<Signature>::hasLRef);
    static constexpr bool value = !Config::isView || no_ref_qualifier;
  };

  // Asserts for functor.
  template <std::size_t BufferSize, typename Config, typename Signature,
            typename Functor, typename Object, typename ErasureT>
  struct asserts_for_function : public std::true_type {

    static_assert(align_size_is_ok<Functor, Config, BufferSize, ErasureT>::value,
      "The `BufferSize` is smaller than the callable object. Please use bigger "
      "`BufferSize` and try again:\n\n"
      "        FnWrapper<Signature, Bigger-BufferSize> f = CallableObject;\n"
      "                             ^^^^^^^^^^^^^^^^^\n"
      "                                     |\n"
      "             should be greater than `sizeof(CallableObject)`\n\n"
      "`FnWrapper` can be `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn`, etc."
    );

    static_assert(assert_throwing_is_ok<Functor, Object, Config>::value,
      "The 'Functor' may throw exceptions during construction and destruction,"
      " which does not match the 'Config::assertNoThrow = true' setting.");

    static_assert(copyable_is_ok<Functor, Config>::value, 
      "Functor cannot match the Config::isCopyable setting.");

    static_assert(!move_constructor_is_deleted<Functor>::value || Config::isView,
      "The move constructor of Functor shouldn't be deleted.");

    static_assert(qualifier_of_signature_match_functor<Signature, Functor>::value,
      "The qualifier 'const', '&' or '&&' of operator() of Functor"
      " cannot match that of Signature.");

    static_assert(view_mode_qualifier_is_ok<Config, Signature>::value,
      "'&' and '&&' are not allowed to qualify the function view.");
  };

  // Is type std::in_place_type_t<T>.
  template <typename>
  struct is_in_place_type : public std::false_type {};

#if EMBED_CXX_VERSION >= 201703L

  template <typename T>
  struct is_in_place_type<std::in_place_type_t<T>> : public std::true_type {};

#endif

  // <https://eel.is/c++draft/func.wrap#ref.ctor-1>.
  template <typename Sig, typename Tuple, 
    typename PureSig = typename unwrap_signature<Sig>::pure_sig>
  struct is_invocable_using_impl;
  template <typename Sig, typename... TArgs, typename Ret, typename... Args>
  struct is_invocable_using_impl<Sig, std::tuple<TArgs...>, Ret(Args...)> {
    using type = conditional_t<
      unwrap_signature<Sig>::isNoexcept,
      is_nothrow_invocable_r<Ret, TArgs..., Args...>,
      is_invocable_r<Ret, TArgs..., Args...>
    >;
  };

  // [func.wrap.ref.ctor]/1 is-invokable-using
  template <typename Signature, typename... T>
  using is_invocable_using_t = 
    typename is_invocable_using_impl<Signature, std::tuple<T...>>::type;

  template <typename T>
  struct is_constant_wrapper : std::false_type {};

#if __cpp_lib_constant_wrapper >= 202603L
  template <auto Cw, typename T>
  struct is_constant_wrapper<std::constant_wrapper<Cw, T>> : std::true_type {};
#endif

  template <typename Functor, typename FnSample, typename Sig, typename = void>
  struct noexcept_qualify_like { using type = Sig; };

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )

  template <typename T, typename = void>
  struct noexcept_qualify_like_helper : std::false_type {};

  template <typename Ret, typename... Args>
  struct noexcept_qualify_like_helper<Ret(*)(Args...) noexcept, void>
  : std::true_type {};

  template <typename Mp, typename Class>
  struct noexcept_qualify_like_helper<Mp Class::*> : std::true_type {};

  template <typename Functor>
  struct noexcept_qualify_like_helper<
    Functor, enable_if_t<is_unique_callable<Functor>::value>>
  : bool_constant<unwrap_signature<
    typename get_unique_signature<Functor>::sig_with_noexcept
  >::isNoexcept> {};

# define EMBED_DETAIL_HELPER_MEMPTR_DEFINE(C, V, REF, NOEXCEPT)               \
  template <typename Ret, typename Class, typename... Args>                   \
  struct noexcept_qualify_like_helper<Ret(Class::*)(Args...) C V REF NOEXCEPT>\
  : bool_constant<unwrap_signature<void() NOEXCEPT>::isNoexcept> {};

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_HELPER_MEMPTR_DEFINE)

# undef EMBED_DETAIL_HELPER_MEMPTR_DEFINE

# define EMBED_DETAIL_NOEXCEPT_QUALIFY_LIKE_DEFINE(C, V, REF, NOEXCEPT)     \
  template <typename Functor, typename Ret, typename... Args,               \
    std::size_t Buf, typename Sig,                                          \
    bool IsCopyable, bool IsView, bool IsThrowing, bool AssertObjectNoThrow \
  > struct noexcept_qualify_like<                                           \
    /* Functor = */ Functor,                                                \
    /* FnSample = */ function<Buf, config_package<                          \
      IsCopyable, IsView, IsThrowing, AssertObjectNoThrow>, Sig>,           \
    /* Sig = */ Ret(Args...) C V REF NOEXCEPT,                              \
    enable_if_t<IsView || !IsThrowing>                                      \
  > {                                                                       \
    using is_nothrow = typename noexcept_qualify_like_helper<Functor>::type;\
/* MSVC 14.36~14.44 regression: noexcept(is_nothrow::value) trigger ICE. */ \
    using sig_normal = conditional_t<is_nothrow::value,                     \
      Ret(Args...) C V REF noexcept, Ret(Args...) C V REF>;                 \
    using sig_view = conditional_t<is_nothrow::value,                       \
      Ret(Args...) C V noexcept, Ret(Args...) C V>;                         \
    using type = conditional_t<IsView, sig_view, sig_normal>;               \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_NOEXCEPT_QUALIFY_LIKE_DEFINE)

# undef EMBED_DETAIL_NOEXCEPT_QUALIFY_LIKE_DEFINE

#endif

  // Add noexcept qualifier if the Functor is noexcept free function, 
  // and the function wrapper or reference support `noexcept`.
  template <typename Functor, template <class, std::size_t> class Fn, typename Sig>
  using noexcept_qualify_like_t = 
    typename noexcept_qualify_like<decay_t<Functor>, Fn<void(), sizeof(void(*)())>, Sig>::type;

  // Check if `T` is the stateless standard operator wrapper.
  template <typename T> struct is_std_op_wrapper : std::false_type {};
  template <typename T> struct is_std_op_wrapper<std::equal_to<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::not_equal_to<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::greater<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::less<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::greater_equal<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::less_equal<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::plus<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::minus<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::multiplies<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::divides<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::modulus<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::negate<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::logical_and<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::logical_or<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::logical_not<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::bit_and<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::bit_or<T>> : std::true_type {};
  template <typename T> struct is_std_op_wrapper<std::bit_xor<T>> : std::true_type {};
#if EMBED_CXX_VERSION >= 201402L
  template <typename T> struct is_std_op_wrapper<std::bit_not<T>> : std::true_type {};
#endif
#if EMBED_CXX_VERSION >= 202002L
  template <> struct is_std_op_wrapper<std::identity>: std::true_type {};
# if __cpp_lib_ranges >= 201911L
  template <> struct is_std_op_wrapper<std::ranges::equal_to> : std::true_type {};
  template <> struct is_std_op_wrapper<std::ranges::not_equal_to> : std::true_type {};
  template <> struct is_std_op_wrapper<std::ranges::less> : std::true_type {};
  template <> struct is_std_op_wrapper<std::ranges::greater> : std::true_type {};
  template <> struct is_std_op_wrapper<std::ranges::less_equal> : std::true_type {};
  template <> struct is_std_op_wrapper<std::ranges::greater_equal> : std::true_type {};
# endif // ^^^ __cpp_lib_ranges >= 201911L
# if __cpp_lib_three_way_comparison >= 201907L
  template <> struct is_std_op_wrapper<std::compare_three_way> : std::true_type {};
# endif // ^^^ __cpp_lib_three_way_comparison >= 201907L
#endif

  // Check empty and normal callable functor.
  // Lambda has trivially default constructor since C++20.
  // See <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0624r2.pdf>.
  template <typename Fn>
  struct is_empty_normal : bool_constant<
    std::is_empty<Fn>::value && std::is_trivially_default_constructible<Fn>::value
    && std::is_trivially_destructible<Fn>::value
  > {};

  // Check whether the functor is stateless.
  template <bool IsView, typename Fn, typename... Args>
  struct is_stateless : bool_constant<
    is_static_callable_functor<Fn, Args...>::value
    || is_std_op_wrapper<Fn>::value
    || (is_empty_normal<Fn>::value && !IsView)
    // ^^^ empty trivial functor may use `this` in operator(). This is
    // not strict stateless and cannot be used in reference semantic.
  > {};

  // Log error for make_fn.
  template <typename Unused>
  constexpr bool make_fn_log_error() noexcept {
    static_assert(always_false<Unused>::value,
      "`make_fn()` CANNOT infer the template arguments of `ebd::basic_fn` from the given "
      "arguments.\nYou can specify the signature and try again:\n\n"
      "        auto f = ebd::make_fn<Signature>(CallableObject);\n"
      "                              ^^^^^^^^^\n"
      "        auto f = ebd::make_fn<FnWrapper, Signature>(CallableObject);\n"
      "                                         ^^^^^^^^^\n\n"
      "The `Signature` is like `void()`, `float(int,int) const`;\n"
      "The `FnWrapper` is an alias of `ebd::basic_fn` and has `template <class, std::size_t>` "
      "as a template argument list, such as `ebd::fn_ref`, `ebd::safe_fn`, etc. If omitted, "
      "the `FnWrapper` will be inferred to be `ebd::fn` if the `CallableObject` is copyable, "
      "and `ebd::unique_fn` otherwise."
    );
    return true;
  }

  // `true` if Cfg::assertNoThrow || Cfg::isView
  template <typename Cfg>
  struct is_cfg_noexcept : bool_constant<Cfg::assertNoThrow || Cfg::isView> {};

  template <typename Signature>
  struct skip_first_arg_sig;

#define EMBED_DETAIL_SKIP_FIRST_ARG_SIG_DEFINE(C, V, REF, NOEXCEPT) \
  template <typename Ret, typename First, typename... Args>         \
  struct skip_first_arg_sig<Ret(First, Args...) C V REF NOEXCEPT> { \
    using type = typename add_qualifier_like<                       \
      First, Ret(Args...) NOEXCEPT>::sig_without_ref;               \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_SKIP_FIRST_ARG_SIG_DEFINE)

#undef EMBED_DETAIL_SKIP_FIRST_ARG_SIG_DEFINE

  template <typename Signature>
  using skip_first_arg_sig_t = typename skip_first_arg_sig<Signature>::type;

} // end namespace fn_traits

// In the namespace "erasure_type", we define a series of 
// types for objects that implement type erasure.
namespace erasure_type {

  // Reference storage, which used in view mode.
  union ErasureRefStorage {
    void*       fill_ptr;
    const void* fill_const_ptr;
    void (*fill_func_ptr) ();
  };

  template <std::size_t Size>
  union EMBED_DETAIL_ALIAS ErasureCore {
    static_assert(Size > 0, "erasure size must greater than 0");
    // An array of `unsigned char` can be used to hold other objects.
    // See <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0137r1.html>.
    unsigned char pod[Size];
    ErasureRefStorage ref_storage; // alignas(ref_storage)
  };

  // Passing the `ErasureBase*` as a parameter can avoid the 
  // ABI incompatibility issue between `Erasure<A>&` and `Erasure<B>&`.
  struct ErasureBase {};

  /// @note Erasure is trivial.
  // The well-defined operation of reusing its storage space is to use
  // placement new. After that, using `access` to obtain the address or reference
  // (rather than the content) is also in accordance with the C++ standard.
  // See <https://eel.is/c++draft/basic.life#7>.
  template <std::size_t Size>
  struct EMBED_DETAIL_ALIAS Erasure : public ErasureBase {
    alignas(default_buffer_size::align_value)
    ErasureCore<Size> m_core;

    // Access the pointer of erasureCore that qualified with nothing or const.
    void* access() noexcept { return &m_core.pod[0]; }
    const void* access() const noexcept { return &m_core.pod[0]; }

    // Access the pointer of erasureCore that qualified with volatile or const volatile.
    volatile void* access() volatile noexcept { return &m_core.pod[0]; }
    const volatile void* access() const volatile noexcept { return &m_core.pod[0]; }

    template <typename T>
    T& access() noexcept
    { return *EMBED_DETAIL_LAUNDER(static_cast<T*>(access())); }

    template <typename T>
    const T& access() const noexcept
    { return *EMBED_DETAIL_LAUNDER(static_cast<const T*>(access())); }

    template <typename T>
    volatile T& access() volatile noexcept
    { return *EMBED_DETAIL_LAUNDER(static_cast<volatile T*>(access())); }

    template <typename T>
    const volatile T& access() const volatile noexcept
    { return *EMBED_DETAIL_LAUNDER(static_cast<const volatile T*>(access())); }
  };

  // ABI for passing either pointer or value.
  union ErasurePass {
    ErasureBase*      ptr;
    ErasureRefStorage val;
  };

} // end namespace erasure_type

// In the namespace "invocation", we define a series of 
// types for objects that implement the behaviour of invocation.
namespace invocation {

// Implement invocation for static call operator.
#if (EMBED_CXX_VERSION >= 202302L && __cpp_static_call_operator >= 202207L)
# define EMBED_DETAIL_STATIC_CALL_INVOKER_IMPL(C, V, REF, NOEXCEPT)             \
  struct static_call {                                                          \
    template <typename Functor>                                                 \
    static Ret invoke(erasure_pass_t, smart_forward_t<Args>... args) NOEXCEPT { \
      return Functor::operator()(std::forward<Args>(args)...);                  \
    }                                                                           \
  }; /* end static_call */
#else
# define EMBED_DETAIL_STATIC_CALL_INVOKER_IMPL(C, V, REF, NOEXCEPT) \
  struct static_call { /* empty struct */ };
#endif

// Implement invocation for constant_wrapper.
#if __cpp_lib_constant_wrapper >= 202603L
# define EMBED_DETAIL_CW_INVOKER_IMPL(C, V, REF, NOEXCEPT)                            \
  struct view_cw {                                                                    \
    template <typename Cw>                                                            \
    static Ret invoke(erasure_pass_t, smart_forward_t<Args>... args) NOEXCEPT {       \
      return invoke_r<Ret>(Cw::value, std::forward<Args>(args)...);                   \
    }                                                                                 \
    template <typename Cw, typename Obj, bool CallPointer>                            \
    static Ret invoke(erasure_pass_t base, smart_forward_t<Args>... args) NOEXCEPT {  \
      if constexpr (CallPointer) {                                                    \
        auto* obj_ptr = static_cast<Obj C V*>(base.val.fill_ptr);                     \
        return invoke_r<Ret>(Cw::value, obj_ptr, std::forward<Args>(args)...);        \
      } else {                                                                        \
        auto& obj = *static_cast<Obj C V*>(base.val.fill_ptr);                        \
        return invoke_r<Ret>(Cw::value, obj, std::forward<Args>(args)...);            \
      }                                                                               \
    }                                                                                 \
  }; /* end view_cw */
#else
# define EMBED_DETAIL_CW_INVOKER_IMPL(C, V, REF, NOEXCEPT)
#endif

  template <std::size_t Size, typename Config, typename Signature>
  struct InvokerImpl;

#define EMBED_DETAIL_INVOKER_IMPL_DEFINE(C, V, REF, NOEXCEPT)                     \
  template <std::size_t Size, typename Config,                                    \
    typename Ret, typename... Args>                                               \
  struct InvokerImpl<Size, Config, Ret(Args...) C V REF NOEXCEPT> {               \
  public:                                                                         \
    using erasure_base_t = erasure_type::ErasureBase C V;                         \
    using erasure_pass_t = erasure_type::ErasurePass C;                           \
    using erasure_t = erasure_type::Erasure<Size> C V;                            \
    static constexpr bool is_rvalue_ref =                                         \
      std::is_rvalue_reference<int REF>::value;                                   \
    using invoker_type =                                                          \
      Ret (*) (erasure_pass_t erased, smart_forward_t<Args>... args);             \
                                                                                  \
    /* Using when M_erasure is empty. */                                          \
    struct empty {                                                                \
      static Ret invoke(erasure_pass_t, smart_forward_t<Args>...) {               \
        throw_or_terminate<Config::isThrowing>();                                 \
        /* Unreachable: throw_or_terminate() is [[noreturn]] */                   \
      }                                                                           \
    };                                                                            \
                                                                                  \
    /* Using when Config::isView == false. */                                     \
    struct inplace {                                                              \
      template <typename Functor>                                                 \
      static Ret invoke(erasure_pass_t base, smart_forward_t<Args>... args) {     \
        auto* erased = static_cast<erasure_t C V*>(base.ptr);                     \
        auto& fn = erased->template access<Functor>();                            \
        using Fn = conditional_t<is_rvalue_ref,                                   \
          remove_reference_t<decltype(fn)>&&,                                     \
          remove_reference_t<decltype(fn)>&>;                                     \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
    };                                                                            \
                                                                                  \
    /* Using when Config::isView == true. */                                      \
    struct view {                                                                 \
      /* Using when Functor::is_stored_origin == true. */                         \
      template <typename Functor>                                                 \
      static enable_if_t<is_stored_origin<Functor, true>::value, Ret>             \
      invoke(erasure_pass_t base, smart_forward_t<Args>... args) {                \
        auto* fn = reinterpret_cast<Functor>(base.val.fill_func_ptr);             \
        using Fn = remove_reference_t<decltype(fn)>&;                             \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
                                                                                  \
      /* Using when Functor::is_stored_origin == false. */                        \
      template <typename Functor>                                                 \
      static enable_if_t<!is_stored_origin<Functor, true>::value, Ret>            \
      invoke(erasure_pass_t base, smart_forward_t<Args>... args) {                \
        auto& fn = *(static_cast<Functor C V*>(base.val.fill_ptr));               \
        using Fn = remove_reference_t<decltype(fn)>&;                             \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
    };                                                                            \
                                                                                  \
    /* Used for stateless(empty) Functor (like std::less). */                     \
    struct empty_normal {                                                         \
      template <typename EmptyFn>                                                 \
      static Ret invoke(erasure_pass_t, smart_forward_t<Args>... args) {          \
        C V EmptyFn fn{};  /* Empty and trivial class. It is stateless */         \
        using Fn = conditional_t<is_rvalue_ref,                                   \
          remove_reference_t<decltype(fn)>&&,                                     \
          remove_reference_t<decltype(fn)>&>;                                     \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
    };                                                                            \
                                                                                  \
    EMBED_DETAIL_STATIC_CALL_INVOKER_IMPL(C, V, REF, NOEXCEPT)                    \
    EMBED_DETAIL_CW_INVOKER_IMPL(C, V, REF, NOEXCEPT)                             \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_INVOKER_IMPL_DEFINE)

#undef EMBED_DETAIL_INVOKER_IMPL_DEFINE
#undef EMBED_DETAIL_CW_INVOKER_IMPL

} // end namespace invocation

// In the namespace "management", we define a series of 
// types for objects that implement the behaviour of management.
namespace management {

  enum class OperatorCode {
    clone = 0,  // Indicates that the manager should clone the object.
    move,       // Indicates that the manager should move the object.
    destroy     // Indicates that the manager should destroy the object.
  };

  template <std::size_t Size, typename Config, typename Signature>
  struct ManagerImpl {
  private:
    using invoke_impl_t = invocation::InvokerImpl<Size, Config, Signature>;
    using erasure_base_t  = typename invoke_impl_t::erasure_base_t;
    using erasure_t       = typename invoke_impl_t::erasure_t;
  public:
    using manager_type = void (*) (
      OperatorCode op, erasure_base_t* dst, erasure_base_t* src);

    // Get functor pointer without any qualifier.
    template <typename Functor>
    static Functor* get_pointer(erasure_base_t* src) noexcept {
      auto* src_ = static_cast<erasure_t*>(src);
      auto& fn = src_->template access<Functor>();
      return const_cast<Functor*>(std::addressof(fn));
    }

    // Use placement new to create a type-erased object.
    template <typename Functor, typename Object>
    static void create(erasure_base_t* target, Object&& obj)
    noexcept(std::is_nothrow_constructible<Functor, Object&&>::value) {
      ::new (const_cast<void*>(static_cast<erasure_t*>(target)->access()))
          Functor(std::forward<Object>(obj));
    }

    /// @brief Store the object pointer from function reference without placement new.
    /// @note Avoid using placement new to make the function constexpr in C++20.
    template <typename Object>
    static EMBED_CXX20_CONSTEXPR void 
    ref_create(erasure_base_t* target, Object* obj) noexcept {
      using pure_erasure_t = remove_cv_t<erasure_t>;
      const_cast<pure_erasure_t*>(static_cast<erasure_t*>(target))
          ->m_core.ref_storage.fill_ptr = const_cast<remove_cv_t<Object>*>(obj);
    }

#if EMBED_CXX_VERSION >= 201703L

    // In-place create target object. (Cooperate with std::in_place_type)
    template <typename Functor, typename... CArgs>
    static void emplace_create(erasure_base_t* target, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<Functor, CArgs&&...>::value) {
      ::new (const_cast<void*>(static_cast<erasure_t*>(target)->access()))
          Functor(std::forward<CArgs>(args)...);
    }

#endif

    // Destroy a type-erased object.
    template <typename Functor>
    static void destroy(erasure_base_t* victim)
    noexcept(std::is_nothrow_destructible<Functor>::value) {
      auto* victim_ = static_cast<erasure_t*>(victim);
      victim_->template access<Functor>().~Functor();
    }

    // Clone type-erased object from `src` to `dst`.
    /// @attention `clone` will never change @a src.
    template <typename Functor>
    static void clone(erasure_base_t* EMBED_RESTRICT dst, erasure_base_t* EMBED_RESTRICT src)
    noexcept(std::is_nothrow_copy_constructible<Functor>::value) {
      const auto& src_obj = *get_pointer<Functor>(src);
      create<Functor>(dst, src_obj);
    }

    // Move type-erased object from `src` to `dst`.
    template <typename Functor>
    static void move(erasure_base_t* EMBED_RESTRICT dst, erasure_base_t* EMBED_RESTRICT src)
    noexcept(std::is_nothrow_move_constructible<Functor>::value) {
      auto& src_obj = *get_pointer<Functor>(src);
      create<Functor>(dst, std::move(src_obj));
    }

    // Using when M_erasure is empty.
    struct empty {
      static void manage(OperatorCode, erasure_base_t*, erasure_base_t*) {
        // Nothing here.
      }
    };

    // Using when Config::isView == false.
    struct inplace {
      // Using when the Functor is copyable and not trivial.
      template <typename Functor, bool IsCopyable>
      static enable_if_t<IsCopyable && !is_traditional_trivial<Functor>::value>
      /* copyable */ manage(
        OperatorCode op, 
        erasure_base_t* EMBED_RESTRICT dst, 
        erasure_base_t* EMBED_RESTRICT src
      ) {
        switch (op) {
        case OperatorCode::clone:
          clone<Functor>(dst, src);
          break;
        case OperatorCode::move:
          move<Functor>(dst, src);
          break;
        case OperatorCode::destroy:
          destroy<Functor>(dst);
          break;
        default: EMBED_DETAIL_UNREACHABLE(); break;
        }
      }

      // Using when the Functor is move only and not trivial.
      template <typename Functor, bool IsCopyable>
      static enable_if_t<!IsCopyable && !is_traditional_trivial<Functor>::value>
      /* move-only */ manage(
        OperatorCode op, 
        erasure_base_t* EMBED_RESTRICT dst, 
        erasure_base_t* EMBED_RESTRICT src
      ) {
        switch (op) {
        case OperatorCode::clone:
          EMBED_DETAIL_UNREACHABLE(); // move only
          break;
        case OperatorCode::move:
          move<Functor>(dst, src);
          break;
        case OperatorCode::destroy:
          destroy<Functor>(dst);
          break;
        default: EMBED_DETAIL_UNREACHABLE(); break;
        }
      }

      // Used when the Functor is trivial.
      template <typename Functor, bool IsCopyable>
      static enable_if_t<is_traditional_trivial<Functor>::value> manage(
        OperatorCode op, 
        erasure_base_t* EMBED_RESTRICT dst, 
        erasure_base_t* EMBED_RESTRICT src
      ) {
        switch (op) {
        case OperatorCode::clone: EMBED_FALLTHROUGH(); /* fallthrough */
        case OperatorCode::move:
          std::memcpy(
            const_cast<void*>(static_cast<erasure_t*>(dst)->access()),
            const_cast<const void*>(static_cast<erasure_t*>(src)->access()),
            sizeof(erasure_t)
          );
          break;
        case OperatorCode::destroy: /* Do nothing */ break;
        default: EMBED_DETAIL_UNREACHABLE(); break;
        }
      }

    }; // end inplace
  };

} // end namespace management

namespace command {

  /// @note CommandTable is trivial
  template <bool IsView, std::size_t Size, 
    typename Config, typename Signature, typename ArgsPackage>
  struct CommandTable;

  // Command Table for inplace mode.
  template <std::size_t Size, typename Config, typename Signature, typename... Args>
  struct CommandTable</* IsView = */ false, Size, Config, Signature, args_package<Args...>> {
    using invoker_impl_t = invocation::InvokerImpl<Size, Config, Signature>;
    using manager_impl_t = management::ManagerImpl<Size, Config, Signature>;
    using invoker_t       = typename invoker_impl_t::invoker_type;
    using erasure_base_t  = typename invoker_impl_t::erasure_base_t;
    using erasure_pass_t  = typename invoker_impl_t::erasure_pass_t;
    using manager_t       = typename manager_impl_t::manager_type;

    manager_t m_manager;
    invoker_t m_invoker;

    auto invoke(erasure_pass_t erased, Args&&... args) const
    noexcept(unwrap_signature<Signature>::isNoexcept)
    -> typename unwrap_signature<Signature>::ret {
      return m_invoker(erased, std::forward<Args>(args)...);
    }

    void clone(erasure_base_t* EMBED_RESTRICT dst, erasure_base_t* EMBED_RESTRICT src) const
    noexcept(Config::assertNoThrow) {
      m_manager(management::OperatorCode::clone, dst, src);
    }

    void move(erasure_base_t* EMBED_RESTRICT dst, erasure_base_t* EMBED_RESTRICT src) const
    noexcept(Config::assertNoThrow) {
      m_manager(management::OperatorCode::move, dst, src);
    }

    void destroy(erasure_base_t* dst) const
    noexcept(Config::assertNoThrow) {
      m_manager(management::OperatorCode::destroy, dst, nullptr);
    }

    // Empty init.
    EMBED_CXX14_CONSTEXPR void set_empty() noexcept {
      m_invoker = &invoker_impl_t::empty::invoke;
      m_manager = &manager_impl_t::empty::manage;
    }

    // Check the `m_invoker` is empty::invoke. (constexpr && noexcept)
    EMBED_NODISCARD constexpr bool is_empty() const noexcept {
      return m_invoker == &invoker_impl_t::empty::invoke;
    }

    // Initialize owning function wrapper. (Enable if Functor is NOT stateless.)
    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    enable_if_t<!is_stateless</*IsView*/false, DecFunctor, Args...>::value>
    init(erasure_base_t* target, Functor&& obj)
    noexcept(std::is_nothrow_constructible<DecFunctor, Functor&&>::value) {
      manager_impl_t::template create<DecFunctor>(target, std::forward<Functor>(obj));
      m_invoker = &invoker_impl_t::inplace::template invoke<DecFunctor>;
      m_manager = &manager_impl_t::inplace::template manage<DecFunctor, Config::isCopyable>;
    }

    // Initialize owning function wrapper. (Enable if Functor is stateless.)
    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    EMBED_CXX20_CONSTEXPR enable_if_t<is_stateless</*IsView*/false, DecFunctor, Args...>::value>
    init(erasure_base_t*, Functor&&) noexcept {
      using invoker_impl_target_t = conditional_t<
        is_static_callable_functor<DecFunctor, Args...>::value,
        typename invoker_impl_t::static_call,
        typename invoker_impl_t::empty_normal
      >;
      m_invoker = &invoker_impl_target_t::template invoke<DecFunctor>;
      m_manager = &manager_impl_t::empty::manage;
    }

#if EMBED_CXX_VERSION >= 201703L

    // In-place initialize the target with specified arguments.
    template <typename Functor, typename DecFunctor = decay_t<Functor>, typename... CArgs>
    void emplace_init(erasure_base_t* target, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<DecFunctor, CArgs&&...>::value) {
      manager_impl_t::template emplace_create<DecFunctor>(
        target, std::forward<CArgs>(args)...);
      m_invoker = &invoker_impl_t::inplace::template invoke<DecFunctor>;
      m_manager = &manager_impl_t::inplace::template manage<DecFunctor, Config::isCopyable>;
    }

#endif
  };

  // Command Table for view mode.
  template <std::size_t Size, typename Config, typename Signature, typename... Args>
  struct CommandTable</* IsView = */ true, Size, Config, Signature, args_package<Args...>> {
    using invoker_impl_t = invocation::InvokerImpl<Size, Config, Signature>;
    using manager_impl_t = management::ManagerImpl<Size, Config, Signature>;
    using invoker_t       = typename invoker_impl_t::invoker_type;
    using erasure_base_t  = typename invoker_impl_t::erasure_base_t;
    using erasure_pass_t  = typename invoker_impl_t::erasure_pass_t;
    using erasure_t       = typename invoker_impl_t::erasure_t;

    // No m_manager because IsView = true.
    invoker_t m_invoker;

    auto invoke(erasure_pass_t erased, Args&&... args) const
    noexcept(unwrap_signature<Signature>::isNoexcept)
    -> typename unwrap_signature<Signature>::ret {
      return m_invoker(erased, std::forward<Args>(args)...);
    }

    void clone(erasure_base_t* EMBED_RESTRICT dst, erasure_base_t* EMBED_RESTRICT src)
    const noexcept {
      auto* destination = static_cast<erasure_t*>(dst);
      auto* source = static_cast<erasure_t*>(src);
      std::memcpy(
        const_cast<void*>(destination->access()), 
        const_cast<const void*>(source->access()), 
        sizeof(erasure_t)
      );
    }

    void move(erasure_base_t*, erasure_base_t*) = delete;
    void destroy(erasure_base_t*) = delete;

    // Initialize non-owning function wrapper. (Enable if the functor is function pointer(FP))
    template <bool IsStoredOrigin, typename Functor, typename DecFunctor = decay_t<Functor>>
    enable_if_t<IsStoredOrigin> /* Enable if the functor is function pointer(FP) */
    init(erasure_base_t* target, Functor&& obj) noexcept {
      // Since the `is_stored_origin<Functor>` is true, then it must
      // be function pointer which have nothing about ownership.
      manager_impl_t::template create<DecFunctor>(target, std::forward<Functor>(obj));
      m_invoker = &invoker_impl_t::view::template invoke<DecFunctor>;
    }

    // Initialize non-owning function wrapper. (Enable if the functor is neither FP nor stateless-fn)
    template <bool IsStoredOrigin, typename Functor, typename DecFunctor = decay_t<Functor>>
    EMBED_CXX20_CONSTEXPR
    enable_if_t<!IsStoredOrigin && !is_stateless</*IsView*/true, DecFunctor, Args...>::value>
    init(erasure_base_t* target, Functor&& obj) noexcept {
      // User has to make sure the callable object must remain alive while the function_ref is in use.
      manager_impl_t::template ref_create<>(target, std::addressof(obj));
      m_invoker = &invoker_impl_t::view::template invoke<DecFunctor>;
    }

    // Initialize non-owning function wrapper. (Enable if the functor is stateless-fn)
    template <bool IsStoredOrigin, typename Functor, typename DecFunctor = decay_t<Functor>>
    EMBED_CXX20_CONSTEXPR
    enable_if_t<!IsStoredOrigin && is_stateless</*IsView*/true, DecFunctor, Args...>::value>
    init(erasure_base_t*, Functor&&) noexcept {
      using invoker_impl_target_t = conditional_t<
        is_static_callable_functor<DecFunctor, Args...>::value,
        typename invoker_impl_t::static_call,
        typename invoker_impl_t::empty_normal
      >;
      m_invoker = &invoker_impl_target_t::template invoke<DecFunctor>;
    }

#if __cpp_lib_constant_wrapper >= 202603L

    /// @brief Initialize the m_invoker from given std::constant_wrapper.

    template <typename Cw>
    constexpr void cw_init() noexcept {
      m_invoker = &invoker_impl_t::view_cw::template invoke<Cw>;

      // Mandates are as follows.
      if constexpr (sizeof...(Args) > 0 && (requires {
          typename std::constant_wrapper<remove_cvref_t<Args>::value>; } && ...)) {
        static_assert(!requires { typename std::constant_wrapper<
              std::invoke(Cw::value, remove_cvref_t<Args>::value...)>;
          }, "The argument types of fn_ref are all constexpr-param, and the"
          " INVOKE result can be wrapped into std::constant_wrapper. This"
          " means that you can simply use std::invoke(f, args...) instead"
          " of fn_ref to avoid indirect INVOKE."
        );
      }
    }

    template <typename Cw, bool CallPointer, typename Obj>
    constexpr void cw_init(erasure_base_t* target, Obj* obj_ptr) noexcept {
      manager_impl_t::template ref_create<>(target, obj_ptr);
      m_invoker = &invoker_impl_t::view_cw::template invoke<Cw, Obj, CallPointer>;
    }

#endif
  };

} // end namespace command

// Move the function's implementation to the base class to simplify the code.
namespace crtp_mixins {

  // Implement the 'operator()' for function.
  template <bool IsView, typename Signature, typename Self>
  struct operator_call_impl; // Undefined

#define EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE(C, V, REF, NOEXCEPT)         \
  template <bool IsView, typename Ret, typename Self, typename... Args>     \
  struct operator_call_impl<IsView, Ret(Args...) C V REF NOEXCEPT, Self> {  \
  public:                                                                   \
    EMBED_DETAIL_ALL_DEFAULT(operator_call_impl)                            \
                                                                            \
    Ret operator()(Args... args) C V REF NOEXCEPT {                         \
      using erasure_t = typename Self::erasure_t;                           \
      using command_t = const typename Self::command_t;                     \
      using pass_t = typename command_t::erasure_pass_t;                    \
      remove_cv_t<pass_t> erased;                                           \
      auto* self_q = static_cast<Self C V*>(this);                          \
      auto& cmd = const_cast<command_t&>(self_q->m_command);                \
      erased.ptr = const_cast<erasure_t*>(&(self_q->m_erasure));            \
      return cmd.invoke(erased, std::forward<Args>(args)...);               \
    }                                                                       \
  };                                                                        \
                                                                            \
  /* Specialized for `ebd::fn_ref`, to make its operator() behavior */      \
  /* consistent with `std::function_ref`. */                                \
  template <typename Ret, typename Self, typename... Args>                  \
  struct operator_call_impl</* IsView = */ true,                            \
    Ret(Args...) C V REF NOEXCEPT, Self> {                                  \
  public:                                                                   \
    EMBED_DETAIL_ALL_DEFAULT(operator_call_impl)                            \
                                                                            \
    Ret operator()(Args... args) const V NOEXCEPT {                         \
      using erasure_t = typename Self::erasure_t;                           \
      using command_t = const typename Self::command_t;                     \
      using pass_t = typename command_t::erasure_pass_t;                    \
      remove_cv_t<pass_t> erased;                                           \
      auto* self_q = static_cast<Self const V*>(this);                      \
      auto& erasure = const_cast<erasure_t&>(self_q->m_erasure);            \
      auto& cmd = const_cast<command_t&>(self_q->m_command);                \
      erased.val = erasure.m_core.ref_storage;                              \
      return cmd.invoke(erased, std::forward<Args>(args)...);               \
    }                                                                       \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE)

#undef EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE

  // Implement the destructor.
  template <typename Config, typename Self>
  struct destructor_impl {
    EMBED_DETAIL_COPY_FUNCTION(destructor_impl, default)
    EMBED_DETAIL_MOVE_FUNCTION(destructor_impl, default)
    destructor_impl() = default;

    ~destructor_impl() noexcept(Config::assertNoThrow) {
      using erasure_t = typename Self::erasure_t;
      auto& self = static_cast<const Self&>(*this);
      self.m_command.destroy(&const_cast<erasure_t&>(self.m_erasure));
    }
  };

  // Implement the move constructor and move assignment.
  template <typename Config, typename Self>
  struct move_impl {
    EMBED_DETAIL_DTOR_ECTOR_DEFAULT(move_impl);
    EMBED_DETAIL_COPY_FUNCTION(move_impl, default);

    move_impl(move_impl&& other_raw) noexcept(Config::assertNoThrow) {
      // Get the real `self` and `other`.
      auto& self = static_cast<Self&>(*this);
      auto&& other = static_cast<Self&&>(other_raw);
      using command_t = typename Self::command_t;

      // Move from `other` to `self`.
      other.m_command.move(&self.m_erasure, &other.m_erasure);
      std::memcpy(&self.m_command, &other.m_command, sizeof(command_t));
      other.m_command.destroy(&other.m_erasure);
      other.m_command.set_empty();
    }

    move_impl& operator=(move_impl&& other_raw) noexcept(Config::assertNoThrow) {
      // Get the real `self` and `other`.
      auto& self = static_cast<Self&>(*this);
      auto&& other = static_cast<Self&&>(other_raw);
      using command_t = typename Self::command_t;

      // Clear and move from `other` to `self`.
      self.clear();
      if (!other.is_empty() && this != std::addressof(other)) {
        other.m_command.move(&self.m_erasure, &other.m_erasure);
        std::memcpy(&self.m_command, &other.m_command, sizeof(command_t));
        other.m_command.destroy(&other.m_erasure);
        other.m_command.set_empty();
      }
      return *this;
    }
  };

  // Implement the copy constructor and copy assignment.
  template <typename Config, typename Self>
  struct copy_impl {
    EMBED_DETAIL_DTOR_ECTOR_DEFAULT(copy_impl);
    EMBED_DETAIL_MOVE_FUNCTION(copy_impl, default);

    copy_impl(const copy_impl& other_raw) noexcept(Config::assertNoThrow) {
      // Get the real `self` and `other`.
      auto& self = static_cast<Self&>(*this);
      auto& other = static_cast<const Self&>(other_raw);
      using erasure_t = typename Self::erasure_t;
      using command_t = typename Self::command_t;

      // Copy from `other` to `self`.
      other.m_command.clone(&self.m_erasure, const_cast<erasure_t*>(&other.m_erasure));
      std::memcpy(&self.m_command, &other.m_command, sizeof(command_t));
    }

    copy_impl& operator=(const copy_impl& other_raw) noexcept(Config::assertNoThrow) {
      auto& other = static_cast<const Self&>(other_raw);
      if (!other.is_empty() && this != std::addressof(other)) {
        Self(other).swap(static_cast<Self&>(*this));
      }
      return *this;
    }
  };

  template <bool IsView, bool IsCopyable, typename Config, typename Self>
  struct EMBED_DETAIL_FORCE_EBO lifetime_operations_impl; // Undefined

  // When `IsView` is true, the function should be trivially relocatable.
  template <bool IsCopyable, typename Config, typename Self>
  struct lifetime_operations_impl<
    /* IsView = */ true, IsCopyable, Config, Self
  > { EMBED_DETAIL_ALL_DEFAULT(lifetime_operations_impl) };

  // Implement clone constructor, move constructor, destructor, clone assignment,
  // and move assignment when `IsView` is false and `IsCopyable` is false.
  template <typename Config, typename Self>
  struct lifetime_operations_impl<
    /* IsView = */ false, /* IsCopyable = */ false, Config, Self
  >
    : public destructor_impl<Config, Self>,
      public move_impl<Config, Self>
  {
    EMBED_DETAIL_DTOR_ECTOR_DEFAULT(lifetime_operations_impl)
    EMBED_DETAIL_MOVE_FUNCTION(lifetime_operations_impl, default)
    EMBED_DETAIL_COPY_FUNCTION(lifetime_operations_impl, delete)
  };

  // Implement clone constructor, move constructor, destructor, clone assignment,
  // and move assignment when `IsView` is false and `IsCopyable` is true.
  template <typename Config, typename Self>
  struct lifetime_operations_impl<
    /* IsView = */ false, /* IsCopyable = */ true, Config, Self
  >
    : public destructor_impl<Config, Self>,
      public move_impl<Config, Self>,
      public copy_impl<Config, Self>
  { EMBED_DETAIL_ALL_DEFAULT(lifetime_operations_impl) };

  // Implement the 'operator*' for function.
  template <typename Signature, typename Self, bool IsView,
    typename ArgsPackage = typename unwrap_signature<Signature>::args>
  struct operator_dereference_impl;

  template <typename Signature, typename Self, bool IsView, typename... Args>
  struct operator_dereference_impl<Signature, Self, IsView, args_package<Args...>> {
  private:
    using function_ptr_t = typename unwrap_signature<Signature>::pure_sig*;

  public:
    // If the value stored in m_erasure is a pointer to a free function, 
    // return that pointer. Otherwise, return `nullptr`.
    /// @warning If the addresses of different functions may be the same 
    /// (which is not in accordance with the C++ standard), then this function
    /// has undefined behavior. For MSVC in release mode, `/OPT:NOICF` is needed.
    function_ptr_t operator*() const noexcept {
      using invoker_impl_t = typename Self::command_t::invoker_impl_t;
      using invoker_t = conditional_t<IsView, 
        typename invoker_impl_t::view, typename invoker_impl_t::inplace>;

      const auto& self = static_cast<const Self&>(*this);
      if (self.m_command.m_invoker == &invoker_t::template invoke<function_ptr_t>) {
        return self.m_erasure.template access<function_ptr_t>();
      }
      return nullptr;
    }
  };

  // Implement the member variables. Transplant the member variables
  // to the base class to achieve greater flexibility.
  /// @attention This class must be placed first in the inheritance list; otherwise, there
  /// will be an out-of-order error when it comes to move constructors and move assignments.
  template <std::size_t BufferSize, typename Config, typename Signature>
  struct member_variable_impl {
    EMBED_DETAIL_ALL_DEFAULT(member_variable_impl)

    // Zero initialize the `m_erasure` and `m_command`.
    constexpr member_variable_impl(std::nullptr_t) noexcept
    : m_erasure(erasure_t{}), m_command(command_t{}) {}
  protected:
    using erasure_t = erasure_type::Erasure<BufferSize>;

    using command_t = command::CommandTable<
      Config::isView, BufferSize, Config, Signature,
      typename unwrap_signature<Signature>::args>;

#if !(defined(__OPTIMIZE__) || defined(NDEBUG))
    static_assert(is_traditional_trivial<erasure_t>::value,
      EMBED_DETAIL_REPORT_IE("erasure_t should be trivial."));

    static_assert(is_traditional_trivial<command_t>::value,
      EMBED_DETAIL_REPORT_IE("command_t should be trivial."));
#endif

    erasure_t m_erasure;
    command_t m_command;
  };

  /// @brief Implement core components for function wrapper.
  template <bool IsView, std::size_t BufferSize, typename Config, typename Signature, typename Self>
  struct core_components_impl; // undefined

  // Implement the Ctor, assignment operator, swap(), is_empty()
  // operator bool, etc for normal function wrapper.
  template <std::size_t BufferSize, typename Config, typename Signature, typename Self>
  struct core_components_impl</*IsView=*/ false, BufferSize, Config, Signature, Self> {
    EMBED_DETAIL_ALL_DEFAULT(core_components_impl)

    // Construct an empty wrapper.
    core_components_impl(std::nullptr_t) noexcept {
      auto& self = static_cast<Self&>(*this);
      self.m_command.set_empty();
    }

    // Return `true` if the object is empty.
    EMBED_NODISCARD EMBED_CXX14_CONSTEXPR bool is_empty() const noexcept {
      auto const& self = static_cast<const Self&>(*this);
      return self.m_command.is_empty();
    }

    // Return `true` if the object is NOT empty.
    EMBED_CXX14_CONSTEXPR explicit operator bool() const noexcept { return !is_empty(); }

    // Clear the object.
    void clear() noexcept(Config::assertNoThrow) {
      auto& self = static_cast<Self&>(*this);
      self.m_command.destroy(&self.m_erasure);
      self.m_command.set_empty();
    }

    // Overload the function specifically for the case where nullptr is
    // passed as a parameter, in order to improve the program's running
    // efficiency. (Using the `swap` method would be much slower.)
    core_components_impl& operator=(std::nullptr_t) noexcept(Config::assertNoThrow) {
      if (!is_empty()) { clear(); }
      return *this;
    }

    // Swap the contents of two function objects. (Inplace mode)
    void swap(core_components_impl& fn_raw) noexcept(Config::assertNoThrow) {
      // Avoid self swap.
      if (this == std::addressof(fn_raw)) { return; }

      typename Self::erasure_t tmp_nil{}; // Empty temporary var

      auto& self = static_cast<Self&>(*this);
      auto& fn = static_cast<Self&>(fn_raw);

      // Move source from `m_erasure` to `tmp_nil`.
      self.m_command.move(&tmp_nil, &self.m_erasure);
      self.m_command.destroy(&self.m_erasure);

      // Move source from `fn.m_erasure` to `m_erasure`.
      fn.m_command.move(&self.m_erasure, &fn.m_erasure);
      fn.m_command.destroy(&fn.m_erasure);

      // Move source from `tmp_nil` to `fn.m_erasure`.
      self.m_command.move(&fn.m_erasure, &tmp_nil);
      self.m_command.destroy(&tmp_nil);

      std::swap(self.m_command, fn.m_command);
    }
  };

  // Implement the Ctor, assignment operator, swap(), is_empty()
  // operator bool, etc for function reference (function view).
  template <std::size_t BufferSize, typename Config, typename Signature, typename Self>
  struct core_components_impl</*IsView=*/ true, BufferSize, Config, Signature, Self> {
    EMBED_DETAIL_ALL_DEFAULT(core_components_impl)

    // Empty state of function view is removed.
    core_components_impl(std::nullptr_t)              = delete;
    EMBED_CXX14_CONSTEXPR bool is_empty()             = delete;
    EMBED_CXX14_CONSTEXPR explicit operator bool()    = delete;
    void clear()                                      = delete;
    core_components_impl& operator=(std::nullptr_t)   = delete;
    EMBED_DETAIL_TEMPLATE_BEGIN(typename T)
    EMBED_DETAIL_REQUIRES_END(
      (!fn_can_convert<Self, T>::value)
      && (!std::is_pointer<T>::value)
      && (!is_constant_wrapper<T>::value)
    ) core_components_impl& operator=(T)              = delete;

    // Swap the contents of two function objects. (View mode)
    void swap(core_components_impl& fn_raw) noexcept {
      auto& self = static_cast<Self&>(*this);
      auto& fn = static_cast<Self&>(fn_raw);
      std::swap(self.m_erasure, fn.m_erasure);
      std::swap(self.m_command, fn.m_command);
    }
  };

} // end namespace crtp_mixins

  /// @brief A lightweight and heap-free wrapper for callable objects.
  /// @tparam BufferSize - Specifies the size reserved to store the object.
  /// @tparam Config - Specifies the configuration attributes of the wrapper.
  ///           See @def config_package for details.
  /// @tparam Signature - The signature of the wrapper, e.g., @e `Ret(Args...)`.
  template <std::size_t BufferSize, typename Config, typename Signature>
  class EMBED_DETAIL_FORCE_EBO function final
    : public crtp_mixins::member_variable_impl<
        /* Buf = */ BufferSize, /* Cfg = */ Config, /* Sig = */ Signature
      >,
      public crtp_mixins::operator_call_impl<
        /* IsView = */ Config::isView, /* Signature = */ Signature, 
        /* Self = */ function<BufferSize, Config, Signature>
      >,
      public crtp_mixins::operator_dereference_impl<
        Signature, /* Self = */ function<BufferSize, Config, Signature>,
        /* IsView = */ Config::isView
      >,
      public crtp_mixins::lifetime_operations_impl<
        /* IsView = */ Config::isView, /* IsCopyable = */ Config::isCopyable,
        Config, /* Self = */ function<BufferSize, Config, Signature>
      >,
      public crtp_mixins::core_components_impl<
        /* IsView = */ Config::isView, /* BufferSize = */ BufferSize,
        /* Config = */ Config, /* Signature = */ Signature,
        /* Self = */ function<BufferSize, Config, Signature>
      >
  {
  private:

    template<std::size_t, typename, typename>
    friend class function;

    template <bool, typename, typename>
    friend struct crtp_mixins::operator_call_impl;

    template <typename, typename>
    friend struct crtp_mixins::copy_impl;

    template <typename, typename>
    friend struct crtp_mixins::move_impl;

    template <typename, typename>
    friend struct crtp_mixins::destructor_impl;

    template <typename, typename, bool, typename>
    friend struct crtp_mixins::operator_dereference_impl;

    template <bool, std::size_t, typename, typename, typename>
    friend struct crtp_mixins::core_components_impl;

    /// @brief ASSERT the given template arguments are valid.

    /// @tparam BufferSize
    static_assert(BufferSize >= sizeof(void*), 
      "The 'BufferSize' that you pass in is too small."
      " Try to use a BufferSize that is greater than or equal to sizeof(void*).");
    
    /// @tparam Config
    static_assert(is_config_package<Config>::value, 
      "The second argument must be 'config_package'."
      " Try to use config_package<...> as the second argument.");

    /// @tparam Signature
    static_assert(unwrap_signature<Signature>::isSignature, 
      "The 'Signature' argument of ebd::function must be a function type,"
      " such as void(), void(int) const or int(char*, float).");

    /// Check the "noexcept" is same.
    static_assert(!(Config::isThrowing && unwrap_signature<Signature>::isNoexcept),
      "This 'noexcept' qualifier is in conflict with the 'IsThrowing'"
      " configuration option. (Use 'ebd::safe_fn' or 'ebd::fn_ref')");

    using MemberVariableBase = crtp_mixins::member_variable_impl<
      BufferSize, Config, Signature>;

    using CoreComponents = crtp_mixins::core_components_impl<
      Config::isView, BufferSize, Config, Signature, function>;

    using erasure_t = typename MemberVariableBase::erasure_t;

    using command_t = typename MemberVariableBase::command_t;

    // The `m_erasure` contains the type-erased object.
    using MemberVariableBase::m_erasure;

    // The `m_command` is responsible for managing and invoking the `m_erasure`.
    using MemberVariableBase::m_command;

    // The buffer size.
    static constexpr std::size_t buffer_size = BufferSize;

    // `true` if self is copyable.
    static constexpr bool internal_is_copyable = Config::isCopyable || Config::isView;

  public:

    // The return type.
    using result_type = typename unwrap_signature<Signature>::ret;

    /// @brief Get the buffer size.
    EMBED_NODISCARD EMBED_INLINE static constexpr std::size_t
    get_buffer_size() noexcept { return buffer_size; }

    /// @brief Return `true` if the function is copyable.
    EMBED_NODISCARD EMBED_INLINE static constexpr bool
    is_copyable() noexcept { return internal_is_copyable; }

#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wuninitialized"
#endif
    /// @brief All following methods that end with `= default` are implemented in 
    /// the base class @e `crtp_mixins::lifetime_operations_impl`.

    // The destructor of the function wrapper, is trivial if `Config::isView == true`.
    ~function()                                   = default;
    // The copy constructor of the function wrapper, `=delete` if `(Config::isView || 
    // Config::isCopyable) == true`, and is trivial if `Config::isView == true`.
    function(const function& other)               = default;
    // The move constructor of the function wrapper, is trivial if 
    // `(Config::isView || Config::isCopyable) == true`.
    function(function&& other)                    = default;
    // The copy assignment of the function wrapper, `=delete` if `(Config::isView || 
    // Config::isCopyable) == true`, and is trivial if `Config::isView == true`.
    function& operator=(const function& other)    = default;
    // The move assignment of the function wrapper, is trivial if 
    // `(Config::isView || Config::isCopyable) == true`.
    function& operator=(function&& other)         = default;

#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

    /// @brief All following methods that begin with `CoreComponents` are implemented in 
    /// the base class @e `crtp_mixins::core_components_impl`.

    using CoreComponents::is_empty;

    using CoreComponents::operator bool;

    using CoreComponents::clear;

    using CoreComponents::swap;

    using CoreComponents::operator=;

    // Create an empty function wrapper.
    function() noexcept
#if __cpp_concepts >= 202002L
      requires requires { CoreComponents(nullptr); }
#endif
    : CoreComponents(nullptr) {}

    // Create an empty function wrapper.
    function(std::nullptr_t) noexcept
#if __cpp_concepts >= 202002L
      requires requires { CoreComponents(nullptr); }
#endif
    : CoreComponents(nullptr) {}

    // Use `placement new` to create new functor during construction. (Copy)
    // From `function<Buffer_small, ...>` to `function<Buffer_big, ...>`.
    EMBED_DETAIL_TEMPLATE_BEGIN(
      std::size_t OtherSize, typename OtherCfg, typename OtherSig)
    EMBED_DETAIL_REQUIRES_END(
      fn_can_convert<function, function<OtherSize, OtherCfg, OtherSig>>::value
      && function<OtherSize, OtherCfg, OtherSig>::internal_is_copyable
    ) function(const function<OtherSize, OtherCfg, OtherSig>& other)
    noexcept(is_cfg_noexcept<Config>::value && is_cfg_noexcept<OtherCfg>::value) {
      using other_fn_t = function<OtherSize, OtherCfg, OtherSig>;
      using other_erasure_t = typename other_fn_t::erasure_t;

      // Suppress GCC warning: "-Wmaybe-uninitialized".
      std::memset(&m_erasure, 0, sizeof(void*));

      other.m_command.clone(
        &m_erasure, const_cast<other_erasure_t*>(&other.m_erasure));
      std::memcpy(&m_command, &other.m_command, sizeof(command_t));
    }

    // Use `placement new` to create new functor during construction. (Move)
    // From `function<Buffer_small, ...>` to `function<Buffer_big, ...>`.
    EMBED_DETAIL_TEMPLATE_BEGIN(
      std::size_t OtherSize, typename OtherCfg, typename OtherSig)
    EMBED_DETAIL_REQUIRES_END(
      fn_can_convert<function, function<OtherSize, OtherCfg, OtherSig>>::value
      && (!Config::isView)
    ) function(function<OtherSize, OtherCfg, OtherSig>&& other)
    noexcept(is_cfg_noexcept<Config>::value && is_cfg_noexcept<OtherCfg>::value) {
      // Suppress GCC warning: "-Wmaybe-uninitialized".
      std::memset(&m_erasure, 0, sizeof(void*));

      other.m_command.move(&m_erasure, &other.m_erasure);
      std::memcpy(&m_command, &other.m_command, sizeof(command_t));
      other.m_command.destroy(&other.m_erasure);
      other.m_command.set_empty();
    }

    /// @brief Builds a Fn that targets a copy/move of the incoming function object.
    /// @param functor - A callable object with parameters of type `Args...`
    /// and returns a value convertible to `Ret`. (The Signature is `Ret(Args...)`)
    /// @note Used for function wrapper only. (OWNING)
    EMBED_DETAIL_TEMPLATE_BEGIN(typename Functor)
    EMBED_DETAIL_REQUIRES_END(
      (!fn_can_convert<function, Functor>::value)
      && (!is_self<Functor, function>::value)
      && (!is_in_place_type<decay_t<Functor>>::value)
      && is_callable_from<Functor, Signature>::value
      && (!Config::isView)
    ) function(Functor&& functor)
    noexcept(is_nothrow_construct_from_functor<Functor&&>::value) {

      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Functor, Functor&&, erasure_t>::value,
        EMBED_DETAIL_REPORT_IE("asserts_for_function<...>::value should be always true."));

      if (check_not_empty::not_empty(functor)) {
        m_command.template init<>(&m_erasure, std::forward<Functor>(functor));
      } else {
        m_command.set_empty();
      }
    }

    /// @brief Builds a function reference from function pointer.
    /// @param function_ptr - A function pointer that is NOT a null pointer.
    /// @note Used for function reference only. (NON-OWNING)
    EMBED_DETAIL_TEMPLATE_BEGIN(typename Func)
    EMBED_DETAIL_REQUIRES_END(
      std::is_function<Func>::value
      && is_invocable_using_t<Signature, Func>::value
      && Config::isView
    ) function(Func* function_ptr) noexcept {

      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Func*, Func*&&, erasure_t>::value,
        EMBED_DETAIL_REPORT_IE("asserts_for_function<...>::value should be always true."));

      EMBED_DETAIL_ASSERT_MESSAGE(function_ptr != nullptr, 
        "[Embedded Function]: The function pointer should not be a nullptr.");

      m_command.template init</* IsStoredOrigin = */ true>(
        &m_erasure, std::forward<Func*>(function_ptr));
    }

    /// @brief Builds a function reference from given functor.
    /// @param functor - A callable object with parameters of type `Args...`
    /// and returns a value convertible to `Ret`. (The Signature is `Ret(Args...)`)
    /// @note Used for function reference only. (NON-OWNING)
    EMBED_DETAIL_TEMPLATE_BEGIN(typename Functor, 
      typename Tp = remove_reference_t<Functor>,
      typename Tp_cv = typename unwrap_signature<Signature>::template add_cv_like<Tp>)
    EMBED_DETAIL_REQUIRES_END(
      (!is_self<Functor, function>::value)
      && is_invocable_using_t<Signature, Tp_cv&>::value
      && (!std::is_member_pointer<Tp>::value)
      && (!fn_can_convert<function, Functor>::value)
      && Config::isView
    ) EMBED_CXX20_CONSTEXPR function(Functor&& functor) noexcept
    : MemberVariableBase(nullptr) {

      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Functor, Functor&&, erasure_t>::value,
        EMBED_DETAIL_REPORT_IE("asserts_for_function<...>::value should be always true."));

      m_command.template init</* IsStoredOrigin = */ false>(
        &m_erasure, std::forward<Functor>(functor));
    }

#if EMBED_CXX_VERSION >= 201703L

    /// @brief In-place constructs the Fn within the internal storage with specified arguments.
    /// @param args - The arguments for constructing the Fn.
    EMBED_DETAIL_TEMPLATE_BEGIN(typename Fn, typename... CArgs)
    EMBED_DETAIL_REQUIRES_END(
      std::is_constructible<Fn, CArgs...>::value
      && is_callable_from<Fn, Signature>::value
      && (!Config::isView)
    ) explicit function(std::in_place_type_t<Fn>, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<Fn, CArgs...>::value) {

      static_assert(std::is_same<Fn, decay_t<Fn>>::value,
        "decay_t<Fn> should be the same type as Fn.");
      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Fn, Fn, erasure_t>::value,
        EMBED_DETAIL_REPORT_IE("asserts_for_function<...>::value should be always true."));

      m_command.template emplace_init<Fn>(&m_erasure, std::forward<CArgs>(args)...);
    }

    /// @brief In-place constructs the Fn within the internal storage with init_list and specified arguments.
    /// @param il - The initializer_list for constructing the Fn.
    /// @param args - The arguments for constructing the Fn.
    EMBED_DETAIL_TEMPLATE_BEGIN(typename Fn, typename U, typename... CArgs)
    EMBED_DETAIL_REQUIRES_END(
      std::is_constructible<Fn, std::initializer_list<U>&, CArgs...>::value
      && is_callable_from<Fn, Signature>::value
      && (!Config::isView)
    ) explicit function(std::in_place_type_t<Fn>, std::initializer_list<U> il, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<Fn, CArgs...>::value) {

      static_assert(std::is_same<Fn, decay_t<Fn>>::value,
        "decay_t<Fn> should be the same type as Fn.");
      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Fn, Fn, erasure_t>::value,
        EMBED_DETAIL_REPORT_IE("asserts_for_function<...>::value should be always true."));

      m_command.template emplace_init<Fn>(&m_erasure, il, std::forward<CArgs>(args)...);
    }

#endif

#if __cpp_lib_constant_wrapper >= 202603L

    /// @todo experimental

    // Create function reference with given `std::constant_wrapper` param.
    template <auto CwVal, typename Fn>
      requires is_invocable_using_t<Signature, const Fn&>::value
        && Config::isView
    constexpr function(std::constant_wrapper<CwVal, Fn>) noexcept
    : MemberVariableBase(nullptr) {
      using Cw = std::constant_wrapper<CwVal, Fn>;
      m_command.template cw_init<Cw>();

      // Mandates are as follows.
      if constexpr (std::is_pointer_v<Fn> || std::is_member_pointer_v<Fn>) {
        static_assert(Cw::value != nullptr, "Cannot create fn_ref from null constant_wrapper");
      }
    }

    // Create function reference with given `std::constant_wrapper` and object params.
    template <auto CwVal, typename Fn, typename Up, typename Tp = remove_reference_t<Up>>
      requires (!std::is_rvalue_reference_v<Up&&>)
        && is_invocable_using_t<Signature, const Fn&, 
          typename unwrap_signature<Signature>::template add_cv_like<Tp>&>::value
        && Config::isView
    constexpr function(std::constant_wrapper<CwVal, Fn>, Up&& obj) noexcept
    : MemberVariableBase(nullptr) {
      using Cw = std::constant_wrapper<CwVal, Fn>;
      m_command.template cw_init<Cw, /*CallPointer*/false>(&m_erasure, std::addressof(obj));

      // Mandates are as follows.
      if constexpr (std::is_pointer_v<Fn> || std::is_member_pointer_v<Fn>) {
        static_assert(Cw::value != nullptr, "Cannot create fn_ref from null constant_wrapper");
      }
    }

    // Create function reference with given `std::constant_wrapper` and pointer params.
    template <auto CwVal, typename Fn, typename Tp,
      typename Tp_cv = typename unwrap_signature<Signature>::template add_cv_like<Tp>
    >
      requires std::is_convertible_v<Tp*, Tp_cv*>
        && is_invocable_using_t<Signature, const Fn&, Tp_cv*>::value
        && Config::isView
    constexpr function(std::constant_wrapper<CwVal, Fn>, Tp* obj) noexcept
    : MemberVariableBase(nullptr) {
      using Cw = std::constant_wrapper<CwVal, Fn>;
      m_command.template cw_init<Cw, /*CallPointer*/true>(&m_erasure, obj);

      // Mandates are as follows.
      if constexpr (std::is_pointer_v<Fn> || std::is_member_pointer_v<Fn>) {
        static_assert(Cw::value != nullptr, "Cannot create fn_ref from null constant_wrapper");
      }
      if constexpr (std::is_member_pointer_v<Fn>) {
        EMBED_DETAIL_ASSERT_MESSAGE(obj != nullptr, 
          "[Embedded Function]: The object pointer shouldn't be nullptr.");
      }
    }

#endif

  };

  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig,
    EMBED_DETAIL_REQUIRES(Cfg::isView == false)
  > EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator==(const function<Buf, Cfg, Sig>& fn, std::nullptr_t) noexcept {
    return fn.is_empty();
  }

  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig,
    EMBED_DETAIL_REQUIRES(Cfg::isView == false)
  > EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator==(std::nullptr_t, const function<Buf, Cfg, Sig>& fn) noexcept {
    return fn.is_empty();
  }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig,
    EMBED_DETAIL_REQUIRES(Cfg::isView == false)
  > EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator!=(const function<Buf, Cfg, Sig>& fn, std::nullptr_t) noexcept {
    return !fn.is_empty();
  }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig,
    EMBED_DETAIL_REQUIRES(Cfg::isView == false)
  > EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator!=(std::nullptr_t, const function<Buf, Cfg, Sig>& fn) noexcept {
    return !fn.is_empty();
  }

  // Make a function.
  template <typename Fn, bool NoThrow, typename... CArgs>
  EMBED_CXX20_CONSTEXPR inline
  Fn make_function_impl(CArgs&&... args) noexcept(NoThrow) {
    static_assert(is_ebd_fn<Fn>::value,
      "Fn must be the alias of `ebd::detail::function`.");
    return Fn{std::forward<CArgs>(args)...};
  }

} // end namespace detail


/**
 * @brief A basic function wrapper that users can customize.
 * 
 * This alias provides the most flexible way to instantiate a function wrapper
 * by directly specifying all configuration parameters. It is intended for
 * advanced use cases where none of the predefined aliases (`fn`, `unique_fn`,
 * `safe_fn`, `fn_ref`) satisfy the required combination of copyability,
 * view semantics, exception behavior, and no‑throw assertions.
 * 
 * @tparam Signature              Function signature, e.g., `void(int, char)`, 
 *                                `int(int, float) const`, `void() &&`, etc.
 * 
 * @tparam BufferSize             Size of the internal storage (in bytes).
 *                                The value will NOT be automatically aligned.
 * 
 * @tparam IsCopyable             If `true`, the stored callable object must be
 *                                copy‑constructible; otherwise, move‑only is
 *                                sufficient (copyable is still accepted but
 *                                only move operations will be used).
 * 
 * @tparam IsView                 If `true`, the wrapper acts as a non‑owning
 *                                view (no copy/move/destroy of the target).
 *                                The stored object is either stored directly
 *                                (if is function pointer) or by pointer.
 *                                *Empty state has been removed in view mode.*
 * 
 * @tparam IsThrowing             If `true`, calling an empty wrapper throws
 *                                `std::bad_function_call` (if exceptions are
 *                                enabled); otherwise, `std::terminate` is 
 *                                called. When @arg `IsView` is `true`, this 
 *                                config argument will be ignored cause there 
 *                                is no empty state in view mode.
 * 
 * @tparam AssertObjectNoThrow    If `true`, the wrapper requires that the
 *                                callable object's construction, destruction,
 *                                copy, and move operations are `noexcept`.
 *                                Violations trigger a `static_assert`.
 * 
 * @note                          Prefer using the predefined aliases (`fn`, 
 *                                `unique_fn`, `safe_fn`, `fn_ref`) unless you
 *                                need a combination not covered by them.
 * 
 * @example                       A move-only, non‑throwing wrapper:
 * ```cpp
 * template <typename Signature, std::size_t BufferSize>
 * using unique_safe_fn = ebd::basic_fn<Signature, BufferSize,
 *                                      false, // IsCopyable
 *                                      false, // IsView
 *                                      false, // IsThrowing
 *                                      true>; // AssertObjectNoThrow
 * ```
 */
template <
  typename Signature,
  std::size_t BufferSize,
  bool IsCopyable,
  bool IsView,
  bool IsThrowing,
  bool AssertObjectNoThrow
>
using basic_fn = detail::function<
  /* BufferSize = */  BufferSize,
  /* Config = */      detail::config_package<
    /* IsCopyable = */          IsCopyable,
    /* IsView = */              IsView,
    /* IsThrowing = */          IsThrowing,
    /* AssertObjectNoThrow = */ AssertObjectNoThrow>,
  /* Signature = */   Signature
>;

/// @brief A function object wrapper for copyable and callable objects.
/// @tparam Signature - Function signature. Seems like `Ret(Args...)`.
/// @tparam BufferSize - Buffer size. Used for storing the callable object.
/// And the buffer size will be aligned automatically.
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
using fn = basic_fn<
  /* Signature = */           Signature,
  /* BufferSize = */          detail::get_aligned_size(BufferSize),
  /* IsCopyable = */          true,
  /* IsView = */              false,
  /* IsThrowing = */          true,
  /* AssertObjectNoThrow = */ false
>;

/// @brief A function object wrapper for movable and callable objects.
/// @tparam Signature - Function signature. Seems like `Ret(Args...)`.
/// @tparam BufferSize - Buffer size. Used for storing the callable object.
/// And the buffer size will be aligned automatically.
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
using unique_fn = basic_fn<
  /* Signature = */           Signature,
  /* BufferSize = */          detail::get_aligned_size(BufferSize),
  /* IsCopyable = */          false,
  /* IsView = */              false,
  /* IsThrowing = */          true,
  /* AssertObjectNoThrow = */ false
>;

/// @brief A SAFE function object wrapper for copyable and callable objects.
/// @throws Strong noexcept guarantee. (ASSERT-NO-THROW)
/// @tparam Signature - Function signature. Seems like `Ret(Args...)`.
/// @tparam BufferSize - Buffer size. Used for storing the callable object.
/// And the buffer size will be aligned automatically.
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
using safe_fn = basic_fn<
  /* Signature = */           Signature,
  /* BufferSize = */          detail::get_aligned_size(BufferSize),
  /* IsCopyable = */          true,
  /* IsView = */              false,
  /* IsThrowing = */          false,
  /* AssertObjectNoThrow = */ true
>;

/// @brief A non-owning polymorphic function wrapper.
/// @tparam Signature - Function signature. Seems like `Ret(Args...)`.
/// @tparam Unused - Unused.
template <typename Signature, std::size_t Unused = 0 /* Unused */>
using fn_ref = basic_fn<
  /* Signature = */           Signature,
  /* BufferSize = */          detail::default_buffer_size::ref_buf,
  /* IsCopyable = */          true,
  /* IsView = */              true,
  /* IsThrowing = */          false,
  /* AssertObjectNoThrow = */ false
>;

/// @deprecated Use `fn_ref` instead.
template <typename Signature, std::size_t Unused = 0 /* Unused */>
using fn_view EMBED_DEPRECATED("Use fn_ref instead") = fn_ref<Signature>;


/// @brief make_fn[0]: Make function with specified signature for copyable functor.
/// @return `fn<Signature, sizeof(Functor)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Signature, // [User specify] function signature.
  typename Functor,   // [Auto] Functor type.
  typename Class = detail::remove_cvref_t<Functor>,
  // [Auto] The buffer size of functor.
  std::size_t BufferSize = sizeof(Class),
  // [Auto] The function type. (fn or unique_fn)
  typename Fn = detail::conditional_t<
    std::is_copy_constructible<Class>::value, 
    fn<Signature, BufferSize>, unique_fn<Signature, BufferSize>
  >,
  // [Auto] Get the nothrow guarantee in construction of functor.
  bool NoThrow = detail::is_nothrow_construct_from_functor<Functor&&>::value
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] Functor must be copyable.
  std::is_copy_constructible<Functor>::value
  // [Require] First template argument must be signature.
  && detail::unwrap_signature<Signature>::isSignature
  // [Require] Functor cannot be the function pointer or pointer to member function.
  && std::is_class<Class>::value
)
EMBED_NODISCARD inline Fn make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    Fn, /* NoThrow = */ NoThrow
  >(std::forward<Functor>(functor));
}

/// @brief make_fn[1]: Make function with specified signature for move-only functor.
/// @return `unique_fn<Signature, sizeof(Functor)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Signature, // [User specify] function signature.
  typename Functor,   // [Auto] Functor type.
  // [Auto] Get the nothrow guarantee of functor.
  bool NoThrow = std::is_nothrow_move_constructible<Functor>::value
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] Functor must be movable.
  std::is_move_constructible<Functor>::value
  // [Require] Functor must be non-copyable.
  && (!std::is_copy_constructible<Functor>::value)
  // [Require] First template argument must be signature.
  && detail::unwrap_signature<Signature>::isSignature
)
EMBED_NODISCARD inline unique_fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    unique_fn<Signature, sizeof(Functor)>, NoThrow
  >(std::forward<Functor>(functor));
}

/// @brief make_fn[2]: Make an empty function with specified signature and buffer size.
/// @return `fn<Signature, BufferSize>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Signature, // [User specify] function signature.
  std::size_t BufferSize = detail::default_buffer_size::value
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] First template argument must be signature.
  detail::unwrap_signature<Signature>::isSignature
)
EMBED_NODISCARD inline fn<Signature, BufferSize>
make_fn(std::nullptr_t = nullptr) noexcept {
  return detail::make_function_impl<
    fn<Signature, BufferSize>, /* NoThrow = */ true
  >(nullptr);
}

/// @brief make_fn[3]: Make function for function pointer. (auto deduce signature and buffer size)
/// @return `fn<Ret(Args...) const, sizeof(Ret(*)(Args...))>`
template <typename Ret, typename... Args>
EMBED_NODISCARD inline fn<Ret(Args...) const, sizeof(Ret(*)(Args...))>
make_fn(Ret (*func_ptr) (Args...)) noexcept {
  return detail::make_function_impl<
    fn<Ret(Args...) const, sizeof(Ret(*)(Args...))>,
    /* NoThrow = */ true
  >(func_ptr);
}

/// @brief make_fn[4]: Make function for function pointer with specified signature.
/// @return `fn<Signature, sizeof(FunctionPtr)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Signature, // [User specify] function signature.
  // [Auto] The type of the function pointer.
  typename FunctionPtr = typename detail::unwrap_signature<Signature>::pure_sig*
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] First template argument must be signature.
  detail::unwrap_signature<Signature>::isSignature
  // [Require] The `FunctionPtr` must be the function pointer.
  && detail::is_function_ptr<FunctionPtr>::value
)
EMBED_NODISCARD inline fn<Signature, sizeof(FunctionPtr)>
make_fn(FunctionPtr func_ptr) noexcept {
  return detail::make_function_impl<
    fn<Signature, sizeof(FunctionPtr)>,
    /* NoThrow = */ true
  >(func_ptr);
}

/// @brief make_fn[5]: Create a function from another function. (Copy)
/// @return `detail::function<Buf, Cfg, Sig>`
template <std::size_t Buf, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Cfg, Sig>
make_fn(const detail::function<Buf, Cfg, Sig>& fn)
noexcept(Cfg::isView || Cfg::assertNoThrow) {
  return detail::make_function_impl<
    detail::function<Buf, Cfg, Sig>,
    /* NoThrow = */ Cfg::isView || Cfg::assertNoThrow
  >(fn);
}

/// @brief make_fn[6]: Create a function from another function. (Move)
/// @return `detail::function<Buf, Cfg, Sig>`
template <std::size_t Buf, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Cfg, Sig>
make_fn(detail::function<Buf, Cfg, Sig>&& fn)
noexcept(Cfg::isView || Cfg::assertNoThrow) {
  using Fn = detail::function<Buf, Cfg, Sig>;
  return detail::make_function_impl<
    detail::function<Buf, Cfg, Sig>,
    /* NoThrow = */ Cfg::isView || Cfg::assertNoThrow
  >(std::forward<Fn>(fn));
}

/// @brief make_fn[7]: Make a function from lambda or unique-operator() functor.
/// @note Auto deduce signature and buffer size.
/// @return `fn<Signature, BufferSize>` or `unique_fn<Signature, BufferSize>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Lambda, // [Auto] The lambda or functor that overloads operator() only once.
  // [Auto] The basic type of the functor.
  typename Class = detail::remove_cvref_t<Lambda>,
  // [Auto] The buffer size of functor.
  std::size_t BufferSize = sizeof(Class),
  // [Auto] The signature of functor.
  typename Signature = detail::get_unique_signature_t<Class>,
  // [Auto] The function type. (fn or unique_fn)
  typename Fn = detail::conditional_t<
    std::is_copy_constructible<Class>::value, 
    fn<Signature, BufferSize>, unique_fn<Signature, BufferSize>
  >,
  // [Auto] Get the nothrow guarantee in construction of functor.
  bool NoThrow = detail::is_nothrow_construct_from_functor<Lambda&&>::value
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] The functor must be unique callable.
  detail::is_unique_callable<Class>::value
  // [Require] The signature must be valid.
  && detail::unwrap_signature<Signature>::isSignature
)
EMBED_NODISCARD inline Fn make_fn(Lambda&& fn) noexcept(NoThrow) {
  return detail::make_function_impl<Fn, NoThrow>(std::forward<Lambda>(fn));
}

#define EMBED_DETAIL_MAKE_FN_DEFINE(C, V, REF, NOEXCEPT)                        \
  template <typename Class, typename Ret, typename... Args>                     \
  EMBED_NODISCARD inline auto                                                   \
  make_fn(Ret(Class::* memfunc)(Args...) C V REF NOEXCEPT) noexcept             \
  -> fn<                                                                        \
    Ret(detail::get_qualified_with_t<int REF, C V Class>, Args...) const,       \
    sizeof(memfunc)                                                             \
  > {                                                                           \
    using qualified_class_t = detail::get_qualified_with_t<int REF, C V Class>; \
    using signature_t = Ret (qualified_class_t, Args...) const;                 \
    using fn_t = fn<signature_t, sizeof(memfunc)>;                              \
    return detail::make_function_impl<fn_t, /* NoThrow = */ true>(memfunc);     \
  }

/// @brief make_fn[8]: Make function for pointer to member function. 
/// (auto deduce signature and buffer size)
/// @return `fn<Ret(Class, Args...) const, sizeof(Ret(Class::*)(Args...))>`
EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_MAKE_FN_DEFINE)

#undef EMBED_DETAIL_MAKE_FN_DEFINE

/// @brief make_fn[9]: Make function for member function pointer with specified signature.
/// @return `fn<Signature, sizeof(MemFuncPtr)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Signature, // [User specify] function signature.
  // [Auto] Deduce the type of member function pointer.
  typename MemFuncPtr = detail::get_member_fn_type_t<Signature>,
  // [Auto] Deduce the size of member function pointer.
  std::size_t BufferSize = sizeof(MemFuncPtr)
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] Signature must be valid.
  detail::unwrap_signature<Signature>::isSignature
  // [Require] Member function pointer must be valid.
  && std::is_member_function_pointer<MemFuncPtr>::value
)
EMBED_NODISCARD inline fn<Signature, BufferSize>
make_fn(MemFuncPtr memfunc_ptr) noexcept {
  return detail::make_function_impl<
    fn<Signature, BufferSize>,
    /* NoThrow = */ true
  >(memfunc_ptr);
}

/// @brief make_fn[10]: Make function for pointer to member object.
/// @return `fn<T(Class&) const, sizeof(ptr_memobj)>` 
template <typename Class, typename T,
  typename Ret = typename detail::invoke_result<T Class::*, Class&>::type>
EMBED_NODISCARD inline auto make_fn(T Class::* ptr_memobj) noexcept
-> fn<Ret(Class&) const, sizeof(ptr_memobj)> {
  return detail::make_function_impl<
    fn<Ret(Class&) const, sizeof(ptr_memobj)>,
    /* NoThrow = */ true
  >(ptr_memobj);
}

#if EMBED_CXX_VERSION >= 201703L

/// @brief make_fn[11]: In-place make function.
/// @return `decltype(make_fn(std::declval<Functor>()))`
template <typename Functor, typename... CArgs>
EMBED_NODISCARD inline auto make_fn(std::in_place_type_t<Functor>, CArgs&&... args)
noexcept(std::is_nothrow_constructible<Functor, CArgs...>::value) {
  using signature = typename detail::is_ebd_fn<
    decltype(make_fn(std::declval<Functor>()))>::signature;

  using Fn = detail::conditional_t<
    std::is_copy_constructible<Functor>::value,
    ebd::fn<signature, sizeof(Functor)>, ebd::unique_fn<signature, sizeof(Functor)>>;

  return detail::make_function_impl<
    Fn, std::is_nothrow_constructible<Functor, CArgs...>::value
  >(std::in_place_type<Functor>, std::forward<CArgs>(args)...);
}

/// @brief make_fn[11]: In-place make function. (std::initializer_list)
/// @return `decltype(make_fn(std::declval<Functor>()))`
template <typename Functor, typename U, typename... CArgs>
EMBED_NODISCARD inline auto
make_fn(std::in_place_type_t<Functor>, std::initializer_list<U> il, CArgs&&... args)
noexcept(std::is_nothrow_constructible<Functor, std::initializer_list<U>&, CArgs...>::value) {
  using signature = typename detail::is_ebd_fn<
    decltype(make_fn(std::declval<Functor>()))>::signature;

  using Fn = detail::conditional_t<
    std::is_copy_constructible<Functor>::value,
    ebd::fn<signature, sizeof(Functor)>, ebd::unique_fn<signature, sizeof(Functor)>>;

  return detail::make_function_impl<
    Fn, std::is_nothrow_constructible<Functor, std::initializer_list<U>&, CArgs...>::value
  >(std::in_place_type<Functor>, il, std::forward<CArgs>(args)...);
}

#endif

/// @brief make_fn[12]: Make function with specified wrapper.
/// @tparam Fn - Can be `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn`, or `ebd::fn_ref`.
/// @return `Fn<Signature, sizeof(functor)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  template <class, std::size_t> class Fn,
  typename SpecifiedSig = void,
  typename Functor,
  typename Deduction = decltype(make_fn(std::declval<Functor>())),
  typename RawSig = typename detail::is_ebd_fn<Deduction>::signature,
  typename Signature = detail::conditional_t<
    std::is_void<SpecifiedSig>::value,
    detail::noexcept_qualify_like_t<Functor, Fn, RawSig>, SpecifiedSig
  >,
  std::size_t BufferSize = sizeof(detail::decay_t<Functor>),
  typename FnWrapper = Fn<Signature, BufferSize>,
  bool NoThrow = noexcept(FnWrapper(std::declval<Functor>()))
)
EMBED_DETAIL_REQUIRES_END(
  detail::is_ebd_fn<FnWrapper>::value
  && detail::unwrap_signature<Signature>::isSignature
)
EMBED_NODISCARD EMBED_CXX20_CONSTEXPR inline
FnWrapper make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    /* Fn = */ FnWrapper, /* NoThrow = */ NoThrow
  >(std::forward<Functor>(functor));
}

// When all other make_fn() fail to match the input parameters, 
// this function will be called as the fall back to avoid the 
// awful template error flood.
template <typename Unused = void,
  EMBED_DETAIL_REQUIRES(!detail::unwrap_signature<Unused>::isSignature)>
constexpr int make_fn(...) noexcept(detail::make_fn_log_error<Unused>()) { return 0; }
template <template <class, std::size_t> class Unused>
constexpr int make_fn(...) noexcept(detail::make_fn_log_error<Unused<void(), 0>>()) { return 0; }

#if __cpp_lib_constant_wrapper >= 202603L
namespace detail {

  /// @brief `fn_ref` CTAD guides from `std::constant_wrapper`.

  template <typename Fn>
  using make_fn_ref_deduction_sig_t = noexcept_qualify_like_t<
    Fn, fn_ref, typename is_ebd_fn<decltype(make_fn(std::declval<Fn>()))>::signature>;

  template <auto Cw, typename Fn>
  function(std::constant_wrapper<Cw, Fn>) -> function<
    default_buffer_size::ref_buf,
    config_package<true, true, false, false>, // fn_ref
    make_fn_ref_deduction_sig_t<Fn>
  >;

  template <auto Cw, typename Fn, typename Tp>
  function(std::constant_wrapper<Cw, Fn>, Tp&&) -> function<
    default_buffer_size::ref_buf,
    config_package<true, true, false, false>, // fn_ref
    skip_first_arg_sig_t<make_fn_ref_deduction_sig_t<Fn>>
  >;

} // end namespace detail
#endif // ^^^ __cpp_lib_constant_wrapper >= 202603L

} // end namespace ebd

#undef EMBED_DETAIL_FN_EXPAND
#undef EMBED_DETAIL_FN_EXPAND_IMPL
#undef EMBED_DETAIL_REQUIRES
#undef EMBED_DETAIL_FORCE_EBO
#undef EMBED_DETAIL_VIRTUAL_INHERITANCE
#undef EMBED_DETAIL_MOVE_FUNCTION
#undef EMBED_DETAIL_COPY_FUNCTION
#undef EMBED_DETAIL_DTOR_ECTOR_DEFAULT
#undef EMBED_DETAIL_ALL_DEFAULT
#undef EMBED_DETAIL_TEMPLATE_BEGIN
#undef EMBED_DETAIL_REQUIRES_END
#undef EMBED_DETAIL_TEXT
#undef EMBED_DETAIL_TEXT_IMPL
#undef EMBED_DETAIL_LAUNDER
#undef EMBED_DETAIL_ALIAS
#undef EMBED_DETAIL_FAIL_MESSAGE
#undef EMBED_DETAIL_UNREACHABLE
#undef EMBED_DETAIL_ASSERT_MESSAGE
#undef EMBED_DETAIL_REPORT_IE
#if defined(EMBED_FN_CONFIG_UNDEF_MACROS)
// #undef most of the EMBED_* macros if EMBED_FN_CONFIG_UNDEF_MACROS is defined.
// EMBED_CXX_VERSION and EMBED_CXX_ENABLE_EXCEPTION are reserved.
# undef EMBED_HAS_BUILTIN
# undef EMBED_HAS_ATTRIBUTE
# undef EMBED_HAS_CXX_ATTRIBUTE
# undef EMBED_ABI_VISIBILITY
# undef EMBED_CXX14_CONSTEXPR
# undef EMBED_CXX20_CONSTEXPR
# undef EMBED_INLINE
# undef EMBED_RESTRICT
# undef EMBED_NODISCARD
# undef EMBED_FALLTHROUGH
# undef EMBED_DEPRECATED

# undef EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER
# undef EMBED_FN_CONFIG_DISABLE_SMART_FORWARD
# undef EMBED_FN_CONFIG_UNDEF_MACROS
# undef EMBED_FN_HOOK_DEBUG
#endif

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // EMBED_INCLUDED_EMBED_FUNCTION_HPP_

