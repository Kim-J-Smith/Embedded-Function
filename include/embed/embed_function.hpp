/**
 * @file        embed_function.hpp
 * 
 * @date        2026-2-7
 * 
 * @version     2.0.0
 * 
 * @copyright   Copyright (c) 2026 Kim-J-Smith
 *              (https://github.com/Kim-J-Smith/embed-function)
 *              All rights reserved.
 * 
 * @attention   This source is released under the MIT license
 *              (http://opensource.org/licenses/MIT)
 */

// Just like function pointers, it is quick and efficient.

#ifndef INCLUDED_EMBED_FUNCTION_HPP_
#define INCLUDED_EMBED_FUNCTION_HPP_

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable: 4514 4668 4005 4710 4371)
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

#ifndef EMBED_CXX17_NOEXCEPT
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_CXX17_NOEXCEPT noexcept
# else
#  define EMBED_CXX17_NOEXCEPT
# endif
#endif

#ifndef EMBED_CXX14_CONSTEXPR
# if EMBED_CXX_VERSION >= 201402L
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

#ifndef EMBED_NODISCARD
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_NODISCARD [[nodiscard]]
# elif defined(__GNUC__) || defined(__clang__)
#  define EMBED_NODISCARD __attribute__((warn_unused_result))
# else
#  define EMBED_NODISCARD
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
# if defined(_MSC_VER)
#  define EMBED_UNREACHABLE() __assume(false)
# elif defined(__GNUC__) && (__GNUC__ >= 5)
#  define EMBED_UNREACHABLE() __builtin_unreachable()
# elif EMBED_HAS_BUILTIN(__builtin_unreachable)
#  define EMBED_UNREACHABLE() __builtin_unreachable()
# else
#  define EMBED_UNREACHABLE() 
# endif
#endif

#if EMBED_CXX_VERSION >= 201103L
# include <cstddef>    // std::size_t
# include <cstring>    // std::memcpy
# include <new>        // placement new, std::launder(C++17)
# include <utility>    // std::move, std::forward, std::addressof
# include <functional> // std::bad_function_call
# include <type_traits>
# include <exception>
#else
# error The embed-function requires the support of syntax features of C++11.\
 You can use the -std=c++11 compilation option, or simply switch to a newer compiler.
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
// See https://en.cppreference.com/w/cpp/language/noexcept_spec
// The noexcept-specification is a part of the function type and 
// may appear as part of any function declarator. (Since C++17)

# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, ) EMBED_DETAIL_FN_EXPAND_IMPL(F, noexcept)
#else
# define EMBED_DETAIL_FN_EXPAND(F) \
  EMBED_DETAIL_FN_EXPAND_IMPL(F, )
#endif

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

  // (undocumented) Tags that used in `invoke_result`, `invoke`, `invoke_r`, etc.
  class invoke_tag_normal {};
  class invoke_tag_memfn_ref_like {};
  class invoke_tag_memfn_pointer_like {};
  class invoke_tag_memobj_ref_like {};
  class invoke_tag_memobj_pointer_like {};

  // (undocumented) Unwrap the `std::reference_wrapper` recursively.
  template <typename T, typename U = remove_cvref_t<T>>
  struct inv_unwrap { using type = T; };

  template <typename T, typename UnderType>
  struct inv_unwrap<T, std::reference_wrapper<UnderType>>
  { using type = typename inv_unwrap<UnderType&>::type; };

  template <typename T>
  using inv_unwrap_t = typename inv_unwrap<T>::type;

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

  // (undocumented) Extract type information from 'operator T()'.
  template <typename Class, typename T, typename = void>
  struct has_type_conversion_operator
  : public std::false_type {};

  template <typename Class, typename T>
  struct has_type_conversion_operator<Class, T,
    void_t<decltype(&Class::operator T)>>
  : public std::true_type {};

  // (undocumented) Check whether type `From` can be converted 
  // to type `To`, without invoking the operator T <REF>.
  template <typename To, typename From>
  struct is_no_reference_convertible {
    using To_no_cvref = remove_cvref_t<To>;
    static constexpr bool value = std::is_convertible<From, To>::value
      && !has_type_conversion_operator<From, To_no_cvref&>::value
      && !has_type_conversion_operator<From, const To_no_cvref&>::value
      && !has_type_conversion_operator<From, volatile To_no_cvref&>::value
      && !has_type_conversion_operator<From, const volatile To_no_cvref&>::value
      && !has_type_conversion_operator<From, To_no_cvref&&>::value
      && !has_type_conversion_operator<From, const To_no_cvref&&>::value
      && !has_type_conversion_operator<From, volatile To_no_cvref&&>::value
      && !has_type_conversion_operator<From, const volatile To_no_cvref&&>::value;
  };

  // (undocumented) True if `To` is a reference type, a `From` value 
  // can be bound to `To` in copy-initialization, and a temporary 
  // object would be bound to the reference, false otherwise.
  template <typename To, typename From>
  struct reference_converts_from_temporary_impl {
    using From_ = conditional_t<
      std::is_scalar<From>::value || std::is_void<From>::value,
      remove_cv_t<From>, From>;

    using NoRefTo = remove_reference_t<To>;

    static constexpr bool bound_rref = std::is_rvalue_reference<To>::value
      && !std::is_reference<From_>::value 
      && is_no_reference_convertible<To, From_>::value;

    static constexpr bool bound_lref = std::is_lvalue_reference<To>::value
      && !std::is_reference<From_>::value 
      && std::is_const<NoRefTo>::value
      && !std::is_volatile<NoRefTo>::value
      && is_no_reference_convertible<To, From_>::value;

    static constexpr bool value = bound_rref || bound_lref;
  };

  // See https://en.cppreference.com/w/cpp/types/reference_converts_from_temporary.html .
  template <typename To, typename From>
  struct reference_converts_from_temporary
  : public std::integral_constant<bool, 
#if EMBED_HAS_BUILTIN(__reference_converts_from_temporary) && !defined(__clang__)
    // Clang 20 still has issues with __reference_converts_from_temporary.
    __reference_converts_from_temporary(To, From)
#else
    reference_converts_from_temporary_impl<To, From>::value
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
    static typename std::integral_constant<bool, NoThrow>
    test(int) noexcept { return {}; }

    using type = decltype(test<Ret, true>(1));
    using nothrow = decltype(test<Ret>(1));
  };

#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

  // See https://en.cppreference.com/w/cpp/types/is_invocable.html .
  template <typename Ret, typename Func, typename... Args>
  struct is_invocable_r : public std::integral_constant<
    bool, is_invocable_impl<invoke_result<Func, Args...>, Ret>::type::value
  >::type {};

  template <typename Ret, typename Func, typename... Args>
  struct is_nothrow_invocable_r : public std::integral_constant<
    bool, call_is_nothrow<Func, Args...>::value
     && is_invocable_impl<invoke_result<Func, Args...>, Ret>::nothrow::value
  >::type {};

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
  { return static_cast<inv_unwrap_t<Arg>&&>(arg).*std::forward<MemObj>(obj); }

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
    return (static_cast<inv_unwrap_t<Arg>&&>(arg).*std::forward<MemFunc>(memfn))(
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
    // `true` if the callable object is copyable.
    static constexpr bool isCopyable = IsCopyable;
    // `true` if the callable object is not managed by the wrapper.
    static constexpr bool isView = IsView;
    // `true` if the empty-calling will throw std::bad_function_call.
    static constexpr bool isThrowing = IsThrowing;
    // `true` if the callable object is nothrow callable.
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
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_UNWRAP_SIGNATURE_DEFINE)

#undef EMBED_DETAIL_UNWRAP_SIGNATURE_DEFINE

  // Check the type is ebd::detail::function or not.
  template <typename T>
  struct is_ebd_fn_impl : public std::false_type {};

  template <std::size_t Buf, typename Cfg, typename Sig>
  struct is_ebd_fn_impl<function<Buf, Cfg, Sig>>
  : public std::integral_constant<bool, 
    unwrap_signature<Sig>::isSignature
    && is_config_package<Cfg>::value
  > {};

  template <typename T>
  using is_ebd_fn = is_ebd_fn_impl<remove_cvref_t<T>>;

  // Throw std::bad_function_call or just call std::terminate().
  template<bool IsThrowing>
  [[noreturn]] inline enable_if_t<!IsThrowing>
  throw_or_abort() noexcept {
    std::terminate();
  }

  template<bool IsThrowing>
  [[noreturn]] inline enable_if_t<IsThrowing>
  throw_or_abort() noexcept(!EMBED_CXX_ENABLE_EXCEPTION) {
#if ( EMBED_CXX_ENABLE_EXCEPTION == true )
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
  template <typename T, bool IsView>
  struct is_stored_origin : public std::integral_constant<
    bool, !IsView || is_function_ptr<decay_t<T>>::value || 
    std::is_member_pointer<decay_t<T>>::value
  >::type {};

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

  template <std::size_t Buf1, typename Cfg1, typename Sig1,
    std::size_t Buf2, typename Cfg2, typename Sig2>
  struct fn_can_convert_impl<
    function<Buf1, Cfg1, Sig1>, function<Buf2, Cfg2, Sig2>
  > {
    using sig1_ret = typename unwrap_signature<Sig1>::ret;
    using sig2_ret = typename unwrap_signature<Sig2>::ret;
    using sig1_args = typename unwrap_signature<Sig1>::args;
    using sig2_args = typename unwrap_signature<Sig2>::args;
    static constexpr bool buf_ok = Buf1 >= Buf2;
    static constexpr bool cfg_ok = 
      Cfg1::isCopyable <= Cfg2::isCopyable // From strict to loose.
      && Cfg1::isView == Cfg2::isView
      && Cfg1::isThrowing == Cfg2::isThrowing
      && Cfg1::assertNoThrow <= Cfg2::assertNoThrow; // From strict to loose.
    static constexpr bool sig_ret_ok = std::is_same<sig1_ret, sig2_ret>::value;
    static constexpr bool sig_args_ok = std::is_same<sig1_args, sig2_args>::value;
    static constexpr bool value = buf_ok && cfg_ok && sig_ret_ok && sig_args_ok;
  };

  // Check ebd::detail::function are similar or not.
  template <typename To, typename From>
  struct fn_can_convert : public std::integral_constant<
    bool, fn_can_convert_impl<
      remove_reference_t<To>, remove_reference_t<From>
    >::value
  >::type {};

  // Nothrow constructible from functor to ebd::fn
  template <typename Functor, typename Config>
  struct is_nothrow_construct_from_functor
  : public std::integral_constant<bool, 
    (Config::isCopyable && std::is_nothrow_copy_constructible<Functor>::value)
    || (!Config::isCopyable && std::is_nothrow_move_constructible<Functor>::value)
  > {};

  // Get invoke result with arguments package.
  template <typename Fn, typename ArgsPackage>
  struct invoke_result_package {
    static_assert(!std::is_void<void_t<Fn>>::value /* always false */,
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
    using callee    = conditional_t<unwrap_sig::hasRRef, dec_func&&, dec_func&>;
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

  // Get aligned size.
  template <std::size_t Size>
  struct get_aligned_size {
    static constexpr std::size_t min_aligned = sizeof(void*);
    static constexpr std::size_t aligned_size = ((Size - 1) / min_aligned + 1) * min_aligned;
    static constexpr std::size_t value = Size == 0 ? min_aligned : aligned_size;
  };

  // Undefined class.
  class UndefinedClass;

  // The default buffer size. Usually is 2 * sizeof(void*).
  struct default_buffer_size {
    static constexpr std::size_t value = sizeof(void (UndefinedClass::*) ());
  };

  // Check the throwing is ok.
  template <typename Functor, typename Object, typename Config,
    typename DecFunctor = decay_t<Functor>>
  struct assert_throwing_is_ok {
    static constexpr bool is_ok = std::is_nothrow_destructible<DecFunctor>::value
      && (std::is_nothrow_copy_constructible<DecFunctor>::value || 
        !std::is_copy_constructible<DecFunctor>::value)
      && (std::is_nothrow_move_constructible<DecFunctor>::value || 
        !std::is_move_constructible<DecFunctor>::value)
      && std::is_nothrow_constructible<DecFunctor, Object>::value;

    static constexpr bool value = 
      Config::isView || !(Config::assertNoThrow && !is_ok);
  };

  // Check empty.
  struct check_not_empty {

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
    static bool check(const T& f) {
      return true;
    }
  };

  // Check copyable.
  template <typename Functor, typename Config, 
    typename DecFunctor = decay_t<Functor>>
  struct copyable_is_ok {
    static constexpr bool copy_ok = std::is_copy_constructible<DecFunctor>::value;
    static constexpr bool move_ok = std::is_move_constructible<DecFunctor>::value;
    static constexpr bool value = Config::isCopyable ? copy_ok : move_ok;
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
    static_assert(!std::is_same<T, T>::value /* always false */,
      "T must be a function pointer or pointer to member function.");
  };

#define EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE(C, V, REF, NOEXCEPT)    \
  template <typename Class, typename Ret, typename... Args>                   \
  struct get_unique_signature_impl<Ret(Class::*)(Args...) C V REF NOEXCEPT> { \
    using type = Ret(Args...) C V REF NOEXCEPT;                               \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE)

#undef EMBED_DETAIL_GET_UNIQUE_SIGNATURE_IMPL_DEFINE

#if ( __cpp_explicit_this_parameter >= 202110L ) || ( EMBED_CXX_VERSION >= 202302L )

  // 3617. function/packaged_task deduction guides and deducing this.
  // See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html .

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
  template <typename With, typename T>
  struct get_qualified_with {
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
    static constexpr bool hasConst = false;
    static constexpr bool hasRRef = false;
    static constexpr bool hasLRef = false;
    static constexpr bool noConst = false;
    static constexpr bool noRRef = false;
    static constexpr bool noLRef = false;
  };

  template <typename Package, typename Fn>
  struct is_class_call_operator<Package, Fn, true> {
    using call_const_res = invoke_result_package<const Fn, Package>;
    using call_lref_res = invoke_result_package<Fn&, Package>;
    using call_rref_res = invoke_result_package<Fn&&, Package>;

    // check the `const` qualifier.
    static constexpr bool hasConst = 
      is_invocable_impl<call_const_res, void>::type::value;
    static constexpr bool noConst = !hasConst;

    // check the `&&` qualifier.
    static constexpr bool noRRef = 
      is_invocable_impl<call_lref_res, void>::type::value;
    static constexpr bool hasRRef = !noRRef;

    // check the `&&` qualifier.
    static constexpr bool noLRef = 
      is_invocable_impl<call_rref_res, void>::type::value;
    static constexpr bool hasLRef = !noLRef;
  };

  // Check the qualifier of signature and functor is matching.
  /// TODO: Improve the verification of the "&" and "&&" qualifier.
  template <typename Signature, typename Functor>
  struct qualifier_of_signature_match_functor {
    using base_fn = remove_cvref_t<Functor>;
    using unwrap_sig = unwrap_signature<Signature>;
    using call_op = is_class_call_operator<typename unwrap_sig::args, base_fn>;

    // The qualifier information of `Signature`.
    static constexpr bool sig_has_const = unwrap_sig::hasConst;
    static constexpr bool sig_has_rref = unwrap_sig::hasRRef;
    static constexpr bool sig_has_lref = unwrap_sig::hasLRef;

    static constexpr bool const_match = !(sig_has_const && call_op::noConst);
    static constexpr bool rref_match = true; // TODO
    static constexpr bool lref_match = true; // TODO
    static constexpr bool value = const_match && rref_match && lref_match;
  };

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

#define EMBED_DETAIL_INVOKER_IMPL_DEFINE(C, V, REF, NOEXCEPT)       \
  template <std::size_t Size, typename Config,                      \
    typename Ret, typename... Args>                                 \
  struct InvokerImpl<Size, Config, Ret(Args...) C V REF NOEXCEPT> { \
  public:                                                           \
    using erasure_base_t = erasure_type::ErasureBase C V;           \
    using erasure_t = erasure_type::Erasure<Size> C V;              \
    static constexpr bool is_rv_ref =                               \
      std::is_rvalue_reference<int REF>::value;                     \
    using invoker_type = Ret (*) (                                  \
      erasure_base_t* base, Args&&... args);                        \
                                                                    \
    struct empty {                                                  \
      /* Using when M_erasure is empty. */                          \
      static Ret invoke(erasure_base_t* base, Args&&... args) {     \
        throw_or_abort<Config::isThrowing>();                       \
        EMBED_UNREACHABLE();                                        \
      }                                                             \
    };                                                              \
                                                                    \
    struct inplace {                                                \
      /* Using when Config::isView == false. */                     \
      template <typename Functor>                                   \
      static Ret invoke(erasure_base_t* base, Args&&... args) {     \
        auto* erased = static_cast<erasure_t*>(base);               \
        auto& fn = erased->template access<Functor>();              \
        using Fn = conditional_t<is_rv_ref,                         \
          remove_reference_t<decltype(fn)>&&,                       \
          remove_reference_t<decltype(fn)>&>;                       \
        return invoke_r<Ret>(static_cast<Fn>(fn),                   \
          std::forward<Args>(args)...);                             \
      }                                                             \
    };                                                              \
                                                                    \
    struct view {                                                   \
      /* Using when Config::isView == true. */                      \
      template <typename Functor>                                   \
      static enable_if_t<                                           \
        is_stored_origin<Functor, true>::value, Ret>                \
      invoke(erasure_base_t* base, Args&&... args) {                \
        auto* erased = static_cast<erasure_t*>(base);               \
        auto& fn = erased->template access<Functor>();              \
        using Fn = conditional_t<is_rv_ref,                         \
          remove_reference_t<decltype(fn)>&&,                       \
          remove_reference_t<decltype(fn)>&>;                       \
        return invoke_r<Ret>(static_cast<Fn>(fn),                   \
          std::forward<Args>(args)...);                             \
      }                                                             \
      template <typename Functor>                                   \
      static enable_if_t<                                           \
        !is_stored_origin<Functor, true>::value, Ret>               \
      invoke(erasure_base_t* base, Args&&... args) {                \
        auto* erased = static_cast<erasure_t*>(base);               \
        auto& fn = *(erased->template access<Functor*>());          \
        using Fn = conditional_t<is_rv_ref,                         \
          remove_reference_t<decltype(fn)>&&,                       \
          remove_reference_t<decltype(fn)>&>;                       \
        return invoke_r<Ret>(static_cast<Fn>(fn),                   \
          std::forward<Args>(args)...);                             \
      }                                                             \
    };                                                              \
                                                                    \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_INVOKER_IMPL_DEFINE)

#undef EMBED_DETAIL_INVOKER_IMPL_DEFINE

} // end namespace invocation

// In the namespace "management", we define a series of 
// types for objects that implement the behaviour of management.
namespace management {

  enum class OperatorCode {
    clone = 0,
    move,
    destroy
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
    static void clone(erasure_base_t* dst, erasure_base_t* src)
    noexcept(std::is_nothrow_copy_constructible<Functor>::value) {
      const auto& src_obj = *get_pointer<Functor>(src);
      create<Functor>(dst, src_obj);
    }

    // Move type-erased object from `src` to `dst`.
    template <typename Functor>
    static void move(erasure_base_t* dst, erasure_base_t* src)
    noexcept(std::is_nothrow_move_constructible<Functor>::value) {
      auto& src_obj = *get_pointer<Functor>(src);
      create<Functor>(dst, std::move(src_obj));
    }

    struct empty {
      static void manage(OperatorCode, erasure_base_t*, erasure_base_t*) {
        // Nothing here.
      }
    };

    struct inplace {
      template <typename Functor, bool IsCopyable>
      static enable_if_t<IsCopyable>
      manage(OperatorCode op, erasure_base_t* dst, erasure_base_t* src) {
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

      template <typename Functor, bool IsCopyable>
      static enable_if_t<!IsCopyable> // move only
      manage(OperatorCode op, erasure_base_t* dst, erasure_base_t* src) {
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
    }; // end inplace
  };

} // end namespace management

namespace command {

  /// @note CommandTable is trivial
  template <bool IsView, std::size_t Size, 
    typename Config, typename Signature, typename ArgsPackage>
  struct CommandTable;

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

    void clone(erasure_base_t* dst, erasure_base_t* src) const
    noexcept(Config::assertNoThrow) {
      m_manager(management::OperatorCode::clone, dst, src);
    }

    void move(erasure_base_t* dst, erasure_base_t* src) const
    noexcept(Config::assertNoThrow) {
      m_manager(management::OperatorCode::move, dst, src);
    }

    void destroy(erasure_base_t* dst) const
    noexcept(Config::assertNoThrow) {
      m_manager(management::OperatorCode::destroy, dst, nullptr);
    }

    // Empty init.
    void set_empty() noexcept {
      m_invoker = &invoker_impl_t::empty::invoke;
      m_manager = &manager_impl_t::empty::manage;
    }

    // Check the `m_invoker` is empty::invoke. (constexpr && noexcept)
    constexpr bool is_empty() const noexcept {
      return m_invoker == &invoker_impl_t::empty::invoke;
    }

    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    void init(erasure_base_t* target, Functor&& obj, std::true_type) noexcept {
      m_invoker = &invoker_impl_t::inplace::template invoke<DecFunctor>;
      m_manager = &manager_impl_t::inplace::template manage<DecFunctor, Config::isCopyable>;
      manager_impl_t::template create<DecFunctor>(target, std::forward<Functor>(obj));
    }
  };

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

    void clone(erasure_base_t* dst, erasure_base_t* src) const noexcept {
      auto* destination = static_cast<erasure_t*>(dst);
      auto* source = static_cast<erasure_t*>(src);
      std::memcpy(
        const_cast<void*>(destination->access()), 
        const_cast<const void*>(source->access()), 
        sizeof(erasure_t)
      );
    }

    void move(erasure_base_t* dst, erasure_base_t* src) const noexcept {
      clone(dst, src); // Trivial move is same as copy.
    }

    void destroy(erasure_base_t* /*dst*/) const noexcept {
      // Do nothing here
    }

    // Empty init.
    void set_empty() noexcept {
      m_invoker = &invoker_impl_t::empty::invoke;
    }

    // Check the `m_invoker` is empty::invoke. (constexpr && noexcept)
    constexpr bool is_empty() const noexcept {
      return m_invoker == &invoker_impl_t::empty::invoke;
    }

    // Enable if Functor is stored origin.
    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    void init(erasure_base_t* target, Functor&& obj, std::true_type) noexcept {
      m_invoker = &invoker_impl_t::view::template invoke<DecFunctor>;
      manager_impl_t::template create<DecFunctor>(target, std::forward<Functor>(obj));
    }

    // Enable if Functor is stored by pointer.
    template <typename Functor, typename DecFunctor = decay_t<Functor>>
    void init(erasure_base_t* target, Functor&& obj, std::false_type) noexcept {
      m_invoker = &invoker_impl_t::view::template invoke<DecFunctor>;
      manager_impl_t::template create<DecFunctor*>(target, std::addressof(obj));
    }
  };

} // end namespace command

  // Implement the 'operator()' for function.
  template <typename Signature, typename Self>
  struct operator_call_impl; // Undefined

#define EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE(C, V, REF, NOEXCEPT)         \
  template <typename Ret, typename Self, typename... Args>                  \
  struct operator_call_impl<Ret(Args...) C V REF NOEXCEPT, Self> {          \
  public:                                                                   \
    operator_call_impl()                                      = default;    \
    ~operator_call_impl()                                     = default;    \
    operator_call_impl(const operator_call_impl&)             = default;    \
    operator_call_impl(operator_call_impl&&)                  = default;    \
    operator_call_impl& operator=(const operator_call_impl&)  = default;    \
    operator_call_impl& operator=(operator_call_impl&&)       = default;    \
                                                                            \
    Ret operator()(Args... args) C V REF NOEXCEPT {                         \
      auto* self_q = static_cast<Self C V*>(this);                          \
      auto* self = const_cast<Self*>(self_q);                               \
      auto* erased = &(self_q->m_erasure);                                  \
      return self->m_command.invoke(erased, std::forward<Args>(args)...);   \
    }                                                                       \
  };

  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE)

#undef EMBED_DETAIL_OPERATOR_CALL_IMPL_DEFINE

  // Implement the copy constructor and copy assignment for function.
  template <bool IsCopyable /* = true */, typename Config, typename Self>
  struct clone_impl {
    clone_impl()                        = default;
    ~clone_impl()                       = default;
    clone_impl(clone_impl&&)            = default;
    clone_impl& operator=(clone_impl&&) = default;

    // Use `placement new` to create new functor during construction,
    // which will call functor's copy-constructor.
    clone_impl(const clone_impl& that)
    noexcept(Config::assertNoThrow || Config::isView) {
      auto* self = static_cast<Self*>(this);
      auto& other = static_cast<const Self&>(that);
      using erasure_t = typename Self::erasure_t;
      using command_t = typename Self::command_t;
      other.m_command.clone(&self->m_erasure, const_cast<erasure_t*>(&other.m_erasure));
      std::memcpy(&self->m_command, &other.m_command, sizeof(command_t));
    }

    // Copy assignment.
    clone_impl& operator=(const clone_impl& other)
    noexcept(Config::assertNoThrow || Config::isView) {
      auto new_item = clone_impl(other);
      static_cast<Self&>(new_item).swap(static_cast<Self&>(*this));
      return *this;
    }
  };

  template <typename Config, typename Self>
  struct clone_impl</* IsCopyable = */ false, Config, Self> {
    clone_impl()                        = default;
    ~clone_impl()                       = default;
    clone_impl(clone_impl&&)            = default;
    clone_impl& operator=(clone_impl&&) = default;

    clone_impl(const clone_impl&)             = delete;
    clone_impl& operator=(const clone_impl&)  = delete;
  };

  template <std::size_t BufferSize, typename Config, typename Signature>
  class function
    : public operator_call_impl<
        Signature, /* Self = */ function<BufferSize, Config, Signature>
      >,
      public clone_impl<
        /* IsCopyable = */ Config::isCopyable || Config::isView,
        Config, /* Self = */ function<BufferSize, Config, Signature>
      >
  {
  private:

    template<std::size_t, typename, typename>
    friend class function;

    template <typename, typename>
    friend struct operator_call_impl;

    template <bool, typename, typename>
    friend struct clone_impl;

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
      "The third argument must be valid function signature."
      " The signature must be like 'Ret(Args...) QUALIFIER'.");

    /// Check the "noexcept" is same.
    static_assert(!(Config::isThrowing && unwrap_signature<Signature>::isNoexcept),
      "This 'noexcept' qualifier is in conflict with"
      " the 'IsThrowing' configuration option.");

    using erasure_t = erasure_type::Erasure<BufferSize>;

    using command_t = command::CommandTable<
      Config::isView, BufferSize, Config, Signature,
      typename unwrap_signature<Signature>::args>;

    static_assert(std::is_trivial<command_t>::value, 
      "command_t should be trivial. This is internal error.");

    // The `m_erasure` contains the type-erased object.
    erasure_t m_erasure{};

    // The `m_command` is responsible for managing and invoking the `m_erasure`.
    command_t m_command{};

  public:

    // The buffer size.
    static constexpr std::size_t buffer_size = BufferSize;

    // The return type.
    using result_type = typename unwrap_signature<Signature>::ret;

    // `true` if self is copyable.
    static constexpr bool is_copyable = Config::isCopyable || Config::isView;

    ~function() noexcept(Config::assertNoThrow || Config::isView) {
      m_command.destroy(&m_erasure);
    }
    
    // Create an empty function wrapper.
    function() noexcept {
      m_command.set_empty();
    }

    // Create an empty function wrapper.
    function(std::nullptr_t) noexcept {
      m_command.set_empty();
    }

    // Implemented in base class `clone_impl`.
    // `=delete` if `is_copyable == false`.
    function(const function& other) = default;

    // Use `placement new` to create new functor during construction,
    // which will call functor's move-constructor.
    function(function&& other)
    noexcept(Config::assertNoThrow || Config::isView) {
      other.m_command.move(&m_erasure, &other.m_erasure);
      std::memcpy(&m_command, &other.m_command, sizeof(command_t));
      other.m_command.destroy(&other.m_erasure);
      other.m_command.set_empty();
    }

    // Use `placement new` to create new functor during construction. (Copy)
    // From `function<Buffer_small, ...>` to `function<Buffer_big, ...>`.
    template <std::size_t OtherSize, typename OtherCfg, typename OtherSig,
      typename Enable = enable_if_t<fn_can_convert<
        function, function<OtherSize, OtherCfg, OtherSig>
      >::value && is_copyable>
    >
    function(const function<OtherSize, OtherCfg, OtherSig>& other)
    noexcept((Config::assertNoThrow || Config::isView)
    && (OtherCfg::assertNoThrow || OtherCfg::isView)) {
      using other_fn_t = function<OtherSize, OtherCfg, OtherSig>;
      using other_erasure_t = typename other_fn_t::erasure_t;
      other.m_command.clone(
        &m_erasure, const_cast<other_erasure_t*>(&other.m_erasure));
      std::memcpy(&m_command, &other.m_command, sizeof(command_t));
    }

    // Use `placement new` to create new functor during construction. (Move)
    // From `function<Buffer_small, ...>` to `function<Buffer_big, ...>`.
    template <std::size_t OtherSize, typename OtherCfg, typename OtherSig,
      typename Enable = enable_if_t<fn_can_convert<
        function, function<OtherSize, OtherCfg, OtherSig>
      >::value>
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
      typename Enable1 = enable_if_t<!fn_can_convert<function, Functor>::value>,
      typename Enable2 = enable_if_t<!is_self<Functor, function>::value>>
    function(Functor&& functor)
    noexcept(is_nothrow_construct_from_functor<Functor, Config>::value) {

      static_assert(is_callable_functor<Functor, Signature>::value,
        "The functor is NOT callable with given argumemts.");

      static_assert(align_size_is_ok<Functor, Config, BufferSize, erasure_t>::value,
        "The size of Functor is too large, and the BufferSize is too small."
        " Try use greater 'BufferSize' as the template argument");

      static_assert(assert_throwing_is_ok<Functor, Functor&&, Config>::value,
        "The 'Functor' may throw exceptions during construction and destruction,"
        " which does not match the 'Config::assertNoThrow = true' setting.");

      static_assert(copyable_is_ok<Functor, Config>::value, 
        "Functor cannot match the Config::isCopyable setting.");

      static_assert(!move_constructor_is_deleted<Functor>::value,
        "The move constructor of Functor shouldn't be deleted.");

      static_assert(qualifier_of_signature_match_functor<Signature, Functor>::value,
        "The qualifier 'const', '&' or '&&' of operator() of Functor"
        " cannot match that of Signature.");
      
      if (check_not_empty::check(functor)) {
        m_command.template init<>(
          &m_erasure, std::forward<Functor>(functor), 
          is_stored_origin<decay_t<Functor>, Config::isView>{});
      } else {
        m_command.set_empty();
      }
    }

    // Return `true` if the object is empty.
    EMBED_CXX14_CONSTEXPR bool is_empty() const noexcept {
      return m_command.is_empty();
    }

    // Return `true` if the object is NOT empty.
    EMBED_CXX14_CONSTEXPR explicit operator bool() const noexcept {
      return !is_empty();
    }

    // Clear the object.
    EMBED_CXX14_CONSTEXPR void clear() 
    noexcept(Config::assertNoThrow || Config::isView) {
      m_command.destroy(&m_erasure);
      m_command.set_empty();
    }

    // Swap the contents of two function objects.
    void swap(function& fn)
    noexcept(Config::assertNoThrow || Config::isView) {
      erasure_t tmp_nil{};
      if (!is_empty()) {
        m_command.move(&tmp_nil, &m_erasure);
        m_command.destroy(&m_erasure);
      }
      if (static_cast<bool>(fn)) {
        fn.m_command.move(&m_erasure, &fn.m_erasure);
        fn.m_command.destroy(&fn.m_erasure);
      }
      if (!is_empty()) {
        m_command.move(&fn.m_erasure, &tmp_nil);
        m_command.destroy(&tmp_nil);
      }
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

    // Move assignment.
    function& operator=(function&& other)
    noexcept(Config::assertNoThrow || Config::isView) {
      clear();
      if (!other.is_empty()) {
        other.m_command.move(&m_erasure, &other.m_erasure);
        std::memcpy(&m_command, &other.m_command, sizeof(command_t));
        other.m_command.destroy(&other.m_erasure);
        other.m_command.set_empty();
      }
      return *this;
    }

    // Implemented in base class `clone_impl`.
    // `=delete` if `is_copyable == false`.
    function& operator=(const function& other) = default;

    // Assign a callable object to the object.
    template <typename Functor, 
      typename Enable1 = enable_if_t<!fn_can_convert<function, Functor>::value>,
      typename Enable2 = enable_if_t<!is_self<Functor, function>::value>>
    function& operator=(Functor&& fn)
    noexcept(is_nothrow_construct_from_functor<Functor, Config>::value) {
      function(std::forward<Functor>(fn)).swap(*this);
      return *this;
    }

    // Assign another `function` object to this object.
    // Enable if the `function` object can be converted to the current object.
    template <std::size_t OtherSize, typename OtherCfg, typename OtherSig,
      typename Enable = enable_if_t<fn_can_convert<
        function, function<OtherSize, OtherCfg, OtherSig>
      >::value>
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
  template <typename Fn, bool NoThrow, typename Functor>
  inline Fn make_function_impl(Functor&& functor) noexcept(NoThrow) {
    static_assert(is_ebd_fn<Fn>::value,
      "Fn must be the alias of `ebd::detail::function`.");
    return Fn{std::forward<Functor>(functor)};
  }

} // end namespace detail

/**
 * @brief A function object wrapper for copyable and callable objects.
 * 
 * @tparam `Signature` - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam `BufferSize` - Buffer size. Used for storing the callable object.
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
 * @tparam `Signature` - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam `BufferSize` - Buffer size. Used for storing the callable object.
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
 * @tparam `Signature` - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam `BufferSize` - Buffer size. Used for storing the callable object.
 * And the buffer size will be aligned automatically.
 * 
 * @internal `Config` - Configuration package. Used to configure the wrapper.
 *  @arg IsCopyable - Here is `true`, means the callable object must be copyable.
 *  @arg IsView - Here is `false`, which means this is NOT a view.
 *  @arg IsThrowing - Here is `false`, which means the wrapper will not throw std::bad_function_call.
 *  @arg AssertNoThrow - Here is `true`, which means the callable object doesn't need
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
 * @tparam `Signature` - Function signature. Seems like `Ret(Args...)`.
 * 
 * @tparam `Unused` - Unused.
 * 
 * @internal `Config` - Configuration package. Used to configure the wrapper.
 *  @arg IsCopyable - Here is `true`, but unused because this is a view.
 *  @arg IsView - Here is `true`, which means this is a view.
 *  @arg IsThrowing - Here is `false`, which means the wrapper will not throw std::bad_function_call.
 *  @arg AssertNoThrow - Here is `false`, which means the callable object doesn't need
 *       to make sure it doesn't throw exceptions when constructing and destructing.
 */
template <typename Signature, std::size_t = 0 /* Unused */>
using fn_view = detail::function<
  detail::default_buffer_size::value, 
  detail::config_package<
    /* IsCopyable = */          true, 
    /* IsView = */              true, 
    /* IsThrowing = */          false, 
    /* AssertObjectNoThrow = */ false
  >, 
  Signature
>;

/// @brief make_fn[0]: Make function with specified signature for copyable functor.
/// @return `fn<Signature, sizeof(Functor)>`
template <
  typename Signature, // [User specify] function signature.
  typename Functor,   // [Auto] Functor type.
  // [Auto] Get the nothrow guarantee of functor.
  bool NoThrow = std::is_nothrow_copy_constructible<Functor>::value,
  // [Enable] Functor must be copyable.
  typename Enable1 = detail::enable_if_t<std::is_copy_constructible<Functor>::value>,
  // [Enable] First template argument must be signature.
  typename Enable2 = detail::enable_if_t<detail::unwrap_signature<Signature>::isSignature>
>
EMBED_NODISCARD inline fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    fn<Signature, sizeof(Functor)>, NoThrow
  >(std::forward<Functor>(functor));
}

/// @brief make_fn[1]: Make function with specified signature for move-only functor.
/// @return `unique_fn<Signature, sizeof(Functor)>`
template <
  typename Signature, // [User specify] function signature.
  typename Functor,   // [Auto] Functor type.
  // [Enable] Functor must be movable and non-copyable.
  typename Enable1 = detail::enable_if_t<
    std::is_move_constructible<Functor>::value
    && !std::is_copy_constructible<Functor>::value
  >,
  // [Enable] First template argument must be signature.
  typename Enable2 = detail::enable_if_t<detail::unwrap_signature<Signature>::isSignature>,
  // [Auto] Get the nothrow guarantee of functor.
  bool NoThrow = std::is_nothrow_move_constructible<Functor>::value
>
EMBED_NODISCARD inline unique_fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept(NoThrow) {
  return detail::make_function_impl<
    unique_fn<Signature, sizeof(Functor)>, NoThrow
  >(std::forward<Functor>(functor));
}

/// @brief make_fn[2]: Make an empty function with specified signature and buffer size.
/// @return `fn<Signature, BufferSize>`
template <
  typename Signature, // [User specify] function signature.
  std::size_t BufferSize = detail::default_buffer_size::value,
  // [Enable] First template argument must be signature.
  typename Enable = detail::enable_if_t<detail::unwrap_signature<Signature>::isSignature>
>
EMBED_NODISCARD inline fn<Signature, BufferSize>
make_fn() noexcept {
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
template <
  typename Signature, // [User specify] function signature.
  // [Auto] The type of the function pointer.
  typename FunctionPtr = typename detail::unwrap_signature<Signature>::pure_sig*,
  // [Enable] First template argument must be signature.
  typename Enable1 = detail::enable_if_t<detail::unwrap_signature<Signature>::isSignature>,
  // [Enable] The `FunctionPtr` must be the function pointer.
  typename Enable2 = detail::enable_if_t<detail::is_function_ptr<FunctionPtr>::value>
>
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
template <
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
  bool NoThrow = std::is_nothrow_constructible<Class, Lambda&&>::value,
  // [Enable] The functor must be unique callable.
  typename Enable1 = detail::enable_if_t<
    detail::is_unique_callable<Class>::value
  >,
  // [Enable] The signature must be valid.
  typename Enable2 = detail::enable_if_t<
    detail::unwrap_signature<Signature>::isSignature
  >
>
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
  EMBED_DETAIL_FN_EXPAND(EMBED_DETAIL_MAKE_FN_DEFINE);

#undef EMBED_DETAIL_MAKE_FN_DEFINE

} // end namespace ebd

namespace std EMBED_ABI_VISIBILITY(default) {

  // Swap `::ebd::safe_fn` is safe.
  // This function will not throw exceptions.
  template <std::size_t Buf, typename Sig>
  inline void swap(
    ::ebd::safe_fn<Sig, Buf>& f1,
    ::ebd::safe_fn<Sig, Buf>& f2
  ) noexcept { f1.swap(f2); }

  // Swap `::ebd::fn_view` is safe.
  // This function will not throw exceptions.
  template <std::size_t Buf, typename Sig>
  inline void swap(
    ::ebd::fn_view<Sig, Buf>& f1,
    ::ebd::fn_view<Sig, Buf>& f2
  ) noexcept { f1.swap(f2); }

}

#undef EMBED_DETAIL_FN_EXPAND
#undef EMBED_DETAIL_FN_EXPAND_IMPL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // INCLUDED_EMBED_FUNCTION_HPP_

