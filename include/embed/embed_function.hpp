/******************************************************************************
The MIT License(MIT)

https://github.com/Kim-J-Smith/embed-function

Copyright (c) 2025 Kim-J-Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/
/**
 * @file        embed_function.hpp
 * 
 * @brief       A very tiny C++ wrapper for callable objects.
 * 
 * @version     1.1.2
 * 
 * @date        2026-1-27
 * 
 * @author      Kim-J-Smith
 * 
 * Wrapper for any callable object, including functor, free function, lambda, etc.
 * 
 * Design intent is to do what `std::function` do but without
 * allocation of heap memory, virtual function. Whats more, user
 * can even disable the c++ exception to decrease ROM consumption.
 * 
 * The usage method of embed::function is almost exactly the same
 * as that of std::function. If users are familiar with the operation
 * of std::function, they can quickly get familiar with embed::function
 * as well. embed::function ensures that no heap memory is used.
 * 
 * After careful consideration, embed::function does not plan to implement
 * the `target_type()` and `target()` member functions. The main reason for 
 * the former is that it relies on RTTI, which is often disabled in the embedded 
 * domain. The latter is due to the reason of thread-safe access isolation.
 * Additionally, Unlike std::function, embed::function can wrap move-only objects.
 * (more details: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4159.pdf)
 * 
 * By default, the space occupied by a single instance is only the size of
 * 2 pointers(buffer + manager). For larger lambda function objects, embed::function
 * allows users to specify the second template parameter, that is, the
 * buffer size to accommodate it (but such behavior is not recommended.
 * It is a better choice to wrap it with a lambda function and then pass it in).
 * 
 * If the user does not disable the C++ exception system, they will receive
 * a warning during the compilation process (this is merely a reminder and
 * will not actually affect the compilation). The user needs to choose to disable
 * C++ exceptions or to ignore this warning by defining a macro (the specific
 * warning content will include more detailed instructions on how to handle it).
 * 
 * EXAMPLE:
 *  
 *  embed::function<Signature> fn = free_function;
 * 
 *  embed::function<void() const> fn = []() { printf("hello\n"); };
 * 
 *  embed::function<Signature> fn = callable_class{};
 * 
 *  embed::function<Signature, sizeof(callable_class)> = callable_class{...};
 * 
 * 
 *  auto fn = embed::make_function<Signature>(free_function);
 * 
 *  auto fn = embed::make_function(free_function); // auto infer the Signature
 * 
 *  auto fn = embed::make_function<void(char)>([](int) { printf("hello\n"); });
 * 
 *  auto fn = embed::make_function([](int) { printf("hello\n"); }); // auto infer the Signature
 * 
 *  auto fn = embed::make_function<Signature>(callable_class{});
 * 
 *  auto fn = embed::make_function(callable_class{}); // auto infer the Signature
 * 
 *  auto fn = embed::make_function(&my_class::member_function); // auto infer the Signature
 * 
 * 
 *  embed::Fn fn = free_function; // require C++17 template deduce guide
 * 
 *  embed::Fn fn = []() { printf("hello\n"); }; // require C++17 template deduce guide
 * 
 *  embed::Fn fn = callable_class{}; // require C++17 template deduce guide
 * 
 *  embed::Fn fn = callable_class{...}; // require C++17 template deduce guide
 * 
 */

/// @c C++11 "embed_function.hpp"
#ifndef EMBED_FUNCTION_HPP_
#define EMBED_FUNCTION_HPP_

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable: 4514 4668 4005 4710 4371)
#endif
////////////////////////////////////////////////////////////////

/// @note User can customize following configs

/**
 * When fast invocation is enabled, the rate of function calls increases
 * by approximately 50%, which is consistent with the invocation overhead
 * of `std::function`. With fast-call enabled, each instance of 
 * `embed::function` uses an extra pointer-sized RAM (In the fast mode,
 * it occupies a total of two pointer sizes plus the buffer size of RAM space).
 */
#define EMBED_FN_NEED_FAST_CALL     false

// Assert that the wrapped callable object does not throw exceptions.
#define EMBED_FN_NOTHROW_CALLABLE   false

// Ensure that no `bad_function_call` exception is thrown if true.
// When an empty embed::Fn is called, the `bad_function_call_handler` function 
// will be invoked for handling. This function can be customized by the user.
#define EMBED_FN_ENSURE_NO_THROW    true

// Disable the warnings of embed::Fn
#define EMBED_FN_NO_WARING          false

////////////////////////////////////////////////////////////////


// The accurate version of C++.
#ifndef EMBED_CXX_VERSION
# if defined(_MSC_VER) && ( _MSC_VER >= 1900 )
#  define EMBED_CXX_VERSION _MSVC_LANG
# else
#  define EMBED_CXX_VERSION __cplusplus
# endif
#endif

/// @c EMBED_ALIAS
#ifndef EMBED_ALIAS
# if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#  define EMBED_ALIAS __attribute__((may_alias))
# else
#  define EMBED_ALIAS
# endif
#endif

/// @c EMBED_HAS_BUILTIN
#ifndef EMBED_HAS_BUILTIN
# if defined(__has_builtin) && defined(__is_identifier)
#  define EMBED_HAS_BUILTIN(x) (__has_builtin(x) || !__is_identifier(x))
# else
#  define EMBED_HAS_BUILTIN(x) 0
# endif
#endif

/// @c EMBED_CXX_ENABLE_EXCEPTION
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

/// @brief warning if exception is enabled.
#if ( !EMBED_NO_WARNING ) && ( !EMBED_FN_NO_WARING )
# if ( EMBED_CXX_ENABLE_EXCEPTION != 0 )
#  if defined(_MSC_VER)
#   pragma message(__FILE__ " [WARNING]: You are using c++ exception, which may consume more ROM.\
 Try not use '/EHs-c- /D _HAS_EXCEPTIONS=0 /wd4577' to disable the exception. Or if you exactly\
 want to enable the exception, then please use '/D EMBED_NO_WARNING=1' to ignore this warning.")
#  else
#   warning You are using c++ exception, which may consume more ROM.\
 Try use '-fno-exceptions' to disable the exception. Or if you exactly\
 want to enable the exception, then please use '-DEMBED_NO_WARNING=1' to ignore this warning.
#  endif
# endif
#endif

/// @c EMBED_ABI_VISIBILITY(x)
#ifndef EMBED_ABI_VISIBILITY
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_ABI_VISIBILITY(x) __attribute__((visibility(#x)))
# else
#  define EMBED_ABI_VISIBILITY(x) 
# endif
#endif

/// @c EMBED_CXX17_NOEXCEPT
#ifndef EMBED_CXX17_NOEXCEPT
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_CXX17_NOEXCEPT noexcept
# else
#  define EMBED_CXX17_NOEXCEPT
# endif
#endif

/// @c EMBED_CXX14_CONSTEXPR
#ifndef EMBED_CXX14_CONSTEXPR
# if EMBED_CXX_VERSION >= 201402L
#  define EMBED_CXX14_CONSTEXPR constexpr
# else
#  define EMBED_CXX14_CONSTEXPR
# endif
#endif

/// @c EMBED_FN_CASE_NOEXCEPT
/// @brief Only when C++ version is greater than C++17,
/// c++ exception is disabled, and user need nothrow callable
/// functor, @b EMBED_FN_CASE_NOEXCEPT will be defined as noexcept.
#ifndef EMBED_FN_CASE_NOEXCEPT
# if EMBED_CXX_VERSION >= 201703L && EMBED_FN_NOTHROW_CALLABLE && !EMBED_CXX_ENABLE_EXCEPTION
#  define EMBED_FN_CASE_NOEXCEPT noexcept
# else
#  define EMBED_FN_CASE_NOEXCEPT
# endif
#endif

/// @c EMBED_INLINE
#ifndef EMBED_INLINE
# if defined(__GNUC__) || defined(__clang__) || defined(__TASKING__)
#  define EMBED_INLINE inline __attribute__((always_inline))
# elif defined(_MSC_VER) || defined(__IAR_SYSTEMS_ICC__)
#  define EMBED_INLINE __forceinline
# else
#  define EMBED_INLINE inline
# endif
#endif

/// @c EMBED_NODISCARD
#ifndef EMBED_NODISCARD
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_NODISCARD [[nodiscard]]
# elif defined(__GNUC__) || defined(__clang__)
#  define EMBED_NODISCARD __attribute__((warn_unused_result))
# else
#  define EMBED_NODISCARD
# endif
#endif

/// @c EMBED_UNUSED
#ifndef EMBED_UNUSED
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_UNUSED [[maybe_unused]]
# elif defined(__GNUC__) || defined(__clang__) || defined(__TI_COMPILER_VERSION__)
#  define EMBED_UNUSED __attribute__((unused))
# else
#  define EMBED_UNUSED
# endif
#endif

/// @c EMBED_LIKELY(condition) @c EMBED_UNLIKELY(condition)
#ifndef EMBED_LIKELY
# if EMBED_CXX_VERSION >= 202002L
#  define EMBED_LIKELY(condition) (condition) [[likely]]
#  define EMBED_UNLIKELY(condition) (condition) [[unlikely]]
# elif defined(__GNUC__) || defined(__clang__)
#  define EMBED_LIKELY(condition) (__builtin_expect(static_cast<bool>(condition), 1))
#  define EMBED_UNLIKELY(condition) (__builtin_expect(static_cast<bool>(condition), 0))
# elif EMBED_HAS_BUILTIN(__builtin_expect)
#  define EMBED_LIKELY(condition) (__builtin_expect(static_cast<bool>(condition), 1))
#  define EMBED_UNLIKELY(condition) (__builtin_expect(static_cast<bool>(condition), 0))
# else
#  define EMBED_LIKELY(condition) (condition)
#  define EMBED_UNLIKELY(condition) (condition)
# endif
#endif

/// @c EMBED_SWITCH_EXPECT(condition,expect_val)
#ifndef EMBED_SWITCH_EXPECT
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_SWITCH_EXPECT(condition,expect_val) __builtin_expect(condition,expect_val)
# elif EMBED_HAS_BUILTIN(__builtin_expect)
#  define EMBED_SWITCH_EXPECT(condition,expect_val) __builtin_expect(condition,expect_val)
# else
#  define EMBED_SWITCH_EXPECT(condition,expect_val) (condition)
# endif
#endif

/// @c EMBED_UNREACHABLE()
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

/// @c EMBED_PUSH_STD @c EMBED_POP_STD
#ifndef EMBED_PUSH_STD
# if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER) || defined(__INTEL_COMPILER)
#  define EMBED_PUSH_STD _Pragma("push_macro(\"std\")")
#  define EMBED_POP_STD _Pragma("pop_macro(\"std\")")
# else
#  define EMBED_PUSH_STD
#  define EMBED_POP_STD
# endif
#endif

// Header files
#if EMBED_CXX_VERSION >= 201103L
# ifndef EMBED_NO_STD_HEADER
#  include <cstddef> // std::size_t
#  include <new> // placement new, std::launder(C++17)
#  include <utility> // std::move, std::forward, std::addressof
#  include <functional> // std::bad_function_call
#  include <type_traits>
#  include <exception>
# else
/// @brief In some extreme cases, users cannot use standard header files.
/// Here, alternative solutions are provided.
EMBED_PUSH_STD
#  include "embed_function_nostd.hpp"
#  define std detail::fn_no_std
# endif
#else
# error The embed-function requires the support of syntax features of C++11.\
 You can use the -std=c++11 compilation option, or simply switch to a newer compiler.
#endif

////////////////////////////////////////////////////////////////
namespace embed { namespace detail {

  /// @note User can customize following configs

  // the default buffer size for `embed::Fn`.
  constexpr std::size_t FnDefaultBufSize = (1 * sizeof(void*));

  // The callback function is to handle the `bad_function_call`
  // only when the C++ exception is disabled.
  [[noreturn]] EMBED_UNUSED inline void bad_function_call_handler() noexcept
  {
    /// Your can deal with the `bad_function_call` here.
    /// Or you can just ignore this function, and use
    /// @e `std::set_terminate` instead.
    /// @example std::set_terminate(handle_reset);

    std::terminate(); // Terminate the program
  }

  // The callback function is to handle the case that
  // copying non-copyable object that has been wrapped in `embed::Fn` instance.
  [[noreturn]] EMBED_UNUSED inline void bad_function_copy_handler() noexcept
  {
    /// Your can deal with the bad function copy here.
    /// Or you can just ignore this function, and use
    /// @e `std::set_terminate` instead.
    /// @example std::set_terminate(handle_reset);

    std::terminate(); // Terminate the program
  }

} } // end namespace embed::detail
////////////////////////////////////////////////////////////////

// Helper macro to generate overload code.
# define EMBED_FN_GENERATE_CODE_C_V_REF(F_) \
  F_(, , )                                  \
  F_(const, , )                             \
  F_(, volatile, )                          \
  F_(, , &)                                 \
  F_(const, volatile, )                     \
  F_(const, , &)                            \
  F_(, volatile, &)                         \
  F_(const, volatile, &)                    \
  F_(, , &&)                                \
  F_(const, , &&)                           \
  F_(, volatile, &&)                        \
  F_(const, volatile, &&)

// Helper macro for switch-case
# if ( EMBED_FN_NEED_FAST_CALL == false )
#  define EMBED_FN_SWITCH_EXPECT(cond,val) \
  EMBED_SWITCH_EXPECT(cond,val)
# else
#  define EMBED_FN_SWITCH_EXPECT(cond,val) (cond)
# endif

namespace embed EMBED_ABI_VISIBILITY(default)
{
  // declare ahead
  template <typename Signature, std::size_t BufSize>
  class Fn;

namespace detail {

#if defined(EMBED_NO_STD_HEADER)
  /**
   *  @brief Exception class thrown when class template function's
   *  operator() is called with an empty target.
   *  @ingroup exceptions
   */
  struct bad_function_call : public std::exception
  {
    bad_function_call()                                     = default;
    bad_function_call(const bad_function_call&)             = default;
    bad_function_call& operator=(const bad_function_call&)  = default;
    ~bad_function_call() override                           = default;

    const char* what() const noexcept override
    {
      return "embed::Fn::operator() is called with an empty target";
    };
  };
#else
  using std::bad_function_call;
#endif

  template <bool Has_Throw>
  [[noreturn]] inline typename std::enable_if<!Has_Throw>::type
  throw_bad_function_call_or_abort_impl() noexcept {
    bad_function_call_handler();
  }
  template <bool Has_Throw>
  [[noreturn]] inline typename std::enable_if<Has_Throw>::type
  throw_bad_function_call_or_abort_impl() {
#if EMBED_CXX_ENABLE_EXCEPTION
    throw bad_function_call();
#endif
  }

  /// @c throw_bad_function_call_or_abort
  // For private use only.
  [[noreturn]] inline void throw_bad_function_call_or_abort()
    noexcept(!EMBED_CXX_ENABLE_EXCEPTION || EMBED_FN_ENSURE_NO_THROW)
  {
    throw_bad_function_call_or_abort_impl<
      EMBED_CXX_ENABLE_EXCEPTION && (!EMBED_FN_ENSURE_NO_THROW)
    >();
    EMBED_UNREACHABLE(); // Unreachable
  }

  /// @c FnBufType
  /// @brief Anchor the size of the objects that the embed::Fn can store.
  template <std::size_t BufSize>
  union FnBufType
  {
    void*       vPtr;
    const void* cvPtr;
    void (* fPtr) ();

    static_assert(BufSize > 0, "embed::Fn requires the BufSize greater than 0");
    char        buf[BufSize];
  };

  /// @c EMBED_LAUNDER(x)
# ifndef EMBED_LAUNDER
#  if ( EMBED_CXX_VERSION >= 201703L ) && !defined(EMBED_NO_STD_HEADER)
#   define EMBED_LAUNDER(x) ( ::std::launder(x) )
#  elif EMBED_HAS_BUILTIN(__builtin_launder)
  template <typename T>
  EMBED_NODISCARD EMBED_INLINE constexpr T* launder(T* ptr) noexcept {
    return __builtin_launder(ptr);
  }
#   define EMBED_LAUNDER(x) ( ::embed::detail::launder(x) )
#  else
#   define EMBED_LAUNDER(x) (x)
#  endif
# endif

  /**
   * @c FnFunctor
   * @note FnFunctor is trivial.
   * 
   * The well-defined operation of reusing its storage space is to use
   * placement new. After that, using M_access to obtain the address or reference
   * (rather than the content) is also in accordance with the C++ standard.
   * See https://eel.is/c++draft/basic.life#7
   * 
   * This EMBED_ALIAS serves only as an auxiliary function.
   * It prevents the compiler from performing aggressive optimizations.
   * Even if the EMBED_ALIAS does not exist, the reuse of the FnFunctor's
   * memory and the subsequent use of M_access for access are well-defined.
   */
  template <std::size_t BufSize>
  union EMBED_ALIAS FnFunctor
  {
    // non-volatile
    EMBED_INLINE void* M_access() noexcept { return &M_pod_data[0]; }
    EMBED_INLINE const void* M_access() const noexcept { return &M_pod_data[0]; }

    // volatile
    EMBED_INLINE volatile void* M_access() volatile noexcept
    { return &M_pod_data[0]; }
    EMBED_INLINE const volatile void* M_access() volatile const noexcept
    { return &M_pod_data[0]; }

    // non-volatile
    template <typename T>
    EMBED_INLINE T& M_access() noexcept
    { return *EMBED_LAUNDER( static_cast<T*>(M_access()) ); }
    template <typename T>
    EMBED_INLINE const T& M_access() const noexcept
    { return *EMBED_LAUNDER( static_cast<const T*>(M_access()) ); }

    // volatile
    template <typename T>
    EMBED_INLINE volatile T& M_access() volatile noexcept
    { return *EMBED_LAUNDER( static_cast<volatile T*>(M_access()) ); }
    template <typename T>
    EMBED_INLINE const volatile T& M_access() volatile const noexcept
    { return *EMBED_LAUNDER( static_cast<const volatile T*>(M_access()) ); }

    FnBufType<BufSize>    M_unused;
    char                  M_pod_data[sizeof(FnBufType<BufSize>)];
  };

  /// @c FnToolBox
  /// @brief Tool Box for embed::Fn
  class FnToolBox
  {
  public:

    // cooperate with M_manager in FnManagerCopyable
    enum FunctorManagerOpCode {
      OP_clone_functor,     // Clone the M_functor
      OP_move_functor,      // Move the M_functor
      OP_destroy_functor,   // Destroy the M_functor

#if ( EMBED_FN_NEED_FAST_CALL == false )
      // M_invoker is not a member of embed::Fn in this mode,
      // so use OP_get_invoker to get it.
      OP_get_invoker,
#endif
    };

    // embed::Fn need some type traits that don't exist in C++11.
    /// @c FnTraits is aimed to provide these traits.
    struct FnTraits;

    /// @c FnInvoker is aimed to help Fn call the functor.
    /// When @b EMBED_FN_NEED_FAST_CALL is false, 
    /// @c FnManagerCopyable, @c FnManagerMoveOnly
    /// will help Fn call the functor instead of FnInvoker,
    /// which will save the RAM, but is slower than FnInvoker.
    template <typename Signature, typename Functor,
      std::size_t BufSize, bool Is_volatile, bool Is_rref>
    struct FnInvoker;

    // embed::Fn will forget the type of Functor
    /// @c FnManagerCopyable is aimed to remember the type, 
    /// help Fn manage copyable functor.
    template <typename Signature, typename Functor,
      std::size_t BufSize, bool Is_volatile, bool Is_rref>
    struct FnManagerCopyable;

    // embed::Fn need to wrap non-copyable callable object.
    /// @c FnManagerMoveOnly is aimed to help Fn manage move-only functor.
    template <typename Signature, typename Functor,
      std::size_t BufSize, bool Is_volatile, bool Is_rref>
    struct FnManagerMoveOnly;

    template <typename Signature, typename Functor,
      std::size_t BufSize, bool Is_volatile, bool Is_rref>
    struct FnManagerHelper;
  };


  /**
   * @c FnToolBox::FnTraits
   * @brief Provide necessary type traits.
   */
  struct FnToolBox::FnTraits
  {
    /// @e aligned_buf_size
    template <std::size_t BufSize>
    struct aligned_buf_size
    {
      // round up to an integer
      static constexpr std::size_t value = (BufSize == 0) ? sizeof(void*) :
        ((BufSize - 1) / sizeof(void*) + 1) * sizeof(void*);
    };

    // The behavior of `void_t` is incorrect in the outdated GCC compiler.
    // So we use `make_void` to correct the behavior of `void_t`.
    // See https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1558
    template <typename T> struct make_void { using type = void; };
    template <typename T> using void_t = typename make_void<T>::type;

    /// @e remove_cvref_t for C++11
    template <typename T>
    using remove_cvref_t = typename std::remove_cv<
      typename std::remove_reference<T>::type
    >::type;

    /// @brief trigger the SFINAE
    class failure_type {};

    /// @brief The call tag. Used by `invoke_result` and `invoke_impl`.
    // free_func, static_memfunc, lambda, std::reference_wrapper<R Class::*>, ...
    class invoke_tag_normal {};

    // Class&, std::reference_wrapper<Class>, ...
    class invoke_tag_memfn_ref_like {};
    // Class*, std::unique_ptr<Class>, std::reference_wrapper<Class*>, ...
    class invoke_tag_memfn_pointer_like {};

    // Class&, std::reference_wrapper<Class>, ...
    class invoke_tag_memobj_ref_like {};
    // Class*, std::unique_ptr<Class>, std::reference_wrapper<Class*>, ...
    class invoke_tag_memobj_pointer_like {};

    /// @e inv_unwrap
    template <typename T, typename U = remove_cvref_t<T>>
    struct inv_unwrap { using type = T; };

#if !defined(EMBED_NO_STD_HEADER)
    template <typename T, typename RealType>
    struct inv_unwrap<T, std::reference_wrapper<RealType>>
    { using type = RealType&; };
#else
    // Users can overload the inv_unwrap function
    // for the custom reference_wrapper here.
#endif

    template <typename T>
    using inv_unwrap_t = typename inv_unwrap<T>::type;

    /// @brief bind T and tag
    template <typename T, typename Tag>
    struct type_and_tag_wrapper
    {
      using type = T;
      using tag = Tag;
    };

    template <typename MemObj, typename Arg>
    struct invoke_result_of_memobj_ref_like_helper
    {
      template<typename> static failure_type S_test(...) { return {}; }
      template<typename T> static type_and_tag_wrapper<
        /* type = */ decltype(std::declval<T>().*std::declval<MemObj>()),
        /* tag = */ invoke_tag_memobj_ref_like
      > S_test(int) { return {}; }

      using type = decltype(S_test<Arg>(0));
    };

    template <typename MemObj, typename Arg>
    struct invoke_result_of_memobj_pointer_like_helper
    {
      template<typename> static failure_type S_test(...) { return {}; }
      template<typename T> static type_and_tag_wrapper<
        /* type = */ decltype((*std::declval<T>()).*std::declval<MemObj>()),
        /* tag = */ invoke_tag_memobj_pointer_like
      > S_test(int) { return {}; }

      using type = decltype(S_test<Arg>(0));
    };

    /// @e invoke_result_of_memobj
    template <typename T, typename U>
    struct invoke_result_of_memobj;  // Undefined

    template <typename Class, typename RetT, typename Arg>
    struct invoke_result_of_memobj<RetT Class::*, Arg>
    {
      using MemberObj = RetT Class::*;
      using ThisClass = remove_cvref_t<Arg>;

      using type = typename std::conditional<
        (std::is_same<Class, ThisClass>::value || std::is_base_of<Class, ThisClass>::value),
        typename invoke_result_of_memobj_ref_like_helper<MemberObj, Arg>::type,
        typename invoke_result_of_memobj_pointer_like_helper<MemberObj, Arg>::type
      >::type;
    };

    template <typename MemFunc, typename Arg, typename... ArgsType>
    struct invoke_result_of_memfunc_ref_like_helper
    {
      template<typename> static failure_type S_test(...) { return {}; }
      template<typename T> static type_and_tag_wrapper<
        /* type = */ decltype((std::declval<T>().*std::declval<MemFunc>())(
          std::declval<ArgsType>()...
        )),
        /* tag = */ invoke_tag_memfn_ref_like
      > S_test(int) { return {}; }

      using type = decltype(S_test<Arg>(0));
    };

    template <typename MemFunc, typename Arg, typename... ArgsType>
    struct invoke_result_of_memfunc_pointer_like_helper
    {
      template<typename> static failure_type S_test(...) { return {}; }
      template<typename T> static type_and_tag_wrapper<
        /* type = */ decltype(((*std::declval<T>()).*std::declval<MemFunc>())(
          std::declval<ArgsType>()...
        )),
        /* tag = */ invoke_tag_memfn_pointer_like
      > S_test(int) { return {}; }

      using type = decltype(S_test<Arg>(0));
    };

    /// @e invoke_result_of_memfunc
    template <typename... T>
    struct invoke_result_of_memfunc;  // Undefined

    template <typename Class, typename RetT, typename Arg, typename... ArgsType>
    struct invoke_result_of_memfunc<RetT Class::*, Arg, ArgsType...>
    {
      using MemberFunc = RetT Class::*;
      using ThisClass = remove_cvref_t<Arg>;

      using type = typename std::conditional<
        (std::is_same<Class, ThisClass>::value || std::is_base_of<Class, ThisClass>::value),
        typename invoke_result_of_memfunc_ref_like_helper<MemberFunc, Arg, ArgsType...>::type,
        typename invoke_result_of_memfunc_pointer_like_helper<MemberFunc, Arg, ArgsType...>::type
      >::type;
    };

    /// @e invoke_result_of_normal
    template <typename Functor, typename... ArgsType>
    struct invoke_result_of_normal
    {
      template<typename> static failure_type S_test(...) { return {}; }
      template<typename T> static type_and_tag_wrapper<
        /* type = */ decltype(std::declval<T>()(
          std::declval<ArgsType>()...)),
        /* tag = */ invoke_tag_normal
      > S_test(int) { return {}; }

      using type = decltype(S_test<Functor>(0));
    };

    /// @e invoke_result_impl
    template <bool, bool, typename Functor, typename... ArgsType>
    struct invoke_result_impl
    {
      using type = failure_type;  // failure_type::type will trigger SFINAE
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
    }; // member data

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
    }; // member function

    template <typename NormalFunc, typename... ArgsType>
    struct invoke_result_impl<
      /* is_memfunc_ptr = */ false,
      /* is_memobj_ptr = */ false,
      NormalFunc, ArgsType...
    > {
      using type = typename invoke_result_of_normal<
        NormalFunc, ArgsType...
      >::type;
    }; // normal function

    /// @brief Get the invoke result and invoke tag. Same as std::invoke_result.
    template <typename Func, typename... ArgsT>
    struct invoke_result : public invoke_result_impl<
      std::is_member_function_pointer<
        typename std::remove_reference<Func>::type
      >::value,  // first -> is_member_func
      std::is_member_object_pointer<
        typename std::remove_reference<Func>::type
      >::value,  // second -> is_member_obj
      Func, ArgsT...
    >::type {};

    template <typename To, typename From>
    struct reference_converts_from_temporary
    {
      using no_cvref_To = remove_cvref_t<To>;

      // prvalue(pure right value) is being bound to const lvalue(left value)
      static constexpr bool pr_to_l = std::is_lvalue_reference<To>::value
        && !std::is_reference<From>::value
        && std::is_const<typename std::remove_reference<To>::type>::value
        && std::is_convertible<From, no_cvref_To>::value;

      // prvalue is being bound to [const] xvalue(right reference)
      static constexpr bool pr_to_x = std::is_rvalue_reference<To>::value
        && !std::is_reference<From>::value
        && std::is_convertible<From, no_cvref_To>::value;

      static constexpr bool value = pr_to_l || pr_to_x;
    };

    // check Result::type --> RetT
    template <typename Result, typename RetT,
      bool = std::is_void<RetT>::value, typename = void>
    struct is_invocable_impl : public std::false_type
    {
      using noThrow = std::false_type;
    };

    template <typename Result, typename RetT>
    struct is_invocable_impl<Result, RetT,
      /* is_void<RetT> = */ true, void_t<typename Result::type> >
    : public std::true_type
    {
      using noThrow = std::true_type;
    };

#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
# pragma GCC diagnostic ignored "-Wreturn-type"
#endif
    template <typename Result, typename RetT>
    struct is_invocable_impl<Result, RetT,
      /* is_void<RetT> = */ false, void_t<typename Result::type> >
    {
      // INVOKE expression
      using Res_T = typename Result::type;

      // Unlike declval, this doesn't add_rvalue_reference, so it respects
      // guaranteed copy elision.
      static Res_T S_get() noexcept { return std::declval<Res_T>(); };

      // Used to check if Res_T can implicitly convert to T.
      template <typename T>
      static void S_conv(T) noexcept {};

      // Viable only when INVOKE(f, Args...) can convert to `RetT`.
      template <typename T,
        bool NoThrow_conv = noexcept(S_conv<T>(S_get())),
        typename = decltype(S_conv<T>(S_get())),
#if EMBED_HAS_BUILTIN(__reference_converts_from_temporary)
        bool Dangle = __reference_converts_from_temporary(T, Res_T)
#else
        bool Dangle = reference_converts_from_temporary<T, Res_T>::value
#endif
      >
      static typename std::integral_constant<
        bool, NoThrow_conv && !Dangle
      >::type
      S_test(int) noexcept
      {
        return std::integral_constant<bool, NoThrow_conv && !Dangle>();
      }

      template <typename T, bool = false>
      static std::false_type S_test(...) noexcept { return {}; };

      using type = decltype(S_test<RetT, true>(1));
      using noThrow = decltype(S_test<RetT>(1));
    };
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

    /// @e Callable
    template <typename RetT, typename Functor, typename... ArgsT>
    struct Callable
    : public std::integral_constant<bool,
      is_invocable_impl<
        invoke_result<typename std::decay<Functor>::type&, ArgsT...>, RetT
      >::type::value
      || is_invocable_impl<
        invoke_result<typename std::decay<Functor>::type&&, ArgsT...>, RetT
      >::type::value
    > {
      using Caller_lref = typename std::decay<Functor>::type&;
      using Caller_rref = typename std::decay<Functor>::type&&;

      using Result_lref = invoke_result<Caller_lref, ArgsT...>;
      using Result_rref = invoke_result<Caller_rref, ArgsT...>;

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L ) \
    && (!EMBED_CXX_ENABLE_EXCEPTION)
      // The noexcept-specification is a part of the function type 
      // and may appear as part of any function declarator.
      // And only when `EMBED_CXX_ENABLE_EXCEPTION` is false,
      // embed::Fn::operator() can be `noexcept`.

      template<typename>
      static std::false_type S_test(...) noexcept { return {}; }

      template<typename Caller_>
      static typename std::enable_if<
        noexcept(std::declval<Caller_>()(std::declval<ArgsT>()...)),
        std::true_type
      >::type S_test(int) noexcept { return {}; }

      using NoThrow_call = typename std::integral_constant<
        bool, decltype(S_test<Caller_lref>(0))::value
        || decltype(S_test<Caller_rref>(0))::value
      >::type;
#else
      using NoThrow_call = std::true_type;
#endif
      using NoThrow_conv = typename std::integral_constant<
        bool, is_invocable_impl<Result_lref, RetT>::noThrow::value
        || is_invocable_impl<Result_rref, RetT>::noThrow::value
      >::type;

      static constexpr bool NoThrow_v = NoThrow_call::value && NoThrow_conv::value;

#if ( EMBED_FN_NOTHROW_CALLABLE == true )

      static_assert(NoThrow_v, "embed::Fn requires no-throw callable functor");

#endif
    };

    /// @e results_are_same
    template <typename RetFrom, typename RetTo>
    struct results_are_same
    {
      static constexpr bool value = std::is_same<RetFrom, RetTo>::value;
    };

    /// @e args_package
    template <typename... ArgsT>
    struct args_package
    { };

    template <std::size_t Index, typename Package>
    struct unwrap_package
    {
      // always error if this struct is being used.
      static_assert(Index != 0 && false, "unwrap_package out of range");
    };

    template <typename T, typename... Res>
    struct unwrap_package<0, args_package<T, Res...>>
    {
      using type = T;
    };

    template <std::size_t Index, typename T, typename... Res>
    struct unwrap_package<Index, args_package<T, Res...>>
    {
      using type = typename unwrap_package<Index-1, args_package<Res...>>::type;
    };

    /// @e arguments_are_same_impl
    /// @brief check if arguments are all same.
    template <typename ArgsPackageFrom, typename ArgsPackageTo, std::size_t Idx>
    struct arguments_are_same_impl
    {
      using FromType = typename unwrap_package<Idx, ArgsPackageFrom>::type;
      using ToType = typename unwrap_package<Idx, ArgsPackageTo>::type;

      static constexpr bool value = std::is_same<
        FromType, ToType  
      >::value && arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, Idx-1>::value;
    };

    template <typename ArgsPackageFrom, typename ArgsPackageTo>
    struct arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, 0>
    {
      using FromType = typename unwrap_package<0, ArgsPackageFrom>::type;
      using ToType = typename unwrap_package<0, ArgsPackageTo>::type;

      static constexpr bool value = std::is_same<
        FromType, ToType
      >::value;
    };

    template <typename ArgsPackageFrom, typename ArgsPackageTo>
    struct arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, static_cast<std::size_t>(-1)>
    { static constexpr bool value = false; };

    /// @e arguments_are_same
    template <typename ArgsPackageFrom, typename ArgsPackageTo, std::size_t ArgNum>
    struct arguments_are_same : public std::conditional<
      ArgNum == 0, std::true_type,
      arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, ArgNum-1>
    >::type { };

    /**
     * @e invoke_impl
     * @brief Distribute the call of normal function,
     * member function and member object.
     */
    template <typename RetT, typename Func, typename... Args>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR RetT
    invoke_impl(invoke_tag_normal, Func&& fn, Args&&... args)
      noexcept(noexcept(std::forward<Func>(fn)(std::forward<Args>(args)...)))
    { return std::forward<Func>(fn)(std::forward<Args>(args)...); }

    template <typename RetT, typename MemObj, typename Arg>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR RetT
    invoke_impl(invoke_tag_memobj_ref_like, MemObj&& obj, Arg&& arg)
      noexcept(noexcept(static_cast<inv_unwrap_t<Arg>&&>(arg).*std::forward<MemObj>(obj)))
    { return static_cast<inv_unwrap_t<Arg>&&>(arg).*std::forward<MemObj>(obj); }

    template <typename RetT, typename MemObj, typename Arg>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR RetT
    invoke_impl(invoke_tag_memobj_pointer_like, MemObj&& obj, Arg&& arg)
      noexcept(noexcept((*std::forward<Arg>(arg)).*std::forward<MemObj>(obj)))
    { return (*std::forward<Arg>(arg)).*std::forward<MemObj>(obj); }

    template <typename RetT, typename MemFunc, typename Arg, typename... ArgsType>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR RetT
    invoke_impl(invoke_tag_memfn_ref_like, MemFunc&& memfn, Arg&& arg, ArgsType&&... args)
      noexcept(noexcept(
        (static_cast<inv_unwrap_t<Arg>&&>(arg).*std::forward<MemFunc>(memfn))(
          std::forward<ArgsType>(args)...)))
    {
      return (static_cast<inv_unwrap_t<Arg>&&>(arg).*std::forward<MemFunc>(memfn))(
        std::forward<ArgsType>(args)...
      );
    }

    template <typename RetT, typename MemFunc, typename Arg, typename... ArgsType>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR RetT
    invoke_impl(invoke_tag_memfn_pointer_like, MemFunc&& memfn, Arg&& arg, ArgsType&&... args)
      noexcept(noexcept(
        ((*std::forward<Arg>(arg)).*std::forward<MemFunc>(memfn))(
          std::forward<ArgsType>(args)...)))
    {
      return ((*std::forward<Arg>(arg)).*std::forward<MemFunc>(memfn))(
        std::forward<ArgsType>(args)...
      );
    }

    /// @e invoke_r
    template <typename RetType, typename Callee, typename... ArgsType>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR
    typename std::enable_if<!std::is_void<RetType>::value, RetType>::type
    invoke_r(Callee&& fn, ArgsType&&... args)
      noexcept(noexcept(
        invoke_impl<typename invoke_result<Callee, ArgsType...>::type>(
          typename invoke_result<Callee, ArgsType...>::tag{}, 
          std::forward<Callee>(fn), std::forward<ArgsType>(args)...))
      && Callable<RetType, Callee, ArgsType...>::NoThrow_conv::value)
    {
      // The return type is not `void`.
      using result  = invoke_result<Callee, ArgsType...>;
      using type    = typename result::type;
      using tag     = typename result::tag;

      return invoke_impl<type>(tag{}, std::forward<Callee>(fn), std::forward<ArgsType>(args)...);
    }

    template <typename RetType, typename Callee, typename... ArgsType>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR
    typename std::enable_if<std::is_void<RetType>::value>::type
    invoke_r(Callee&& fn, ArgsType&&... args)
      noexcept(noexcept(
        invoke_impl<typename invoke_result<Callee, ArgsType...>::type>(
          typename invoke_result<Callee, ArgsType...>::tag{}, 
          std::forward<Callee>(fn), std::forward<ArgsType>(args)...)))
    {
      // The return type is `void`.
      using result  = invoke_result<Callee, ArgsType...>;
      using type    = typename result::type;
      using tag     = typename result::tag;

      invoke_impl<type>(tag{}, std::forward<Callee>(fn), std::forward<ArgsType>(args)...);
    }

    /// @e is_similar_Fn
    /// @brief check if two Fn are similar.
    /// "Similar" means the function signature is the same, but may have different buffer size.
    template <
      typename SelfRet, typename SelfArgs_package, std::size_t SelfArgNum, std::size_t SelfBuf,
      typename OtherRet, typename OtherArgs_package, std::size_t OtherArgNum, std::size_t OtherBuf>
    struct is_similar_Fn
    {
      static constexpr bool value = 
        ( aligned_buf_size<SelfBuf>::value == aligned_buf_size<OtherBuf>::value )
        && results_are_same<OtherRet, SelfRet>::value
        && ( SelfArgNum == OtherArgNum )
        && arguments_are_same<SelfArgs_package, OtherArgs_package, SelfArgNum>::value;
    };

    /// @e get_unique_call_signature_impl
    template <typename Signature>
    struct get_unique_call_signature_impl;

#define EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_IMPL(C, V, REF, NOEXC)\
    template <typename Ret, typename Functor, typename... ArgsType>\
    struct get_unique_call_signature_impl<Ret (Functor::*) (ArgsType...) C V REF NOEXC>\
    { using type = Ret(ArgsType...) C V REF; };

#define EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_CVREF(C, V, REF)\
    EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_IMPL(C, V, REF, )

    // Overload the `get_unique_call_signature_impl` with different modifiers.
    // (const / volatile / {& | &&})
    EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_CVREF)

#undef EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_CVREF

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )

    // See https://en.cppreference.com/w/cpp/language/noexcept_spec
    // The noexcept-specification is a part of the function type and 
    // may appear as part of any function declarator. (Since C++17)

# define EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_CVREF(C, V, REF)\
    EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_IMPL(C, V, REF, noexcept)

    // Overload the `get_unique_call_signature_impl` with different modifiers.
    // (const / volatile / {& | &&} + noexcept)
    EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_CVREF)

# undef EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_CVREF

#endif

#undef EMBED_FN_OVERLOAD_UNIQUE_CALL_SIGNATURE_IMPL

#if ( __cpp_explicit_this_parameter >= 202110L ) || ( EMBED_CXX_VERSION >= 202302L )

    // 3617. function/packaged_task deduction guides and deducing this.
    // See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html

    template <typename ThisType, typename Ret, typename... ArgsType>
    struct get_unique_call_signature_impl<Ret (*) (ThisType, ArgsType...)>
    { using type = Ret(ArgsType...); };

    template <typename ThisType, typename Ret, typename... ArgsType>
    struct get_unique_call_signature_impl<Ret (*) (ThisType, ArgsType...) noexcept>
    { using type = Ret(ArgsType...); };

#endif

    /// @e is_unique_callable
    template <typename Functor, bool = true, typename = void>
    struct is_unique_callable
    : public std::false_type
    { };

    template <typename Functor>
    struct is_unique_callable<Functor, true, void_t<decltype(&Functor::operator())>>
    : public std::true_type
    { };

    /// @e get_unique_call_signature
    template <typename Functor, bool = is_unique_callable<Functor>::value>
    struct get_unique_call_signature
    {
      using type = void; // unused
    };

    template <typename Functor>
    struct get_unique_call_signature<Functor, true>
    {
      using type = typename
        get_unique_call_signature_impl<decltype(&Functor::operator())>::type;
    };

    /// @e unwrap_signature
    template <typename Signature> struct unwrap_signature
    {
      static_assert(!std::is_void<void_t<Signature>>::value, /* always false */
        "The Signature must be like `Ret(Args...) [const | volatile | & | &&]`."
        " And your signature format is incorrect.");
    };

#define EMBED_FN_OVERLOAD_UNWRAP_SIGNATURE(C, V, REF)               \
    template <typename RetType, typename... ArgsType>               \
    struct unwrap_signature<RetType(ArgsType...) C V REF> {         \
      using ret = RetType;                                          \
      using args = args_package<ArgsType...>;                       \
      static constexpr std::size_t arg_num = sizeof... (ArgsType);  \
      using pure_sig = RetType(ArgsType...);                        \
    };

    // unwrap_signature for different kinds of signature.(const / volatile / {& | &&})
    EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_OVERLOAD_UNWRAP_SIGNATURE)

#undef EMBED_FN_OVERLOAD_UNWRAP_SIGNATURE

    // get_signature_qualifier
    template <typename Signature>
    struct get_signature_qualifier;

#define EMBED_FN_OVERLOAD_GET_SIGNATURE_QUALIFIER(C, V, REF)                    \
    template <typename Ret, typename... Args>                                   \
    struct get_signature_qualifier<Ret(Args...) C V REF>                        \
    {                                                                           \
      static constexpr bool is_const = std::is_const<int C>::value;             \
      static constexpr bool is_volatile = std::is_volatile<int V>::value;       \
      static constexpr bool is_lref = std::is_lvalue_reference<int REF>::value; \
      static constexpr bool is_rref = std::is_lvalue_reference<int REF>::value; \
    };

    // Use the macro to generate code for different kinds of signature.
    // (const / volatile / {& | &&})
    EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_OVERLOAD_GET_SIGNATURE_QUALIFIER)

#undef EMBED_FN_OVERLOAD_GET_SIGNATURE_QUALIFIER

    // qualifier_conv_safe
    /**
     * +------------------+------------------+--------+
     * |       from       |        to        |  safe  |
     * +------------------+------------------+--------+
     * |    non-const     |       const      |   No   |
     * |      const       |    non-const     |   Yes  |
     * |   non-volatile   |      volatile    |   Yes  |
     * |      volatile    |   non-volatile   |   No   |
     * |       non-&      |          &       |   Yes  |
     * |          &       |       non-&      |   No   |
     * |      non-&&      |         &&       |   Yes  |
     * |         &&       |       non-&&     |   No   |
     * +------------------+------------------+--------+
     */
    template <typename SigFrom, typename SigTo>
    struct qualifier_conv_safe
    {
      static constexpr bool value = 
        !(
      (!get_signature_qualifier<SigFrom>::is_const && get_signature_qualifier<SigTo>::is_const)
    ||(get_signature_qualifier<SigFrom>::is_volatile && !get_signature_qualifier<SigTo>::is_volatile)
    ||(get_signature_qualifier<SigFrom>::is_lref && !get_signature_qualifier<SigTo>::is_lref)
    ||(get_signature_qualifier<SigFrom>::is_rref && !get_signature_qualifier<SigTo>::is_rref)
        );
    };


    // is_similar_Fn_signature
    template <typename SelfSig, typename OtherSig, std::size_t SelfSize, std::size_t OtherSize>
    struct is_similar_Fn_signature
    {
      using SelfRet     = typename unwrap_signature<SelfSig>::ret;
      using SelfArgs    = typename unwrap_signature<SelfSig>::args;
      static constexpr std::size_t SelfArgNum = unwrap_signature<SelfSig>::arg_num;
      using OtherRet     = typename unwrap_signature<OtherSig>::ret;
      using OtherArgs    = typename unwrap_signature<OtherSig>::args;
      static constexpr std::size_t OtherArgNum = unwrap_signature<OtherSig>::arg_num;

      static constexpr bool value = 
        is_similar_Fn<
          SelfRet, SelfArgs, SelfArgNum, SelfSize,
          OtherRet, OtherArgs, OtherArgNum, OtherSize
        >::value 
        && qualifier_conv_safe<OtherSig, SelfSig>::value;
    };

    /// @e is_Fn_and_similar
    template <typename Signature, typename Functor>
    struct is_Fn_and_similar
    : public std::false_type { };

    template <typename Signature,
      typename OtherSignature, std::size_t BufSize>
    struct is_Fn_and_similar<Signature, Fn<OtherSignature, BufSize>>
    {
      static constexpr bool value = is_similar_Fn_signature<
          Signature, OtherSignature, BufSize, BufSize
        >::value;
    };

    /// @e is_movable_and_non_copyable
    template <typename Functor>
    struct is_movable_and_non_copyable
    {
      using Func = remove_cvref_t<Functor>;
      static constexpr bool value = std::is_move_constructible<Func>::value
        && !std::is_copy_constructible<Func>::value;
    };

    /// @e enableIf_movable_and_non_copyable_t
    template <typename Functor>
    using enableIf_movable_and_non_copyable_t = typename std::enable_if<
      is_movable_and_non_copyable<Functor>::value
    >::type;

    /// @e disableIf_movable_and_non_copyable_and_nref_t
    template <typename Functor>
    using disableIf_movable_and_non_copyable_and_nref_t = typename std::enable_if<
      !(is_movable_and_non_copyable<
          typename std::decay<Functor>::type
        >::value && !std::is_reference<Functor>::value)
    >::type;

    /// @e is_class_and_has_call_operator_helper
    template <typename Functor, typename... Args>
    struct is_class_and_has_call_operator_helper
    {
      template <typename>
      static std::false_type S_test_const(...) { return {}; }
      template <typename Func>
      static typename std::enable_if<
        std::is_void<void_t<
          decltype(std::declval<Func const>()(std::declval<Args>()...))
        >>::value, std::true_type
      >::type S_test_const(int) { return {}; }

      static constexpr bool is_const_callable = decltype(S_test_const<Functor>(0))::value;

      template <typename>
      static std::false_type S_test_rref(...) { return {}; }
      template <typename Func>
      static typename std::enable_if<
        std::is_void<void_t<
          decltype(std::declval<Func &&>()(std::declval<Args>()...))
        >>::value, std::true_type
      >::type S_test_rref(int) { return {}; }

      static constexpr bool is_rref_callable = decltype(S_test_rref<Functor>(0))::value;

      template <typename>
      static std::false_type S_test_normal(...) { return {}; }
      template <typename Func>
      static typename std::enable_if<
        std::is_void<void_t<
          decltype(std::declval<Func &>()(std::declval<Args>()...))
        >>::value, std::true_type
      >::type S_test_normal(int) { return {}; }

      static constexpr bool is_normal_callable = decltype(S_test_normal<Functor>(0))::value;
    };

    /// @e is_class_and_has_call_operator
    template <typename PureSig, typename Functor>
    struct is_class_and_has_call_operator;

    template <typename Ret, typename Functor, typename... Args>
    struct is_class_and_has_call_operator<Ret(Args...), Functor>
    : private is_class_and_has_call_operator_helper<
      typename std::decay<Functor>::type, Args...>
    {
    private:
      using Base = is_class_and_has_call_operator_helper<Functor, Args...>;
    public:
      static constexpr bool is_class = 
        std::is_class<typename std::decay<Functor>::type>::value;

      static constexpr bool value = 
        is_class && (
          Base::is_const_callable || Base::is_rref_callable || Base::is_normal_callable
        );

      static constexpr bool is_const_q = is_class && Base::is_const_callable;
      static constexpr bool is_rref_q = is_class && Base::is_rref_callable;
      static constexpr bool not_const_q = is_class && (!Base::is_const_callable);
      static constexpr bool not_rref_q = is_class && (!Base::is_rref_callable);
    };

  }; // end FnTraits


  /**
   * @c FnToolBox::FnInvoker
   * @brief Invoke the functor for Fn.
   */
  template <typename RetType, typename Functor, std::size_t BufSize, 
    bool Is_volatile, bool Is_rref, typename... ArgsType>
  struct FnToolBox::FnInvoker<RetType(ArgsType...), Functor,
    BufSize, Is_volatile, Is_rref>
  {
  private:
    using FnFunctor_Qualifier = typename std::conditional<
      Is_volatile, volatile FnFunctor<BufSize>, FnFunctor<BufSize>
    >::type;
    using Func_Qualifier = typename std::conditional<
      Is_volatile, volatile Functor, Functor
    >::type;
    using FnFunctor_Cast = typename std::conditional<
      Is_rref, typename std::remove_reference<Functor>::type&&,
      typename std::remove_reference<Functor>::type&
    >::type;

    static EMBED_INLINE Functor*
    M_get_pointer(const FnFunctor_Qualifier& src) noexcept
    {
      const Func_Qualifier& fn = src.template M_access<Functor>();
      return const_cast<Functor*>(std::addressof(fn));
    }
  public:

    static RetType M_invoke(const FnFunctor_Qualifier& functor, ArgsType&&... args)
    EMBED_FN_CASE_NOEXCEPT
    {
      return FnTraits::invoke_r<RetType>(
        static_cast<FnFunctor_Cast>(*M_get_pointer(functor)),
        std::forward<ArgsType>(args)...);
    }

  };


  /**
   * @brief The Base of @c FnToolBox::FnManagerCopyable
   *                and @c FnToolBox::FnManagerMoveOnly
   */
  template <typename RetType, typename Functor, std::size_t BufSize,
    bool Is_volatile, bool Is_rref, typename... ArgsType>
  struct FnToolBox::FnManagerHelper<
    RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>
  {
  private:
    using FnFunctor_Qualifier = typename std::conditional<
      Is_volatile, volatile FnFunctor<BufSize>, FnFunctor<BufSize>
    >::type;
    using Func_Qualifier = typename std::conditional<
      Is_volatile, volatile Functor, Functor
    >::type;
    using FnFunctor_Cast = typename std::conditional<
      Is_rref, typename std::remove_reference<Functor>::type&&,
      typename std::remove_reference<Functor>::type&
    >::type;

  public:
    /// @e Invoker_Type (same as `Invoker_Type` in embed::Fn)
    using Invoker_Type = RetType (*) (const FnFunctor_Qualifier&, ArgsType&&...) EMBED_FN_CASE_NOEXCEPT;

  protected:
    /// @e M_create
    /// Q: Why not use Functor, but use the `Func`?
    /// A: Because this is Perfect Forwarding, so compiler need to
    /// deduce the type of `Func`, rather than explicitly specifying it.
    template <typename Func>
    static void M_create(FnFunctor_Qualifier& dest, Func&& functor) noexcept
    {
      ::new (const_cast<void*>(dest.M_access()))
        Functor(std::forward<Func>(functor));
    }

    /// @e M_destroy
    static void M_destroy(FnFunctor_Qualifier& victim) noexcept
    {
      victim.template M_access<Functor>().~Functor();
    }

  public:
    /**
     * @e M_get_pointer
     * @attention break the `const` promises.
     * @note SAFE: Underneath the `src` variable is `M_functor`, which is non-const,
     * and it's safe to use const_cast here. The non-const pointer obtained here
     * is not used to explicitly modify the object, except for side effects and
     * move operations when the callable object is invoked. The former provides
     * the same safety as `std::function`, and the move operation strictly checks
     * that the underlying object is non-const to ensure safety.
     */
    static Functor*
    M_get_pointer(const FnFunctor_Qualifier& src) noexcept
    {
      const Func_Qualifier& fn = src.template M_access<Functor>();
      return const_cast<Functor*>(std::addressof(fn));
    }

    /// @e M_not_empty_function
    template <typename Signature, std::size_t Size>
    static bool M_not_empty_function(const Fn<Signature, Size>& f) noexcept
    { return static_cast<bool>(f); }

    template <typename Signature, std::size_t Size>
    static bool M_not_empty_function(const volatile Fn<Signature, Size>& f) noexcept
    { return f.M_manager != nullptr; }

    template <typename T>
    static bool M_not_empty_function(T* fp) noexcept
    { return fp != nullptr; }

    template <typename Class, typename T>
    static bool M_not_empty_function(T Class::* mp) noexcept
    { return mp != nullptr; }

    template <typename T>
    static bool M_not_empty_function(const T&) noexcept
    { return true; }

    /// @e M_init_functor
    // init functor by using M_create (perfect forward)
    template <typename Func>
    static void M_init_functor(FnFunctor_Qualifier& dest, Func&& functor) noexcept
    {
      M_create(dest, std::forward<Func>(functor));
    }

#if ( EMBED_FN_NEED_FAST_CALL == false )

    /// @e M_invoke
    // This function only used when we expect FnManagerCopyable to help Fn
    // call the functor instead of FnInvoker.
    static RetType M_invoke(const FnFunctor_Qualifier& functor, ArgsType&&... args)
    noexcept(FnTraits::Callable<RetType, Functor, ArgsType...>::NoThrow_v)
    {
      return FnTraits::invoke_r<RetType>(
        static_cast<FnFunctor_Cast>(*M_get_pointer(functor)),
        std::forward<ArgsType>(args)...);
    }

#endif
  };


  /**
   * @c FnToolBox::FnManagerCopyable
   * @brief Manage the functor for embed::Fn.
   */
  template <typename RetType, typename Functor, std::size_t BufSize,
    bool Is_volatile, bool Is_rref, typename... ArgsType>
  struct FnToolBox::FnManagerCopyable<
    RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>
  : public FnToolBox::FnManagerHelper<
    RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>
  {
  public:
    constexpr static std::size_t M_max_size = sizeof(FnBufType<BufSize>);
    constexpr static std::size_t M_max_align = alignof(FnBufType<BufSize>);

    static constexpr bool noThrowExcept =
      std::is_nothrow_copy_constructible<Functor>::value
      && std::is_nothrow_destructible<Functor>::value;

    static constexpr bool smallAndAligned =
      sizeof(Functor) <= M_max_size
      && alignof(Functor) <= M_max_align
      && (sizeof(Functor) % alignof(Functor) == 0);

    /// @e Local_Storage
    /// @brief Reserved for future use.
    using Local_Storage = typename
    std::integral_constant<bool,
      noThrowExcept && smallAndAligned
    >::type; // end Local_Storage

    // MUST small and nothrow
    static_assert(noThrowExcept,
      "embed::Fn requires the functor to be nothrow copy-constructible"
      " and nothrow destructible");
    static_assert(smallAndAligned,
      "embed::Fn requires the functor to fit in `BufSize` and"
      " have valid alignment (adjust `BufSize` if needed)");

    using Base = FnManagerHelper<
      RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>;
    using FnFunctor_Qualifier = typename std::conditional<
      Is_volatile, volatile FnFunctor<BufSize>, FnFunctor<BufSize>
    >::type;

    /// @e M_manager
    // Core func to manager functor.
    static typename Base::Invoker_Type
    M_manager(
      FnFunctor_Qualifier& dest, 
      const FnFunctor_Qualifier& src, 
      FunctorManagerOpCode op
    ) EMBED_CXX17_NOEXCEPT {
      typename Base::Invoker_Type invoker = nullptr;

      switch (EMBED_FN_SWITCH_EXPECT(op, OP_get_invoker))
      {
      case OP_clone_functor:
        Base::M_init_functor(dest,
          *const_cast<const Functor*>(Base::M_get_pointer(src)));
        break;

      case OP_move_functor:
        Base::M_init_functor(dest, std::move( *(Base::M_get_pointer(src)) ));
        break;

      case OP_destroy_functor:
        Base::M_destroy(dest);
        break;

#if ( EMBED_FN_NEED_FAST_CALL == false )
      case OP_get_invoker:
        invoker = &Base::M_invoke;
        break;
#endif

      default: EMBED_UNREACHABLE(); /* Unreachable */ break;
      } // end switch

      return invoker;
    }

  };


  /**
   * @c FnToolBox::FnManagerMoveOnly
   * @brief Manager non-copyable objects.
   */
  template <typename RetType, typename Functor, std::size_t BufSize, 
    bool Is_volatile, bool Is_rref, typename... ArgsType>
  struct FnToolBox::FnManagerMoveOnly<
    RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>
  : public FnToolBox::FnManagerHelper<
    RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>
  {
  public:
    constexpr static std::size_t M_max_size = sizeof(FnBufType<BufSize>);
    constexpr static std::size_t M_max_align = alignof(FnBufType<BufSize>);

    static constexpr bool noThrowExcept =
      std::is_nothrow_move_constructible<Functor>::value
      && std::is_nothrow_destructible<Functor>::value;

    static constexpr bool smallAndAligned =
      sizeof(Functor) <= M_max_size
      && alignof(Functor) <= M_max_align
      && (sizeof(Functor) % alignof(Functor) == 0);

    /// @e Local_Storage
    /// @brief Reserved for future use.
    using Local_Storage = typename
    std::integral_constant<bool,
      noThrowExcept && smallAndAligned
    >::type; // end Local_Storage

    // MUST small and nothrow
    static_assert(noThrowExcept,
      "embed::Fn requires the functor to be nothrow move-constructible"
      " and nothrow destructible");
    static_assert(smallAndAligned,
      "embed::Fn requires the functor to fit in `BufSize` and"
      " have valid alignment (adjust `BufSize` if needed)");

    using Base = FnManagerHelper<
      RetType(ArgsType...), Functor, BufSize, Is_volatile, Is_rref>;
    using FnFunctor_Qualifier = typename std::conditional<
      Is_volatile, volatile FnFunctor<BufSize>, FnFunctor<BufSize>
    >::type;

    /// @e M_manager
    // Core func to manager functor.
    static typename Base::Invoker_Type
    M_manager(
      FnFunctor_Qualifier& dest, 
      const FnFunctor_Qualifier& src, 
      FunctorManagerOpCode op
    ) EMBED_CXX17_NOEXCEPT {
      typename Base::Invoker_Type invoker = nullptr;

      switch (EMBED_FN_SWITCH_EXPECT(op, OP_get_invoker))
      {
      case OP_clone_functor:
        /// @attention non-copyable object CANNOT clone!
        bad_function_copy_handler();
        EMBED_UNREACHABLE(); // Unreachable
        break;

      case OP_move_functor:
        Base::M_init_functor(dest, std::move( *(Base::M_get_pointer(src)) ));
        break;

      case OP_destroy_functor:
        Base::M_destroy(dest);
        break;

#if ( EMBED_FN_NEED_FAST_CALL == false )
      case OP_get_invoker:
        invoker = &Base::M_invoke;
        break;
#endif

      default: EMBED_UNREACHABLE(); /* Unreachable */ break;
      } // end switch

      return invoker;
    }
  };

#define EMBED_FN_MODIFIER_HELPER_MAIN_BODY(C, V, REF)                                     \
  template <typename Functor>                                                             \
  using Copyable = FnToolBox::FnManagerCopyable<RetType(ArgsType...),                     \
    Functor, BufSize, std::is_volatile<V int>::value,                                     \
    std::is_rvalue_reference<int REF>::value>;                                            \
  template <typename Functor>                                                             \
  using MoveOnly = FnToolBox::FnManagerMoveOnly<RetType(ArgsType...),                     \
    Functor, BufSize, std::is_volatile<V int>::value,                                     \
    std::is_rvalue_reference<int REF>::value>;                                            \
  template <typename Functor>                                                             \
  using Invoker = FnToolBox::FnInvoker<RetType(ArgsType...),                              \
    Functor, BufSize, std::is_volatile<V int>::value,                                     \
    std::is_rvalue_reference<int REF>::value>;                                            \
  template <typename Functor>                                                             \
  using Callable = FnToolBox::FnTraits::Callable<RetType, Functor, ArgsType...>;          \
  using Invoker_Type = RetType (*)                                                        \
    (const V FnFunctor<BufSize>&, ArgsType&&...) EMBED_FN_CASE_NOEXCEPT;                  \
  using Manager_Type = Invoker_Type (*) (V FnFunctor<BufSize>&,                           \
    const V FnFunctor<BufSize>&, FnToolBox::FunctorManagerOpCode) EMBED_CXX17_NOEXCEPT;

# if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wuninitialized"
# elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
# endif

#if ( EMBED_FN_NEED_FAST_CALL == true )
# define EMBED_FN_MODIFIER_HELPER_MEMVARS \
  FnFunctor<BufSize>  M_functor{};        \
  Manager_Type        M_manager{};        \
  Invoker_Type        M_invoker{};
# define EMBED_FN_MODIFIER_HELPER_INVOKE_BODY                     \
  if EMBED_LIKELY(M_invoker)                                      \
    return M_invoker(M_functor, std::forward<ArgsType>(args)...); \
  else                                                            \
    detail::throw_bad_function_call_or_abort(); /* may not throw exception */
#else
# define EMBED_FN_MODIFIER_HELPER_MEMVARS \
  FnFunctor<BufSize>  M_functor{};        \
  Manager_Type        M_manager{};
# define EMBED_FN_MODIFIER_HELPER_INVOKE_BODY                               \
  if EMBED_LIKELY(M_manager) {                                              \
    detail::FnFunctor<BufSize> nil;                                         \
    Invoker_Type invoker = M_manager(nil, nil, FnToolBox::OP_get_invoker);  \
    return invoker(M_functor, std::forward<ArgsType>(args)...);             \
  }                                                                         \
  else                                                                      \
    detail::throw_bad_function_call_or_abort(); /* may not throw exception */
#endif

  /**
   * @brief Help "embed::Fn" handle various different modifiers.
   */
  template <typename Signature, std::size_t>
  struct FnQualifierHelper
  {
    static_assert(
      !std::is_void<FnToolBox::FnTraits::void_t<Signature>>::value /* always false */,
      "The Signature must be like `Ret(Args...) [const | volatile | & | &&]`."
      " And your signature format is incorrect.");
  };

#define EMBED_FN_QUALIFIER_HELPER_CODE(C, V, REF)                         \
  template <typename RetType, std::size_t BufSize, typename... ArgsType>  \
  struct FnQualifierHelper<RetType(ArgsType...) C V REF, BufSize>         \
  {                                                                       \
    protected:                                                            \
    EMBED_FN_MODIFIER_HELPER_MAIN_BODY(C, V, REF)                         \
    EMBED_FN_MODIFIER_HELPER_MEMVARS                                      \
    public:                                                               \
    EMBED_INLINE RetType operator() (ArgsType... args) C V REF            \
    EMBED_FN_CASE_NOEXCEPT {                                              \
      EMBED_FN_MODIFIER_HELPER_INVOKE_BODY                                \
    }                                                                     \
  };

  // Use macro to generate code. (Overload for `FnQualifierHelper`)
  EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_QUALIFIER_HELPER_CODE)

# if defined(__clang__)
#  pragma clang diagnostic pop
# elif defined(__GNUC__)
#  pragma GCC diagnostic pop
# endif


#undef EMBED_FN_QUALIFIER_HELPER_CODE
#undef EMBED_FN_MODIFIER_HELPER_MEMVARS
#undef EMBED_FN_MODIFIER_HELPER_INVOKE_BODY
#undef EMBED_FN_MODIFIER_HELPER_MAIN_BODY

} // end namespace embed::detail

  /**
   * @brief   A light polymorphic wrapper for callable object.
   * @note    Only use stack memory. NO HEAP MEMORY!
   */
  // template <typename RetType, std::size_t BufSize, typename... ArgsType>
  template <typename Signature, std::size_t BufSize>
  class Fn
  : private detail::FnToolBox
  , public detail::FnQualifierHelper<Signature, BufSize>
  {
  private:
    using MyQualifierHelper = detail::FnQualifierHelper<Signature, BufSize>;

    template <typename Functor>
    using DecayFunc_t = typename std::enable_if<
      !std::is_same<Fn, FnTraits::remove_cvref_t<Functor> >::value,
      typename std::decay<Functor>::type
    >::type;

    template <typename Functor>
    using MyManager = typename MyQualifierHelper::template Copyable<Functor>;

    template <typename Functor>
    using MyMoveOnly = typename MyQualifierHelper::template MoveOnly<Functor>;

#if ( EMBED_FN_NEED_FAST_CALL == true )
    template <typename Functor>
    using MyInvoker = typename MyQualifierHelper::template Invoker<Functor>;
#endif

    using Invoker_Type = typename MyQualifierHelper::Invoker_Type;

    using Manager_Type = typename MyQualifierHelper::Manager_Type;

    template <typename Functor>
    using Callable = typename MyQualifierHelper::template Callable<Functor>;

    // The `M_functor` store the callable object.
    using MyQualifierHelper::M_functor;

    // The `M_manager` describes how to move / copy / destroy the `M_functor`,
    // and even describes how to invoke `M_functor` when not using `M_invoker`.
    using MyQualifierHelper::M_manager;

#if ( EMBED_FN_NEED_FAST_CALL == true )
    // invoker for the functor (func pointer)
    /// If @b EMBED_FN_NEED_FAST_CALL is not true, `M_manager` 
    /// will help Fn invoke functor instead of `M_invoker`.
    using MyQualifierHelper::M_invoker;
#endif

    // ArgsPackage, RetType, and ArgsNum
    using ArgsPackage   = typename          FnTraits::unwrap_signature<Signature>::args;
    using RetType       = typename          FnTraits::unwrap_signature<Signature>::ret;
    static constexpr std::size_t ArgsNum =  FnTraits::unwrap_signature<Signature>::arg_num;

    // Regard all Fn<Signature, BufSize> as friend class.
    template <typename Sig, std::size_t BSize>
    friend class Fn;

  public:
    // See https://en.cppreference.com/w/cpp/utility/functional/function.html
    // Get the return type.
    using result_type = RetType;

    // The `BufSize` of this embed::Fn object.
    static constexpr std::size_t buffer_size = BufSize;

    // `true` if fast-call mode is enabled.
    static constexpr bool is_fast_mode = static_cast<bool>(EMBED_FN_NEED_FAST_CALL);

  public:

    /**
     * @brief Destroy the functor, call functor's destructor.
     * 
     * @attention 
     * 
     * @b GCC: Due to the more conservative strategy
     * choice of GCC, even if `M_manager` can be determined
     * as `nullptr` during compilation and will not be modified,
     * GCC still chooses to retain the entire destructor code.
     * 
     * This also means that: even if the `embed::Fn` instance is
     * empty and unused, GCC must reserve stack space for it
     * which is to be used by the non-optimizable destructor.
     * 
     * @b MSVC: Same as GCC.
     * 
     * @b Clang: The Clang compiler is capable of detecting empty
     * and unused `embed::Fn` instances and completely eliminating
     * the destructors and stack space of the objects.
     */
    EMBED_INLINE ~Fn() noexcept
    {
      if (M_manager)
        (void)M_manager(M_functor, M_functor, OP_destroy_functor);
    }

    // Create an empty function wrapper.
    /// @deprecated Creating an empty embed::Fn instance is risky.
    EMBED_INLINE Fn() noexcept = default;

    // Create an empty function wrapper.
    /// @deprecated Creating an empty embed::Fn instance is risky.
    EMBED_INLINE Fn(std::nullptr_t) noexcept {}

#if ( EMBED_FN_NEED_FAST_CALL == true )

    // Copy constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's copy-constructor.
    Fn(const Fn& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_clone_functor);
        M_manager = fn.M_manager;
        M_invoker = fn.M_invoker;
      }
    }

    // Move constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's move-constructor.
    Fn(Fn&& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        M_manager = fn.M_manager;
        M_invoker = fn.M_invoker;
        fn.M_manager = nullptr;
        fn.M_invoker = nullptr;
      }
    }

    // Construct Fn<Sig_A> with Fn<Sig_B>
    // restrictions: `Sig_B.ret` can convert to `Sig_A.ret`
    // `Sig_A.args` are same with `Sig_B.args`.
    template <typename OtherSignature, std::size_t OtherSize>
    Fn(
      const Fn<OtherSignature, OtherSize>& fn,
      typename std::enable_if<FnTraits::is_Fn_and_similar<
        Signature, Fn<OtherSignature, OtherSize>
      >::value, bool>::type = true
    ) noexcept {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(
          *reinterpret_cast<detail::FnFunctor<OtherSize>*>(&M_functor),
          fn.M_functor, OP_clone_functor
        );
        M_manager = reinterpret_cast<Manager_Type>(fn.M_manager);
        M_invoker = reinterpret_cast<Invoker_Type>(fn.M_invoker);
      }
    }

    /**
     * @brief Builds a Fn that targets a copy of the incoming
     * function object.
     * @param func - A callable object with parameters of type
     * `ArgsType...` and returns a value convertible to `RetType`.
     * 
     * REQUIRE:
     * 1. `decltype(func)` must be Callable.
     * 2. `std::decay<decltype(func)>::type` must be nothrow copy constructible.
     * 3. `std::decay<decltype(func)>::type` must be nothrow constructible from
     * the type `decltype(func)` object.
     */
    template <typename Functor,
      typename DecayFunctor = Fn::DecayFunc_t<Functor>,
      typename = typename std::enable_if<!FnTraits::is_Fn_and_similar<
        Signature, FnTraits::remove_cvref_t<Functor>
      >::value>::type,
      typename = FnTraits::disableIf_movable_and_non_copyable_and_nref_t<Functor> >
    Fn(Functor&& func) noexcept
    {
      static_assert(Fn::Callable<Functor>::value,
        "embed::Fn requires the Functor is callable and the Signature match RetType");

      static_assert(std::is_copy_constructible<DecayFunctor>::value,
        "embed::Fn target must be copy-constructible");

      static_assert(std::is_nothrow_constructible<DecayFunctor, Functor>::value,
        "embed::Fn target must be NO-THROW constructible from the "
        "constructor argument");

      static_assert(
        !(FnTraits::get_signature_qualifier<Signature>::is_const 
          && FnTraits::is_class_and_has_call_operator<
            typename FnTraits::unwrap_signature<Signature>::pure_sig,
            Functor>::not_const_q),
        "embed::Fn requires the signature is non-const-qualified because the"
        " Functor::operator() is not const-qualified"
      );

      if (Fn::MyManager<DecayFunctor>::M_not_empty_function(func))
      {
        Fn::MyManager<DecayFunctor>::M_init_functor(M_functor, std::forward<Functor>(func));

        // To suppress the warnings of Arduino Uno, a forced type conversion is added here.
        static_assert(
          std::is_same<Manager_Type, decltype(&Fn::MyManager<DecayFunctor>::M_manager)>::value
          && std::is_same<Invoker_Type, decltype(&Fn::MyInvoker<DecayFunctor>::M_invoke)>::value,
          "The library ensures that the types of the two are consistent."
        );
        M_manager = reinterpret_cast<Manager_Type>(&Fn::MyManager<DecayFunctor>::M_manager);
        M_invoker = reinterpret_cast<Invoker_Type>(&Fn::MyInvoker<DecayFunctor>::M_invoke);
      }
    }

# if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    /**
     * @brief Constructor for non-copyable object.
     * @attention embed::Fn instance constructed from non-copyable object
     * CANNOT use any copy behaviour, otherwise `std::terminate` will be called.
     */
    template <typename Functor,
      typename DecayFunctor = Fn::DecayFunc_t<Functor>,
      typename = FnTraits::enableIf_movable_and_non_copyable_t<DecayFunctor>,
      typename = typename std::enable_if<!FnTraits::is_Fn_and_similar<
        Signature, FnTraits::remove_cvref_t<Functor>
      >::value>::type,
      typename = typename std::enable_if<!std::is_reference<Functor>::value>::type >
    Fn(Functor&& func)
    {
      static_assert(Fn::Callable<Functor>::value,
        "embed::Fn requires the Functor is callable and the Signature match RetType");

      static_assert(
        !(FnTraits::get_signature_qualifier<Signature>::is_const 
          && FnTraits::is_class_and_has_call_operator<
            typename FnTraits::unwrap_signature<Signature>::pure_sig,
            Functor>::not_const_q),
        "embed::Fn requires the signature is non-const-qualified because the"
        " Functor::operator() is not const-qualified"
      );

      if (Fn::MyMoveOnly<DecayFunctor>::M_not_empty_function(func))
      {
        Fn::MyMoveOnly<DecayFunctor>::M_init_functor(M_functor, std::move(func));

        // To suppress the warnings of Arduino Uno, a forced type conversion is added here.
        static_assert(
          std::is_same<Manager_Type, decltype(&Fn::MyMoveOnly<DecayFunctor>::M_manager)>::value
          && std::is_same<Invoker_Type, decltype(&Fn::MyInvoker<DecayFunctor>::M_invoke)>::value,
          "The library ensures that the types of the two are consistent."
        );
        M_manager = reinterpret_cast<Manager_Type>(&Fn::MyMoveOnly<DecayFunctor>::M_manager);
        M_invoker = reinterpret_cast<Invoker_Type>(&Fn::MyInvoker<DecayFunctor>::M_invoke);
      }
    }
# endif // !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)

    // Swap Fn but unknown the real type of `Functor`.
    // But only M_manager remember the `Functor`.
    void swap(Fn& fn) noexcept
    {
      detail::FnFunctor<BufSize> tmpFunc{};
      if (M_manager)
      {
        M_manager(tmpFunc, M_functor, OP_move_functor);
        M_manager(M_functor, M_functor, OP_destroy_functor);
      }
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        fn.M_manager(fn.M_functor, fn.M_functor, OP_destroy_functor);
      }
      if (M_manager)
      {
        M_manager(fn.M_functor, tmpFunc, OP_move_functor);
        M_manager(tmpFunc, tmpFunc, OP_destroy_functor);
      }
      std::swap(M_manager, fn.M_manager);
      std::swap(M_invoker, fn.M_invoker);
    }

    /// @brief Overload the function specifically for the case where nullptr is
    /// passed as a parameter, in order to improve the program's running efficiency. 
    /// (Using the `swap` method would be much slower.)
    EMBED_INLINE Fn& operator=(std::nullptr_t) noexcept
    {
      if (M_manager)
      {
        M_manager(M_functor, M_functor, OP_destroy_functor);
        M_manager = nullptr;
        M_invoker = nullptr;
      }
      return *this;
    }

    /// @brief Overload the move assign to enhance the performance.
    Fn& operator=(Fn&& fn) noexcept
    {
      if (M_manager != nullptr) {
        M_manager(M_functor, M_functor, OP_destroy_functor);
        M_manager = nullptr;
        M_invoker = nullptr;
      }
      if (static_cast<bool>(fn)) {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        M_manager = fn.M_manager;
        M_invoker = fn.M_invoker;
        fn.M_manager = nullptr;
        fn.M_invoker = nullptr;
      }
      return *this;
    }

#else

    // Copy constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's copy-constructor.
    Fn(const Fn& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_clone_functor);
        M_manager = fn.M_manager;
      }
    }

    // Move constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's move-constructor.
    Fn(Fn&& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        M_manager = fn.M_manager;
        fn.M_manager = nullptr;
      }
    }

    // Construct Fn<Sig_A> with Fn<Sig_B>
    // restrictions: `Sig_B.ret` can convert to `Sig_A.ret`
    // `Sig_A.args` are same with `Sig_B.args`.
    template <typename OtherSignature, std::size_t OtherSize>
    Fn(
      const Fn<OtherSignature, OtherSize>& fn,
      typename std::enable_if<FnTraits::is_Fn_and_similar<
        Signature, Fn<OtherSignature, OtherSize>
      >::value, bool>::type = true
    ) noexcept {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(
          *reinterpret_cast<detail::FnFunctor<OtherSize>*>(&M_functor),
          fn.M_functor, OP_clone_functor
        );
        M_manager = reinterpret_cast<Manager_Type>(fn.M_manager);
      }
    }

    /**
     * @brief Builds a Fn that targets a copy of the incoming
     * function object.
     * @param func - A callable object with parameters of type
     * `ArgsType...` and returns a value convertible to `RetType`.
     * 
     * REQUIRE:
     * 1. `decltype(func)` must be Callable.
     * 2. `std::decay<decltype(func)>::type` must be nothrow copy constructible.
     * 3. `std::decay<decltype(func)>::type` must be nothrow constructible from
     * the type `decltype(func)` object.
     */
    template <typename Functor,
      typename DecayFunctor = Fn::DecayFunc_t<Functor>,
      typename = typename std::enable_if<!FnTraits::is_Fn_and_similar<
        Signature, FnTraits::remove_cvref_t<Functor>
      >::value>::type,
      typename = FnTraits::disableIf_movable_and_non_copyable_and_nref_t<Functor> >
    Fn(Functor&& func) noexcept
    {
      static_assert(Fn::Callable<Functor>::value,
        "embed::Fn requires the Functor is callable and the Signature match RetType");

      static_assert(std::is_copy_constructible<DecayFunctor>::value,
        "embed::Fn target must be copy-constructible");

      static_assert(std::is_nothrow_constructible<DecayFunctor, Functor>::value,
        "embed::Fn target must be NO-THROW constructible from the "
        "constructor argument");

      static_assert(
        !(FnTraits::get_signature_qualifier<Signature>::is_const 
          && FnTraits::is_class_and_has_call_operator<
            typename FnTraits::unwrap_signature<Signature>::pure_sig,
            Functor>::not_const_q),
        "embed::Fn requires the signature is non-const-qualified because the"
        " Functor::operator() is not const-qualified"
      );

      if (Fn::MyManager<DecayFunctor>::M_not_empty_function(func))
      {
        Fn::MyManager<DecayFunctor>::M_init_functor(M_functor, std::forward<Functor>(func));

        // To suppress the warnings of Arduino Uno, a forced type conversion is added here.
        static_assert(
          std::is_same<Manager_Type, decltype(&Fn::MyManager<DecayFunctor>::M_manager)>::value,
          "The library ensures that the types of the two are consistent."
        );
        M_manager = reinterpret_cast<Manager_Type>(&Fn::MyManager<DecayFunctor>::M_manager);
      }
    }

# if !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)
    /**
     * @brief Constructor for non-copyable object.
     * @attention embed::Fn instance constructed from non-copyable object
     * CANNOT use any copy behaviour, otherwise `std::terminate` will be called.
     */
    template <typename Functor,
      typename DecayFunctor = Fn::DecayFunc_t<Functor>,
      typename = FnTraits::enableIf_movable_and_non_copyable_t<DecayFunctor>,
      typename = typename std::enable_if<!FnTraits::is_Fn_and_similar<
        Signature, FnTraits::remove_cvref_t<Functor>
      >::value>::type,
      typename = typename std::enable_if<!std::is_reference<Functor>::value>::type >
    Fn(Functor&& func)
    {
      static_assert(Fn::Callable<Functor>::value,
        "embed::Fn requires the Functor is callable and the Signature match RetType");

      static_assert(
        !(FnTraits::get_signature_qualifier<Signature>::is_const 
          && FnTraits::is_class_and_has_call_operator<
            typename FnTraits::unwrap_signature<Signature>::pure_sig,
            Functor>::not_const_q),
        "embed::Fn requires the signature is non-const-qualified because the"
        " Functor::operator() is not const-qualified"
      );

      if (Fn::MyMoveOnly<DecayFunctor>::M_not_empty_function(func))
      {
        Fn::MyMoveOnly<DecayFunctor>::M_init_functor(M_functor, std::move(func));

        // To suppress the warnings of Arduino Uno, a forced type conversion is added here.
        static_assert(
          std::is_same<Manager_Type, decltype(&Fn::MyMoveOnly<DecayFunctor>::M_manager)>::value,
          "The library ensures that the types of the two are consistent."
        );
        M_manager = reinterpret_cast<Manager_Type>(&Fn::MyMoveOnly<DecayFunctor>::M_manager);
      }
    }
# endif // !defined(EMBED_NO_NONCOPYABLE_FUNCTOR)

    // Swap Fn but unknown the real type of `Functor`.
    // But only M_manager remember the `Functor`.
    void swap(Fn& fn) noexcept
    {
      detail::FnFunctor<BufSize> tmpFunc{};
      if (M_manager)
      {
        M_manager(tmpFunc, M_functor, OP_move_functor);
        M_manager(M_functor, M_functor, OP_destroy_functor);
      }
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        fn.M_manager(fn.M_functor, fn.M_functor, OP_destroy_functor);
      }
      if (M_manager)
      {
        M_manager(fn.M_functor, tmpFunc, OP_move_functor);
        M_manager(tmpFunc, tmpFunc, OP_destroy_functor);
      }
      std::swap(M_manager, fn.M_manager);
    }

    /// @brief Overload the function specifically for the case where nullptr is
    /// passed as a parameter, in order to improve the program's running efficiency. 
    /// (Using the `swap` method would be much slower.)
    EMBED_INLINE Fn& operator=(std::nullptr_t) noexcept
    {
      if (M_manager)
      {
        M_manager(M_functor, M_functor, OP_destroy_functor);
        M_manager = nullptr;
      }
      return *this;
    }

    /// @brief Overload the move assign to enhance the performance.
    Fn& operator=(Fn&& fn) noexcept
    {
      if (M_manager != nullptr) {
        M_manager(M_functor, M_functor, OP_destroy_functor);
        M_manager = nullptr;
      }
      if (static_cast<bool>(fn)) {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        M_manager = fn.M_manager;
        fn.M_manager = nullptr;
      }
      return *this;
    }

#endif // End EMBED_FN_NEED_FAST_CALL == true or not

    /// @attention operator= may consume more resource,
    /// maybe copy/move constructor is better.
    EMBED_INLINE Fn& operator=(const Fn& fn) noexcept
    {
      if (this != std::addressof(fn))
        Fn(fn).swap(*this);
      return *this;
    }

    /// @attention operator= may consume more resource,
    /// maybe copy/move constructor is better.
    template <typename OtherSignature, std::size_t OtherSize,
      typename = typename std::enable_if<FnTraits::is_Fn_and_similar<
        Signature, Fn<OtherSignature, OtherSize>
      >::value>::type>
    EMBED_INLINE Fn& operator=(const Fn<OtherSignature, OtherSize>& fn) noexcept 
    {
      Fn(fn).swap(*this);
      return *this;
    }

    /// @attention operator= may consume more resource,
    /// maybe copy/move constructor is better.
    template <typename Functor,
      typename DecayFunc = Fn::DecayFunc_t<Functor>,
      typename = typename std::enable_if<!FnTraits::is_Fn_and_similar<
        Signature, FnTraits::remove_cvref_t<Functor>
      >::value>::type>
    EMBED_INLINE Fn& operator=(Functor&& func) noexcept
    {
      Fn(std::forward<Functor>(func)).swap(*this);
      return *this;
    }

    // check if the embed::Fn is empty.
    EMBED_INLINE constexpr bool is_empty() const noexcept
    {
      return static_cast<bool>( M_manager == nullptr );
    }

    // `true` if the embed::Fn is not empty.
    EMBED_INLINE constexpr explicit operator bool() const noexcept
    {
      return !is_empty();
    }


  }; // end Fn


  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static EMBED_INLINE constexpr bool
  operator==(const Fn<Signature, BufSize>& fn, std::nullptr_t) noexcept
  { return fn.is_empty(); }

  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static EMBED_INLINE constexpr bool
  operator==(std::nullptr_t, const Fn<Signature, BufSize>& fn) noexcept
  { return fn.is_empty(); }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static EMBED_INLINE constexpr bool
  operator!=(const Fn<Signature, BufSize>& fn, std::nullptr_t) noexcept
  { return !fn.is_empty(); }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static EMBED_INLINE constexpr bool
  operator!=(std::nullptr_t, const Fn<Signature, BufSize>& fn) noexcept
  { return !fn.is_empty(); }

  /**
   * @brief `embed::function` is an alias of `embed::Fn`.
   * @note It is encouraged to use `embed::function` instead of `embed::Fn`.
   * `embed::function` will automatically align the BufSize.
   */
  template <typename Signature, std::size_t BufSize = detail::FnDefaultBufSize>
  using function = Fn<Signature, detail::FnToolBox::FnTraits::aligned_buf_size<BufSize>::value>;

  /**
   * @brief Make a function and automatically calculate the required size.
   * @note `embed::make_function` has many kinds of override function.
   */
  template <typename Signature, typename Functor>
  EMBED_NODISCARD inline typename std::enable_if<
    !detail::FnToolBox::FnTraits::is_Fn_and_similar<
      Signature,
      typename std::remove_const<
        typename std::remove_reference<Functor>::type
      >::type
    >::value,
    function<Signature, sizeof(Functor)>
  >::type
  make_function(Functor&& func) noexcept
  {
    return function<Signature, sizeof(Functor)>(std::forward<Functor>(func));
  }

  // Overload for empty.
  template <typename Signature, std::size_t BufSize = detail::FnDefaultBufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function() noexcept
  { return function<Signature, BufSize>(); }

  // Overload for nullptr.
  template <typename Signature, std::size_t BufSize = detail::FnDefaultBufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function(std::nullptr_t) noexcept
  { return function<Signature>(nullptr); }

  // Overload for free function.
  template <typename RetType, typename... ArgsType>
  EMBED_NODISCARD inline function<RetType(ArgsType...) const>
  make_function(RetType (*func) (ArgsType...) EMBED_FN_CASE_NOEXCEPT) noexcept
  {
    return function<RetType(ArgsType...) const>(func);
  }

  // Overload for free function with specified signature.
  template <typename Signature, typename RetType, typename... ArgsType>
  EMBED_NODISCARD inline function<Signature>
  make_function(RetType (*func) (ArgsType...) EMBED_FN_CASE_NOEXCEPT) noexcept
  {
    return function<Signature>(func);
  }

  // Overload for lambda function or other object which
  // uniquely override the `operator()`.
  template <typename Lambda,
    typename ClassType = typename std::remove_const<
      typename std::remove_reference<Lambda>::type
    >::type,
    typename Signature = typename 
    detail::FnToolBox::FnTraits::get_unique_call_signature<ClassType>::type
  >
  EMBED_NODISCARD inline typename std::enable_if<
    detail::FnToolBox::FnTraits::is_unique_callable<ClassType>::value,
    function<Signature, sizeof(Lambda)>
  >::type
  make_function(Lambda&& la) noexcept
  {
    return function<Signature, sizeof(Lambda)>(std::forward<Lambda>(la));
  }

  // Overload for `embed::Fn`. (Copy)
  template <typename Signature, std::size_t BufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function(const Fn<Signature, BufSize>& fn) noexcept
  {
    return function<Signature, BufSize>(fn);
  }

  // Overload for `embed::Fn`. (Move)
  template <typename Signature, std::size_t BufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function(Fn<Signature, BufSize>&& fn) noexcept
  {
    return function<Signature, BufSize>(std::move(fn));
  }

  // Overload for `embed::Fn<Other, Size>`.
  template <typename Signature, typename OtherSignature, std::size_t BufSize>
  EMBED_NODISCARD inline typename std::enable_if<
    detail::FnToolBox::FnTraits::is_similar_Fn_signature<
      Signature, OtherSignature, BufSize, BufSize
    >::value,
    function<Signature, BufSize>
  >::type
  make_function(const Fn<OtherSignature, BufSize>& fn) noexcept
  {
    return function<Signature, BufSize>(fn);
  }

  // Overload for member function.
# define EMBED_FN_OVERLOAD_MEMBER_FUNCTION(CONST_, VOLATILE_, REF_)                           \
  template <typename Class, typename RetType, typename... ArgsType>                           \
  EMBED_NODISCARD inline auto                                                                 \
  make_function(RetType (Class::* member_func) (ArgsType...) CONST_ VOLATILE_ REF_) noexcept  \
  -> function<RetType(typename std::conditional<std::is_rvalue_reference<int REF_>::value,    \
    CONST_ VOLATILE_ Class&&, CONST_ VOLATILE_ Class&>::type, ArgsType...) const,             \
    sizeof(member_func)>                                                                      \
  {                                                                                           \
    return function<RetType(typename std::conditional<                                        \
      std::is_rvalue_reference<int REF_>::value,                                              \
      CONST_ VOLATILE_ Class&&, CONST_ VOLATILE_ Class&>::type, ArgsType...) const,           \
      sizeof(member_func)>(member_func);                                                      \
  }

  // Here, macros are used to overload the make_function, 
  // enabling it to adapt to various member functions(const / volatile / &).
  EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_OVERLOAD_MEMBER_FUNCTION)

# undef EMBED_FN_OVERLOAD_MEMBER_FUNCTION


#if ( __cpp_deduction_guides >= 201606 ) || ( EMBED_CXX_VERSION >= 201703L )

  // Deduce the template type.
namespace detail {

  template <typename Functor>
  struct function_deduce_guide_helper
  {
    using type = decltype(make_function(std::declval<Functor>()));
  };

  template <typename T> struct function_deduce_get_signature;

  template <typename Sig, std::size_t Buf>
  struct function_deduce_get_signature<Fn<Sig, Buf>>
  {
    using signature = Sig;
    static constexpr std::size_t bufsize = Buf;
  };

} // end namespace embed::detail

  template <typename Functor,
    typename DeduceRet = typename detail::function_deduce_guide_helper<Functor>::type,
    typename Signature = typename detail::function_deduce_get_signature<DeduceRet>::signature,
    std::size_t BufferSize = detail::function_deduce_get_signature<DeduceRet>::bufsize>
  Fn(Functor) -> Fn<Signature, BufferSize>;

#endif

} // end namespace embed

#if defined(EMBED_NO_STD_HEADER)
# undef std
EMBED_POP_STD
#endif

// std::swap
namespace std EMBED_ABI_VISIBILITY(default)
{

  template<typename Signature, decltype(sizeof(int)) BufSize>
  inline void swap(
    embed::Fn<Signature, BufSize>& fn1,
    embed::Fn<Signature, BufSize>& fn2
  ) noexcept { fn1.swap(fn2); }

}


#undef EMBED_FN_NEED_FAST_CALL
#undef EMBED_FN_NOTHROW_CALLABLE
#undef EMBED_FN_CASE_NOEXCEPT
#undef EMBED_FN_ENSURE_NO_THROW
#undef EMBED_FN_NO_WARING
#undef EMBED_FN_GENERATE_CODE_C_V_REF
#undef EMBED_FN_SWITCH_EXPECT

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // EMBED_FUNCTION_HPP_

