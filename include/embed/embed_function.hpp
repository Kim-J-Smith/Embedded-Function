/**
 * @file        embed_function.hpp
 * 
 * @date        2026-2-7
 * 
 * @version     2.0.12
 * 
 * @copyright   Copyright (c) 2026 Kim-J-Smith
 *              All rights reserved.
 *              (https://github.com/Kim-J-Smith/Embedded-Function)
 * 
 * @attention   This source is released under the MIT license
 *              (http://opensource.org/licenses/MIT)
 */

// Just like function pointers, it is quick and efficient.

/// @b EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER
/// If this macro is defined, bigger default buffer size will be used.

/// @b EMBED_FN_CONFIG_DISABLE_SMART_FORWARD
/// If this macro is defined, `smart_forward_t` will fall back to Perfect Forwarding.

/// @b EMBED_FN_CONFIG_UNDEF_MACROS
/// If this macro is defined, EMBED_* macros will be undefined at the end of this file.

/// @b EMBED_FN_HOOK_TRACE_EMPTY_CALL(message)
/// If this macro is defined, it will be called in function `throw_or_terminate()` in debug mode.

#ifndef EMBED_INCLUDED_EMBED_FUNCTION_HPP_
#define EMBED_INCLUDED_EMBED_FUNCTION_HPP_

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable: 4514 4668 4710 26495)
#endif

#ifndef EMBED_CXX_VERSION
# if defined(_MSC_VER) && ( _MSC_VER >= 1900 )
#  define EMBED_CXX_VERSION _MSVC_LANG
# else
#  define EMBED_CXX_VERSION __cplusplus
# endif
#endif

#ifndef EMBED_ALIAS
# if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#  define EMBED_ALIAS __attribute__((may_alias))
# else
#  define EMBED_ALIAS
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
# if ( EMBED_CXX_VERSION >= 201402L ) && ( __cpp_constexpr >= 201304L )
#  define EMBED_CXX14_CONSTEXPR constexpr
# else
#  define EMBED_CXX14_CONSTEXPR
# endif
#endif

#ifndef EMBED_INLINE
# if defined(__GNUC__) || defined(__clang__) || defined(__TASKING__)
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
# if ( EMBED_CXX_VERSION >= 201703L ) && EMBED_HAS_CXX_ATTRIBUTE(nodiscard)
#  define EMBED_NODISCARD [[nodiscard]]
# elif defined(__GNUC__) || defined(__clang__)
#  define EMBED_NODISCARD __attribute__((warn_unused_result))
# else
#  define EMBED_NODISCARD
# endif
#endif

#ifndef EMBED_FALLTHROUGH
# if ( EMBED_CXX_VERSION >= 201703L ) && EMBED_HAS_CXX_ATTRIBUTE(fallthrough)
#  define EMBED_FALLTHROUGH() [[fallthrough]]
# elif EMBED_HAS_CXX_ATTRIBUTE(gnu::fallthrough)
#  define EMBED_FALLTHROUGH() [[gnu::fallthrough]]
# elif EMBED_HAS_CXX_ATTRIBUTE(clang::fallthrough)
#  define EMBED_FALLTHROUGH() [[clang::fallthrough]]
# elif (defined(__GNUC__) || defined(__clang__)) && EMBED_HAS_ATTRIBUTE(fallthrough)
#  define EMBED_FALLTHROUGH() __attribute__((fallthrough))
# else
#  define EMBED_FALLTHROUGH() (static_cast<void>(0))
# endif
#endif

# ifndef EMBED_LAUNDER
#  if ( EMBED_CXX_VERSION >= 201703L )
#   define EMBED_LAUNDER(x) ( ::std::launder(x) )
#  elif EMBED_HAS_BUILTIN(__builtin_launder)
namespace ebd { namespace detail {
  template <typename T>
  EMBED_NODISCARD EMBED_INLINE constexpr T* launder(T* ptr) noexcept {
    return __builtin_launder(ptr);
  }
}} // end namespace ebd::detail
#   define EMBED_LAUNDER(x) ( ::ebd::detail::launder(x) )
#  else
#   define EMBED_LAUNDER(x) (x)
#  endif
# endif

#ifndef EMBED_UNREACHABLE
# if __cpp_lib_unreachable >= 202202L
#  define EMBED_UNREACHABLE() std::unreachable()
# elif defined(_MSC_VER)
#  define EMBED_UNREACHABLE() __assume(false)
# elif defined(__GNUC__) && (__GNUC__ >= 5)
#  define EMBED_UNREACHABLE() __builtin_unreachable()
# elif EMBED_HAS_BUILTIN(__builtin_unreachable)
#  define EMBED_UNREACHABLE() __builtin_unreachable()
# else
#  define EMBED_UNREACHABLE() 
# endif
#endif

#ifndef EMBED_FAIL_MESSAGE
# if !defined(EMBED_FN_HOOK_TRACE_EMPTY_CALL)
#  define EMBED_FN_HOOK_TRACE_EMPTY_CALL(message)
# endif
# if defined(__OPTIMIZE__) || defined(NDEBUG)
#  define EMBED_FAIL_MESSAGE(message)
# else
#  define EMBED_FAIL_MESSAGE(message) do { EMBED_FN_HOOK_TRACE_EMPTY_CALL(\
  __FILE__ ":" EMBED_DETAIL_TEXT(__LINE__) " " message); } while(0)
# endif
#endif

#if EMBED_CXX_VERSION >= 201103L
# include <cstddef>     // std::size_t
# include <cstring>     // std::memcpy, std::memset
# include <new>         // IWYU pragma: keep (placement new, std::launder(C++17))
# include <utility>     // std::move, std::forward, std::addressof
# include <functional>  // std::bad_function_call
# include <exception>   // std::terminate
# include <type_traits> // std::enable_if, ...
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
// See https://en.cppreference.com/w/cpp/language/noexcept_spec .
// The noexcept-specification is a part of the function type and 
// may appear as part of any function declarator. (Since C++17)

# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, ) EMBED_DETAIL_FN_EXPAND_IMPL(F, noexcept)
#else
# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, )
#endif

/// @brief Similar to `requires` in C++20.
/// Using SFINAE trait `enable_if_t` to require the template arguments.
#define EMBED_DETAIL_REQUIRES_IMPL(require_condition) \
  ::ebd::detail::enable_if_t<(require_condition), int> = 0
#define EMBED_DETAIL_REQUIRES(...)  EMBED_DETAIL_REQUIRES_IMPL((__VA_ARGS__))

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
# define EMBED_DETAIL_REQUIRES_END(...) requires(__VA_ARGS__)
#endif

#define EMBED_DETAIL_TEXT(text) EMBED_DETAIL_TEXT_IMPL(text)
#define EMBED_DETAIL_TEXT_IMPL(text) #text

namespace ebd EMBED_ABI_VISIBILITY(default) {
namespace detail {

/// @brief Here are some standard traits that are not supported in C++11.
inline namespace cxx_traits {

  // See https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1558 .
  template <typename... Args> struct make_void { using type = void; };

  // See https://en.cppreference.com/w/cpp/header/type_traits.html .
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
  // See https://en.cppreference.com/w/cpp/types/result_of.html .
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

  template <typename Func, typename... Args>
  using call_is_nothrow = call_is_nothrow_impl<
    typename invoke_result<Func, Args...>::tag, Func, Args...>;

  // See https://en.cppreference.com/w/cpp/types/reference_converts_from_temporary.html .
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

  // See https://en.cppreference.com/w/cpp/types/is_invocable.html .
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

  // See https://en.cppreference.com/w/cpp/utility/functional/invoke.html .
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

} // end namespace cxx_traits

  // Forward declaration.
  template <std::size_t BufferSize, typename Config, typename Signature>
  class function;

/// @brief Here are some self-defined traits.
inline namespace fn_traits {

  // The value is always false.
  template <typename... Args>
  struct always_false { static constexpr bool value = false; };

  // Is trivial for the purposes of calls. (trivially destruct, copy and move)
  // See https://itanium-cxx-abi.github.io/cxx-abi/abi.html#non-trivial-parameters .
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
    && is_call_trivial<T>::value
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

  template <bool... ConfigArgs>
  struct is_config_package<config_package<ConfigArgs...>>
  : public std::true_type {};

  // Typename parameter package.
  template <typename... Args>
  struct args_package_impl {
    static constexpr std::size_t argsNum = 0;
    using type = args_package_impl<>;
    using next_type = args_package_impl<>;
  };

  template <typename T, typename... Args>
  struct args_package_impl<T, Args...> {
    static constexpr std::size_t argsNum = sizeof...(Args) + 1;
    using type = T;
    using next_type = args_package_impl<Args...>;
  };

  // Implement the "get" trait of args_package.
  template <std::size_t Index, typename Package>
  struct get_args_helper {
    using type = typename 
      get_args_helper<Index-1, typename Package::next_type>::type;
  };

  template <typename Package>
  struct get_args_helper<0, Package> { using type = Package; };

  // Typename parameter package. Easy to find index of element.
  template <typename... Args>
  struct args_package {
    // The `get` is reserved for further use.
    template <std::size_t Index>
    using get = typename get_args_helper<
      Index, args_package_impl<Args...>>::type::type;
    static constexpr std::size_t size = args_package_impl<Args...>::argsNum;
  };

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
    EMBED_FAIL_MESSAGE("[Embedded Function]: Empty function has been called!");
    std::terminate();
  }

  template<bool IsThrowing>
  [[noreturn]] inline enable_if_t<IsThrowing>
  throw_or_terminate() noexcept(!EMBED_CXX_ENABLE_EXCEPTION) {
    EMBED_FAIL_MESSAGE("[Embedded Function]: Empty function has been called!");
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
    bool IsStoredOrigin = !IsView || is_function_ptr<DecT>::value
      || std::is_member_pointer<DecT>::value
  >
  struct is_stored_origin
  : public bool_constant<IsStoredOrigin> {
    static constexpr bool isTrivial = is_traditional_trivial<DecT>::value;
    static_assert(!(IsView && IsStoredOrigin && !isTrivial),
      "Internal error: Stored origin type in view mode must be trivially"
      " copyable/destructible. Here Functor is stored originally,"
      " but it is NOT trivial.");
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

    /// TODO: Finalize the details of the conversion of the qualifiers
    // Check the qualifiers.
    static constexpr bool qualifier_ok = 
      !(unwrap_to::hasConst && !unwrap_from::hasConst)
      && (unwrap_to::hasVolatile == unwrap_from::hasVolatile)
      && (unwrap_to::hasRRef == unwrap_from::hasRRef)
      && (unwrap_to::hasLRef == unwrap_from::hasLRef)
      && (unwrap_to::isNoexcept == unwrap_from::isNoexcept);

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
  // See https://cplusplus.github.io/LWG/issue2116 .
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

  // Check the functor is callable with given arguments.
  template <typename Functor, typename Signature>
  struct is_callable_functor {
    using unwrap_sig = unwrap_signature<Signature>;
    using ret       = typename unwrap_sig::ret;
    using args_pack = typename unwrap_sig::args;
    using dec_func  = decay_t<Functor>;
    using cv_func   = typename unwrap_sig::template add_cv_like<dec_func>;
    using callee    = conditional_t<unwrap_sig::hasRRef, cv_func&&, cv_func&>;
    using res       = invoke_result_package<callee, args_pack>;
    static constexpr bool value = is_invocable_impl<res, ret>::type::value;
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
  template <std::size_t Size>
  struct get_aligned_size {
    static constexpr std::size_t min_aligned = sizeof(void*);
    static constexpr std::size_t aligned_size = ((Size - 1) / min_aligned + 1) * min_aligned;
    static constexpr std::size_t value = Size == 0 ? min_aligned : aligned_size;
  };

  /// @brief Undefined class.
  /// @e EMBED_DETAIL_VIRTUAL_INHERITANCE - This macro is used to inform the MSVC
  /// compiler that this is a declaration of a virtual inheritance class, in order
  /// to obtain the theoretically maximum size of "pointers to member functions".
  class EMBED_DETAIL_VIRTUAL_INHERITANCE UndefinedClass;

  // The default buffer size. Usually is 2 * sizeof(void*).
  struct default_buffer_size {
    // The buffer size for ebd::fn_view. Both pointer and
    // member pointer should be able to be stored into the buffer.
    static constexpr std::size_t view_buf = sizeof(void (UndefinedClass::*) ());
#if defined(EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER)
    // The CommandTable size plus the buffer size is about 8 * sizeof(void).
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

    template <typename Sig>
    static bool check(const ::std::function<Sig>& f) noexcept {
      return static_cast<bool>(f);
    }

    template <std::size_t Buf, typename Cfg, typename Sig>
    static bool check(const function<Buf, Cfg, Sig>& f) noexcept {
      return static_cast<bool>(f);
    }

    template <typename T>
    static bool check(T* f) noexcept {
      return f != nullptr;
    }

    template <typename Class, typename T>
    static bool check(T Class::* f) noexcept {
      return f != nullptr;
    }

    template <typename T>
    static bool check(const T&) noexcept {
      return true;
    }
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

  // Implement the `get_unique_signature`.
  template <typename T>
  struct get_unique_signature_impl {
    static_assert(always_false<T>::value,
      "T must be a function pointer or pointer to member function.");
  };

  // The Config::isThrowing of ebd::fn and ebd::unique_fn is true.
  // So `get_unique_signature_impl` will ignore the `noexcept` specifier.
#define EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE(C, V, REF, NOEXCEPT)    \
  template <typename Class, typename Ret, typename... Args>                   \
  struct get_unique_signature_impl<Ret(Class::*)(Args...) C V REF NOEXCEPT> { \
    using type = Ret(Args...) C V REF;                                        \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE)

#undef EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE

#if ( __cpp_explicit_this_parameter >= 202110L ) || ( EMBED_CXX_VERSION >= 202302L )

  // 3617. function/packaged_task deduction guides and deducing this.
  // See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html .

  // Trait to add qualifiers (const, volatile, &, &&, noexcept) to a function
  // signature by Mapping a `This` type and a base signature to a qualified function type.
  template <typename This, typename Signature>
  struct add_qualifier_with_this;

# define EMBED_DETAIL_ADD_QUALIFIER_WITH_THIS_DEFINE(C, V, REF, NOEXCEPT) \
  template <typename This, typename Ret, typename... Args>                \
  struct add_qualifier_with_this<This C V REF, Ret(Args...) NOEXCEPT> {   \
    using type = Ret(Args...) C V REF NOEXCEPT;                           \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_ADD_QUALIFIER_WITH_THIS_DEFINE)

# undef EMBED_DETAIL_ADD_QUALIFIER_WITH_THIS_DEFINE

  template <typename This, typename Ret, typename... Args>
  struct get_unique_signature_impl<Ret(*)(This, Args...)> {
    using type = typename add_qualifier_with_this<This, Ret(Args...)>::type;
  };

  template <typename This, typename Ret, typename... Args>
  struct get_unique_signature_impl<Ret(*)(This, Args...) noexcept> {
    using type = typename add_qualifier_with_this<This, Ret(Args...) noexcept>::type;
  };

#endif

  // Get the signature of unique callable class.
  template <typename Functor, 
    bool Unique = is_unique_callable<Functor>::value>
  struct get_unique_signature {
    using type = void;
  };

  template <typename Functor>
  struct get_unique_signature<Functor, /* Unique = */ true> {
    using type = typename 
      get_unique_signature_impl<decltype(&Functor::operator())>::type;
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
  // The verification of the "&" and "&&" qualifier is in trait `is_callable_functor`.
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

  // Used to choose either perfect forwarding or pass-by-value.
  // Pass-by-value is faster for scalar types because they can
  // be passed by the register rather than the stack.
#if !defined(EMBED_FN_CONFIG_DISABLE_SMART_FORWARD)
  template <typename T>
  using smart_forward_t = conditional_t<std::is_scalar<T>::value
    || (sizeof(T) <= sizeof(void*) && is_call_trivial<T>::value), 
    T, T&&>;
#else
  template <typename T>
  using smart_forward_t = T&&;
#endif

  // Asserts for functor.
  template <std::size_t BufferSize, typename Config, typename Signature,
            typename Functor, typename Object, typename ErasureT>
  struct asserts_for_function : public std::true_type {
    static_assert(is_callable_functor<Functor, Signature>::value,
      "The functor is NOT callable with given arguments.");

    static_assert(align_size_is_ok<Functor, Config, BufferSize, ErasureT>::value,
      "The size of Functor is too large, and the BufferSize is too small."
      " Try use greater 'BufferSize' as the template argument");

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
  };

  // Is type std::in_place_type_t<T>.
  template <typename>
  struct is_in_place_type : public std::false_type {};

#if EMBED_CXX_VERSION >= 201703L

  template <typename T>
  struct is_in_place_type<std::in_place_type_t<T>> : public std::true_type {};

#endif

} // end namespace fn_traits

// In the namespace "erasure_type", we define a series of 
// types for objects that implement type erasure.
namespace erasure_type {

  template <std::size_t Size>
  union ErasureCoreImpl {
    static_assert(Size > 0, "erasure size must greater than 0");

    void*       fill_ptr;
    const void* fill_const_ptr;
    void (*fill_func_ptr) ();
    char        fill[Size];
  };

  template <std::size_t Size>
  union EMBED_ALIAS ErasureCore {
    char        pod[sizeof(ErasureCoreImpl<Size>)];
    ErasureCoreImpl<Size> unused; // alignas(unused)
  };

  // Passing the `ErasureBase*` as a parameter can avoid the 
  // ABI incompatibility issue between `Erasure<A>&` and `Erasure<B>&`.
  struct ErasureBase {};

  /// @note Erasure is trivial.
  // The well-defined operation of reusing its storage space is to use
  // placement new. After that, using `access` to obtain the address or reference
  // (rather than the content) is also in accordance with the C++ standard.
  // See https://eel.is/c++draft/basic.life#7 .
  template <std::size_t Size>
  struct EMBED_ALIAS Erasure : public ErasureBase {
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
    { return *EMBED_LAUNDER(static_cast<T*>(access())); }

    template <typename T>
    const T& access() const noexcept
    { return *EMBED_LAUNDER(static_cast<const T*>(access())); }

    template <typename T>
    volatile T& access() volatile noexcept
    { return *EMBED_LAUNDER(static_cast<volatile T*>(access())); }

    template <typename T>
    const volatile T& access() const volatile noexcept
    { return *EMBED_LAUNDER(static_cast<const volatile T*>(access())); }
  };

} // end namespace erasure_type

// In the namespace "invocation", we define a series of 
// types for objects that implement the behaviour of invocation.
namespace invocation {

  template <std::size_t Size, typename Config, typename Signature>
  struct InvokerImpl;

#define EMBED_DETAIL_INVOKER_IMPL_DEFINE(C, V, REF, NOEXCEPT)                     \
  template <std::size_t Size, typename Config,                                    \
    typename Ret, typename... Args>                                               \
  struct InvokerImpl<Size, Config, Ret(Args...) C V REF NOEXCEPT> {               \
  public:                                                                         \
    using erasure_base_t = erasure_type::ErasureBase C V;                         \
    using erasure_t = erasure_type::Erasure<Size> C V;                            \
    static constexpr bool is_rvalue_ref =                                         \
      std::is_rvalue_reference<int REF>::value;                                   \
    using invoker_type = Ret (*) (                                                \
      erasure_base_t* base, smart_forward_t<Args>... args);                       \
                                                                                  \
    /* Using when M_erasure is empty. */                                          \
    struct empty {                                                                \
      static Ret invoke(erasure_base_t*, smart_forward_t<Args>...) {              \
        throw_or_terminate<Config::isThrowing>();                                 \
        EMBED_UNREACHABLE();                                                      \
      }                                                                           \
    };                                                                            \
                                                                                  \
    /* Using when Config::isView == false. */                                     \
    struct inplace {                                                              \
      template <typename Functor>                                                 \
      static Ret invoke(erasure_base_t* base, smart_forward_t<Args>... args) {    \
        auto* erased = static_cast<erasure_t*>(base);                             \
        auto& fn = erased->template access<Functor>();                            \
        using Fn = conditional_t<is_rvalue_ref,                                   \
          remove_reference_t<decltype(fn)>&&, remove_reference_t<decltype(fn)>&>; \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
    };                                                                            \
                                                                                  \
    /* Using when Config::isView == true. */                                      \
    struct view {                                                                 \
      /* Using when Functor::is_stored_origin == true. */                         \
      template <typename Functor>                                                 \
      static enable_if_t<is_stored_origin<Functor, true>::value, Ret>             \
      invoke(erasure_base_t* base, smart_forward_t<Args>... args) {               \
        auto* erased = static_cast<erasure_t*>(base);                             \
        auto& fn = erased->template access<Functor>();                            \
        /** @todo @deprecated '&' '&&' shouldn't be sopported in view mode */     \
        using Fn = conditional_t<is_rvalue_ref,                                   \
          remove_reference_t<decltype(fn)>&&, remove_reference_t<decltype(fn)>&>; \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
                                                                                  \
      /* Using when Functor::is_stored_origin == false. */                        \
      template <typename Functor>                                                 \
      static enable_if_t<!is_stored_origin<Functor, true>::value, Ret>            \
      invoke(erasure_base_t* base, smart_forward_t<Args>... args) {               \
        auto* erased = static_cast<erasure_t*>(base);                             \
        auto& fn = *(erased->template access<Functor*>());                        \
        /** @todo @deprecated '&' '&&' shouldn't be sopported in view mode */     \
        using Fn = conditional_t<is_rvalue_ref,                                   \
          remove_reference_t<decltype(fn)>&&, remove_reference_t<decltype(fn)>&>; \
        return invoke_r<Ret>(static_cast<Fn>(fn), std::forward<Args>(args)...);   \
      }                                                                           \
    };                                                                            \
                                                                                  \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_INVOKER_IMPL_DEFINE)

#undef EMBED_DETAIL_INVOKER_IMPL_DEFINE

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
    using invoke_t        = typename invoke_impl_t::invoker_type;
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
      ::new (const_cast<void*>(
        static_cast<erasure_t*>(target)->access()
      )) Functor(std::forward<Object>(obj));
    }

#if EMBED_CXX_VERSION >= 201703L

    // In-place create target object. (Cooperate with std::in_place_type)
    template <typename Functor, typename... CArgs>
    static void emplace_create(erasure_base_t* target, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<Functor, CArgs&&...>::value) {
      ::new (const_cast<void*>(
        static_cast<erasure_t*>(target)->access()
      )) Functor(std::forward<CArgs>(args)...);
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
        default: EMBED_UNREACHABLE(); break;
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
          EMBED_UNREACHABLE(); // move only
          break;
        case OperatorCode::move:
          move<Functor>(dst, src);
          break;
        case OperatorCode::destroy:
          destroy<Functor>(dst);
          break;
        default: EMBED_UNREACHABLE(); break;
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
        default: EMBED_UNREACHABLE(); break;
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
    using manager_t       = typename manager_impl_t::manager_type;

    manager_t m_manager;
    invoker_t m_invoker;

    auto invoke(erasure_base_t* erased, Args&&... args) const
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
    constexpr bool is_empty() const noexcept {
      return m_invoker == &invoker_impl_t::empty::invoke;
    }

    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    void init(erasure_base_t* target, Functor&& obj, std::true_type)
    noexcept(std::is_nothrow_constructible<DecFunctor, Functor&&>::value) {
      m_invoker = &invoker_impl_t::inplace::template invoke<DecFunctor>;
      m_manager = &manager_impl_t::inplace::template manage<DecFunctor, Config::isCopyable>;
      manager_impl_t::template create<DecFunctor>(target, std::forward<Functor>(obj));
    }

    template <typename Functor>
    void init(erasure_base_t*, Functor&&, std::false_type) noexcept {
      static_assert(always_false<Functor>::value,
        "Internal error: When `Config::isView` is false"
        " the Functor must be stored originally.");
      EMBED_UNREACHABLE();
    }

#if EMBED_CXX_VERSION >= 201703L

    // In-place initialize the target with specified arguments.
    template <typename Functor, typename DecFunctor = decay_t<Functor>, typename... CArgs>
    void emplace_init(erasure_base_t* target, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<DecFunctor, CArgs&&...>::value) {
      m_invoker = &invoker_impl_t::inplace::template invoke<DecFunctor>;
      m_manager = &manager_impl_t::inplace::template manage<DecFunctor, Config::isCopyable>;
      manager_impl_t::template emplace_create<DecFunctor>(
        target, std::forward<CArgs>(args)...);
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
    using erasure_t       = typename invoker_impl_t::erasure_t;

    // No m_manager because IsView = true.
    invoker_t m_invoker;

    auto invoke(erasure_base_t* erased, Args&&... args) const
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

    void move(erasure_base_t* EMBED_RESTRICT dst, erasure_base_t* EMBED_RESTRICT src)
    const noexcept {
      clone(dst, src); // Trivial move is same as copy.
    }

    void destroy(erasure_base_t* /*dst*/) const noexcept {
      // Do nothing here
    }

    /// @brief Empty init the `m_invoker` in view mode.
    /// @deprecated As `std::function_ref` has removed empty state, function wrapper
    /// that is in the view mode is not recommended to have null values.
    /// See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p0792r14.html#R1 .
    EMBED_CXX14_CONSTEXPR void set_empty() noexcept {
      m_invoker = &invoker_impl_t::empty::invoke;
    }

    // Check the `m_invoker` is empty::invoke. (constexpr && noexcept)
    constexpr bool is_empty() const noexcept {
      return m_invoker == &invoker_impl_t::empty::invoke;
    }

    // Enable if Functor is stored origin.
    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    void init(erasure_base_t* target, Functor&& obj, std::true_type) noexcept {
      // Since the `is_stored_origin<Functor>` is true, then it must be function
      // pointer or pointer to member, which have nothing about ownership.
      m_invoker = &invoker_impl_t::view::template invoke<DecFunctor>;
      manager_impl_t::template create<DecFunctor>(target, std::forward<Functor>(obj));
    }

    // Enable if Functor is stored by pointer.
    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    void init(erasure_base_t* target, Functor&& obj, std::false_type) noexcept {
      static_assert(!std::is_rvalue_reference<Functor&&>::value,
        "function in view mode cannot be initialized with rvalue reference.");
      m_invoker = &invoker_impl_t::view::template invoke<DecFunctor>;
      manager_impl_t::template create<DecFunctor*>(target, std::addressof(obj));
    }
  };

} // end namespace command

// Move the function's implementation to the base class to simplify the code.
namespace crtp_mixins {

  // Implement the 'operator()' for function.
  template <bool IsView, bool IsThrowing, typename Signature, typename Self>
  struct operator_call_impl; // Undefined

#define EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE(C, V, REF, NOEXCEPT)         \
  template <bool IsView, bool IsThrowing,                                   \
            typename Ret, typename Self, typename... Args>                  \
  struct operator_call_impl<                                                \
    IsView, IsThrowing, Ret(Args...) C V REF NOEXCEPT, Self> {              \
  public:                                                                   \
    EMBED_DETAIL_ALL_DEFAULT(operator_call_impl)                            \
                                                                            \
    Ret operator()(Args... args) C V REF NOEXCEPT {                         \
      auto* self_q = static_cast<Self C V*>(this);                          \
      auto* erased = &(self_q->m_erasure);                                  \
      using command_t = const typename Self::command_t;                     \
      auto& cmd = const_cast<command_t&>(self_q->m_command);                \
      return cmd.invoke(erased, std::forward<Args>(args)...);               \
    }                                                                       \
  };                                                                        \
                                                                            \
  /* Specialized for `ebd::fn_view`, to make its operator() behavior */     \
  /* consistent with `std::function_ref`. */                                \
  template <typename Ret, typename Self, typename... Args>                  \
  struct operator_call_impl</* IsView = */ true, /* IsThrowing = */ false,  \
    Ret(Args...) C V REF NOEXCEPT, Self> {                                  \
  public:                                                                   \
    EMBED_DETAIL_ALL_DEFAULT(operator_call_impl)                            \
                                                                            \
    Ret operator()(Args... args) const NOEXCEPT {                           \
      using erasure_t = typename Self::erasure_t;                           \
      auto* self_q = static_cast<Self const V*>(this);                      \
      auto* erased = const_cast<erasure_t*>(&(self_q->m_erasure));          \
      using command_t = const typename Self::command_t;                     \
      auto& cmd = const_cast<command_t&>(self_q->m_command);                \
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
  protected:
    using erasure_t = erasure_type::Erasure<BufferSize>;

    using command_t = command::CommandTable<
      Config::isView, BufferSize, Config, Signature,
      typename unwrap_signature<Signature>::args>;

    static_assert(is_traditional_trivial<erasure_t>::value,
      "Internal error: erasure_t should be trivial.");

    static_assert(is_traditional_trivial<command_t>::value,
      "Internal error: command_t should be trivial.");

    erasure_t m_erasure;
    command_t m_command;
  };

} // end namespace crtp_mixins

  /// @brief A lightweight and heap-free wrapper for callable objects.
  /// @tparam BufferSize - Specifies the size reserved to store the object.
  /// @tparam Config - Specifies the configuration attributes of the wrapper.
  ///           See @def config_package for details.
  /// @tparam Signature - The signature of the wrapper, e.g., @e `Ret(Args...)`.
  template <std::size_t BufferSize, typename Config, typename Signature>
  class EMBED_DETAIL_FORCE_EBO function
    : public crtp_mixins::member_variable_impl<
        /* Buf = */ BufferSize, /* Cfg = */ Config, /* Sig = */ Signature
      >,
      public crtp_mixins::operator_call_impl<
        /* IsView = */ Config::isView, /* IsThrowing = */ Config::isThrowing,
        Signature, /* Self = */ function<BufferSize, Config, Signature>
      >,
      public crtp_mixins::operator_dereference_impl<
        Signature, /* Self = */ function<BufferSize, Config, Signature>,
        /* IsView = */ Config::isView
      >,
      public crtp_mixins::lifetime_operations_impl<
        /* IsView = */ Config::isView, /* IsCopyable = */ Config::isCopyable,
        Config, /* Self = */ function<BufferSize, Config, Signature>
      >
  {
  private:

    template<std::size_t, typename, typename>
    friend class function;

    template <bool, bool, typename, typename>
    friend struct crtp_mixins::operator_call_impl;

    template <typename, typename>
    friend struct crtp_mixins::copy_impl;

    template <typename, typename>
    friend struct crtp_mixins::move_impl;

    template <typename, typename>
    friend struct crtp_mixins::destructor_impl;

    template <typename, typename, bool, typename>
    friend struct crtp_mixins::operator_dereference_impl;

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
      " configuration option. (Use 'ebd::safe_fn' or 'ebd::fn_view')");

    using MemberVariableBase = crtp_mixins::member_variable_impl<
      BufferSize, Config, Signature>;

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

    /// @brief Return `true` if the function is capyable.
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

    // Create an empty function wrapper.
    function() noexcept {
      m_command.set_empty();
    }

    // Create an empty function wrapper.
    function(std::nullptr_t) noexcept {
      m_command.set_empty();
    }

    // Use `placement new` to create new functor during construction. (Copy)
    // From `function<Buffer_small, ...>` to `function<Buffer_big, ...>`.
    template <std::size_t OtherSize, typename OtherCfg, typename OtherSig,
      EMBED_DETAIL_REQUIRES(fn_can_convert<
        function, function<OtherSize, OtherCfg, OtherSig>
      >::value && function<OtherSize, OtherCfg, OtherSig>::internal_is_copyable)
    >
    function(const function<OtherSize, OtherCfg, OtherSig>& other)
    noexcept((Config::assertNoThrow || Config::isView)
    && (OtherCfg::assertNoThrow || OtherCfg::isView)) {
      using other_fn_t = function<OtherSize, OtherCfg, OtherSig>;
      using other_erasure_t = typename other_fn_t::erasure_t;

      // Suppress GCC warning: "-Wmaybe-uninitialized".
      std::memset(&m_erasure, 0, sizeof(m_erasure));

      other.m_command.clone(
        &m_erasure, const_cast<other_erasure_t*>(&other.m_erasure));
      std::memcpy(&m_command, &other.m_command, sizeof(command_t));
    }

    // Use `placement new` to create new functor during construction. (Move)
    // From `function<Buffer_small, ...>` to `function<Buffer_big, ...>`.
    template <std::size_t OtherSize, typename OtherCfg, typename OtherSig,
      EMBED_DETAIL_REQUIRES(fn_can_convert<
        function, function<OtherSize, OtherCfg, OtherSig>
      >::value)
    >
    function(function<OtherSize, OtherCfg, OtherSig>&& other)
    noexcept((Config::assertNoThrow || Config::isView)
    && (OtherCfg::assertNoThrow || OtherCfg::isView)) {
      other.m_command.move(&m_erasure, &other.m_erasure);
      std::memcpy(&m_command, &other.m_command, sizeof(command_t));
      other.m_command.destroy(&other.m_erasure);
      other.m_command.set_empty();
    }

    /// @brief Builds a Fn that targets a copy/move of the incoming function object.
    /// @param functor - A callable object with parameters of type `Args...`
    /// and returns a value convertible to `Ret`. (The Signature is `Ret(Args...)`)
    template <typename Functor, 
      EMBED_DETAIL_REQUIRES(!fn_can_convert<function, Functor>::value),
      EMBED_DETAIL_REQUIRES(!is_self<Functor, function>::value),
      EMBED_DETAIL_REQUIRES(!is_in_place_type<decay_t<Functor>>::value)
    > function(Functor&& functor)
    noexcept(is_nothrow_construct_from_functor<Functor&&>::value) {

      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Functor, Functor&&, erasure_t>::value,
        "Internal error: asserts_for_function<...>::value should be always true.");

      if (check_not_empty::check(functor)) {
        m_command.template init<>(
          &m_erasure, std::forward<Functor>(functor), 
          is_stored_origin<decay_t<Functor>, Config::isView>{});
      } else {
        m_command.set_empty();
      }
    }

#if EMBED_CXX_VERSION >= 201703L

    /// @brief In-place constructs the Fn within the internal storage with specified arguments.
    /// @param args - The arguments for constructing the Fn.
    template <typename Fn, typename... CArgs,
      EMBED_DETAIL_REQUIRES(always_false<Fn>::value || !Config::isView),
      EMBED_DETAIL_REQUIRES(std::is_constructible<Fn, CArgs...>::value),
      EMBED_DETAIL_REQUIRES(is_callable_functor<Fn, Signature>::value)
    > explicit function(std::in_place_type_t<Fn>, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<Fn, CArgs...>::value) {

      static_assert(std::is_same<Fn, decay_t<Fn>>::value,
        "decay_t<Fn> should be the same type as Fn.");
      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Fn, Fn, erasure_t>::value,
        "Internal error: asserts_for_function<...>::value should be always true.");

      m_command.template emplace_init<Fn>(&m_erasure, std::forward<CArgs>(args)...);
    }

    /// @brief In-place constructs the Fn within the internal storage with init_list and specified arguments.
    /// @param il - The initializer_list for constructing the Fn.
    /// @param args - The arguments for constructing the Fn.
    template <typename Fn, typename U, typename... CArgs,
      EMBED_DETAIL_REQUIRES(always_false<Fn>::value || !Config::isView),
      EMBED_DETAIL_REQUIRES(std::is_constructible<Fn, std::initializer_list<U>&, CArgs...>::value),
      EMBED_DETAIL_REQUIRES(is_callable_functor<Fn, Signature>::value)
    > explicit function(std::in_place_type_t<Fn>, std::initializer_list<U> il, CArgs&&... args)
    noexcept(std::is_nothrow_constructible<Fn, CArgs...>::value) {

      static_assert(std::is_same<Fn, decay_t<Fn>>::value,
        "decay_t<Fn> should be the same type as Fn.");
      static_assert(asserts_for_function<
          BufferSize, Config, Signature, Fn, Fn, erasure_t>::value,
        "Internal error: asserts_for_function<...>::value should be always true.");

      m_command.template emplace_init<Fn>(&m_erasure, il, std::forward<CArgs>(args)...);
    }

#endif

    // Return `true` if the object is empty.
    EMBED_CXX14_CONSTEXPR bool is_empty() const noexcept {
      return m_command.is_empty();
    }

    // Return `true` if the object is NOT empty.
    EMBED_CXX14_CONSTEXPR explicit operator bool() const noexcept {
      return !is_empty();
    }

    // Clear the object.
    void clear() noexcept(Config::assertNoThrow || Config::isView) {
      m_command.destroy(&m_erasure);
      m_command.set_empty();
    }

    // Swap the contents of two function objects. (Inplace mode)
    template <typename Unused = void, 
      EMBED_DETAIL_REQUIRES(always_false<Unused>::value || !Config::isView)
    > void swap(function& fn) noexcept(Config::assertNoThrow) {
      // Avoid self swap.
      if (this == std::addressof(fn)) { return; }

      erasure_t tmp_nil{}; // Empty temporary var

      // Move source from `m_erasure` to `tmp_nil`.
      m_command.move(&tmp_nil, &m_erasure);
      m_command.destroy(&m_erasure);

      // Move source from `fn.m_erasure` to `m_erasure`.
      fn.m_command.move(&m_erasure, &fn.m_erasure);
      fn.m_command.destroy(&fn.m_erasure);

      // Move source from `tmp_nil` to `fn.m_erasure`.
      m_command.move(&fn.m_erasure, &tmp_nil);
      m_command.destroy(&tmp_nil);

      std::swap(m_command, fn.m_command);
    }

    // Swap the contents of two function objects. (View mode)
    template <typename Unused = void, 
      EMBED_DETAIL_REQUIRES(always_false<Unused>::value || Config::isView)
    > void swap(function& fn) noexcept {
      std::swap(m_erasure, fn.m_erasure);
      std::swap(m_command, fn.m_command);
    }

    // Overload the function specifically for the case where nullptr is
    // passed as a parameter, in order to improve the program's running
    // efficiency. (Using the `swap` method would be much slower.)
    function& operator=(std::nullptr_t)
    noexcept(Config::assertNoThrow || Config::isView) {
      if (!is_empty()) { clear(); }
      return *this;
    }

    // Assign a callable object to the object.
    template <typename Functor, 
      EMBED_DETAIL_REQUIRES(!fn_can_convert<function, Functor>::value),
      EMBED_DETAIL_REQUIRES(!is_self<Functor, function>::value)
    > function& operator=(Functor&& fn)
    noexcept(is_nothrow_construct_from_functor<Functor&&>::value) {
      function(std::forward<Functor>(fn)).swap(*this);
      return *this;
    }

    // Assign another `function` object to this object.
    // Enable if the `function` object can be converted to the current object.
    template <std::size_t OtherSize, typename OtherCfg, typename OtherSig,
      EMBED_DETAIL_REQUIRES(fn_can_convert<
        function, function<OtherSize, OtherCfg, OtherSig>
      >::value)
    >
    function& operator=(const function<OtherSize, OtherCfg, OtherSig>& other)
    noexcept((Config::assertNoThrow || Config::isView)
    && (OtherCfg::assertNoThrow || OtherCfg::isView)) {
      function(other).swap(*this);
      return *this;
    }
  };

  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig>
  EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator==(const function<Buf, Cfg, Sig>& fn, std::nullptr_t) noexcept {
    return fn.is_empty();
  }

  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig>
  EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator==(std::nullptr_t, const function<Buf, Cfg, Sig>& fn) noexcept {
    return fn.is_empty();
  }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig>
  EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator!=(const function<Buf, Cfg, Sig>& fn, std::nullptr_t) noexcept {
    return !fn.is_empty();
  }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <std::size_t Buf, typename Cfg, typename Sig>
  EMBED_INLINE EMBED_CXX14_CONSTEXPR bool 
  operator!=(std::nullptr_t, const function<Buf, Cfg, Sig>& fn) noexcept {
    return !fn.is_empty();
  }

  // Make a function.
  template <typename Fn, bool NoThrow, typename... CArgs>
  inline Fn make_function_impl(CArgs&&... args) noexcept(NoThrow) {
    static_assert(is_ebd_fn<Fn>::value,
      "Fn must be the alias of `ebd::detail::function`.");
    return Fn{std::forward<CArgs>(args)...};
  }

} // end namespace detail

/**
 * @brief A function object wrapper for copyable and callable objects.
 * 
 * @tparam Signature - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam BufferSize - Buffer size. Used for storing the callable object.
 * And the buffer size will be aligned automatically.
 * 
 * @internal `Config` - Configuration package. Used to configure the wrapper.
 *  @arg IsCopyable - Here is `true`, means the callable object must be copyable.
 *  @arg IsView - Here is `false`, which means this is NOT a view.
 *  @arg IsThrowing - Here is `true`, which means the wrapper will throw std::bad_function_call.
 *  @arg AssertNoThrow - Here is `false`, which means the callable object doesn't need
 *       to make sure it doesn't throw exceptions when constructing and destructing.
 */
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
using fn = detail::function<
  detail::get_aligned_size<BufferSize>::value, 
  detail::config_package<
    /* IsCopyable = */          true, 
    /* IsView = */              false, 
    /* IsThrowing = */          true, 
    /* AssertObjectNoThrow = */ false
  >, 
  Signature
>;

/**
 * @brief A function object wrapper for movable and callable objects.
 * 
 * @tparam Signature - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam BufferSize - Buffer size. Used for storing the callable object.
 * And the buffer size will be aligned automatically.
 * 
 * @internal `Config` - Configuration package. Used to configure the wrapper.
 *  @arg IsCopyable - Here is `false`, means the callable object can be move-only(copy-only is also OK).
 *  @arg IsView - Here is `false`, which means this is NOT a view.
 *  @arg IsThrowing - Here is `true`, which means the wrapper will throw std::bad_function_call.
 *  @arg AssertNoThrow - Here is `false`, which means the callable object doesn't need
 *       to make sure it doesn't throw exceptions when constructing and destructing.
 */
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
using unique_fn = detail::function<
  detail::get_aligned_size<BufferSize>::value, 
  detail::config_package<
    /* IsCopyable = */          false, 
    /* IsView = */              false, 
    /* IsThrowing = */          true, 
    /* AssertObjectNoThrow = */ false
  >, 
  Signature
>;

/**
 * @brief A @b SAFE function object wrapper for copyable and callable objects.
 * 
 * @throws Strong noexcept guarantee. (ASSERT-NO-THROW)
 * 
 * @tparam Signature - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam BufferSize - Buffer size. Used for storing the callable object.
 * And the buffer size will be aligned automatically.
 * 
 * @internal `Config` - Configuration package. Used to configure the wrapper.
 *  @arg IsCopyable - Here is `true`, means the callable object must be copyable.
 *  @arg IsView - Here is `false`, which means this is NOT a view.
 *  @arg IsThrowing - Here is `false`, which means the wrapper will not throw std::bad_function_call.
 *  @arg AssertNoThrow - Here is `true`, which means the callable object must
 *       to make sure it doesn't throw exceptions when constructing and destructing.
 */
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
using safe_fn = detail::function<
  detail::get_aligned_size<BufferSize>::value, 
  detail::config_package<
    /* IsCopyable = */          true, 
    /* IsView = */              false, 
    /* IsThrowing = */          false, 
    /* AssertObjectNoThrow = */ true
  >, 
  Signature
>;

/**
 * @brief A function object view for callable objects.
 * 
 * @tparam Signature - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam Unused - Unused.
 * 
 * @internal `Config` - Configuration package. Used to configure the wrapper.
 *  @arg IsCopyable - Here is `true`, but unused because this is a view.
 *  @arg IsView - Here is `true`, which means this is a view.
 *  @arg IsThrowing - Here is `false`, which means the wrapper will not throw std::bad_function_call.
 *  @arg AssertNoThrow - Here is `false`, which means the callable object doesn't need
 *       to make sure it doesn't throw exceptions when constructing and destructing.
 */
template <typename Signature, std::size_t Unused = 0 /* Unused */>
using fn_view = detail::function<
  detail::default_buffer_size::view_buf, 
  detail::config_package<
    /* IsCopyable = */          true, 
    /* IsView = */              true, 
    /* IsThrowing = */          false, 
    /* AssertObjectNoThrow = */ false
  >, 
  Signature
>;

/**
 * @brief A basic function wrapper that users can customize.
 * 
 * This alias provides the most flexible way to instantiate a function wrapper
 * by directly specifying all configuration parameters. It is intended for
 * advanced use cases where none of the predefined aliases (`fn`, `unique_fn`,
 * `safe_fn`, `fn_view`) satisfy the required combination of copyability,
 * view semantics, exception behavior, and no‑throw assertions.
 * 
 * @tparam Signature              Function signature, e.g., `void(int, char)`.
 * @tparam BufferSize             Size of the internal storage (in bytes).
 *                                The value will be automatically aligned.
 * @tparam IsCopyable             If `true`, the stored callable object must be
 *                                copy‑constructible; otherwise, move‑only is
 *                                sufficient (copyable is still accepted but
 *                                only move operations will be used).
 * @tparam IsView                 If `true`, the wrapper acts as a non‑owning
 *                                view (no copy/move/destroy of the target).
 *                                The stored object is either stored directly
 *                                (if trivially copyable) or by pointer.
 * @tparam IsThrowing             If `true`, calling an empty wrapper throws
 *                                `std::bad_function_call` (if exceptions are
 *                                enabled); otherwise, `std::terminate` is called.
 * @tparam AssertObjectNoThrow    If `true`, the wrapper requires that the
 *                                callable object's construction, destruction,
 *                                copy, and move operations are `noexcept`.
 *                                Violations trigger a `static_assert`.
 * 
 * @note   This template is marked as **unstable and experimental** in v2.0.x.
 *         The exact set of template parameters and their semantics may change
 *         in future versions. Prefer using the predefined aliases unless you
 *         need a combination not covered by them.
 * 
 * EXAMPLE: a move-only, non‑throwing wrapper with a custom buffer size:
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
  detail::get_aligned_size<BufferSize>::value, 
  detail::config_package<
    /* IsCopyable = */          IsCopyable, 
    /* IsView = */              IsView, 
    /* IsThrowing = */          IsThrowing, 
    /* AssertObjectNoThrow = */ AssertObjectNoThrow
  >, 
  Signature
>;


/// @brief make_fn[0]: Make function with specified signature for copyable functor.
/// @return `fn<Signature, sizeof(Functor)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  typename Signature, // [User specify] function signature.
  typename Functor,   // [Auto] Functor type.
  // [Auto] Get the nothrow guarantee of functor.
  bool NoThrow = std::is_nothrow_copy_constructible<Functor>::value
)
EMBED_DETAIL_REQUIRES_END(
  // [Require] Functor must be copyable.
  std::is_copy_constructible<Functor>::value
  // [Require] First template argument must be signature.
  && detail::unwrap_signature<Signature>::isSignature
  // [Require] Functor cannot be the function pointer or pointer to member function.
  && std::is_class<detail::remove_cvref_t<Functor>>::value
)
EMBED_NODISCARD inline fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    fn<Signature, sizeof(Functor)>, NoThrow
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
  && !std::is_copy_constructible<Functor>::value
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
  // [Auto] The buffersize of functor.
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
template <typename Class, typename T>
EMBED_NODISCARD inline auto make_fn(T Class::* ptr_memobj) noexcept
-> fn<T(Class&) const, sizeof(ptr_memobj)> {
  return detail::make_function_impl<
    fn<T(Class&) const, sizeof(ptr_memobj)>,
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
/// @tparam Fn - Can be `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn`, or `ebd::fn_view`.
/// @return `Fn<Signature, sizeof(functor)>`
EMBED_DETAIL_TEMPLATE_BEGIN(
  template <class, std::size_t> class Fn,
  typename Functor,
  typename Deduction = decltype(make_fn(std::declval<Functor>())),
  typename Signature = typename detail::is_ebd_fn<Deduction>::signature,
  std::size_t BufferSize = sizeof(detail::decay_t<Functor>),
  typename FnWrapper = Fn<Signature, BufferSize>,
  bool NoThrow = noexcept(FnWrapper(std::declval<Functor>()))
)
EMBED_DETAIL_REQUIRES_END(
  detail::is_ebd_fn<FnWrapper>::value
  && detail::unwrap_signature<Signature>::isSignature
)
EMBED_NODISCARD inline FnWrapper make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    /* Fn = */ FnWrapper, /* NoThrow = */ NoThrow
  >(std::forward<Functor>(functor));
}

} // end namespace ebd

#undef EMBED_DETAIL_FN_EXPAND
#undef EMBED_DETAIL_FN_EXPAND_IMPL
#undef EMBED_DETAIL_REQUIRES
#undef EMBED_DETAIL_REQUIRES_IMPL
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
#if defined(EMBED_FN_CONFIG_UNDEF_MACROS)
// #undef most of the EMBED_* macros if EMBED_FN_CONFIG_UNDEF_MACROS is defined.
// EMBED_CXX_VERSION and EMBED_CXX_ENABLE_EXCEPTION are reserved.
# undef EMBED_ALIAS
# undef EMBED_HAS_BUILTIN
# undef EMBED_HAS_ATTRIBUTE
# undef EMBED_HAS_CXX_ATTRIBUTE
# undef EMBED_ABI_VISIBILITY
# undef EMBED_CXX14_CONSTEXPR
# undef EMBED_INLINE
# undef EMBED_RESTRICT
# undef EMBED_NODISCARD
# undef EMBED_FALLTHROUGH
# undef EMBED_LAUNDER
# undef EMBED_UNREACHABLE
# undef EMBED_FAIL_MESSAGE

# undef EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER
# undef EMBED_FN_CONFIG_DISABLE_SMART_FORWARD
# undef EMBED_FN_CONFIG_UNDEF_MACROS
# undef EMBED_FN_HOOK_TRACE_EMPTY_CALL
#endif

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // EMBED_INCLUDED_EMBED_FUNCTION_HPP_

