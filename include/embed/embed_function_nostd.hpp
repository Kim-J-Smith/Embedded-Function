#ifndef EMBED_FUNCTION_NOSTD_HPP__
#define EMBED_FUNCTION_NOSTD_HPP__

/// @c EMBED_CXX_VERSION
#ifndef EMBED_CXX_VERSION
# if defined(_MSC_VER)
#  define EMBED_CXX_VERSION _MSVC_LANG
# else
#  define EMBED_CXX_VERSION __cplusplus
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

/// @c EMBED_INLINE
#ifndef EMBED_INLINE
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_INLINE __attribute__((always_inline)) inline
# elif defined(_MSC_VER)
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

/// @c EMBED_CXX17_NODISCARD
#ifndef EMBED_CXX17_NODISCARD
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_CXX17_NODISCARD [[nodiscard]]
# else
#  define EMBED_CXX17_NODISCARD
# endif
#endif

/// @c EMBED_PLACEMENT_NEW_CONSTEXPR
#ifndef EMBED_PLACEMENT_NEW_CONSTEXPR
# if __cpp_lib_constexpr_new < 202406L
#  define EMBED_PLACEMENT_NEW_CONSTEXPR inline
# else
#  define EMBED_PLACEMENT_NEW_CONSTEXPR constexpr
# endif
#endif

/// @c EMBED_NORETURN
#ifndef EMBED_NORETURN
# if defined(__GNUC__) || defined(__clang__) || defined(__TI_COMPILER_VERSION__)
#  define EMBED_NORETURN __attribute__((__noreturn__))
# elif defined(_MSC_VER)
#  define EMBED_NORETURN __declspec(noreturn)
# else
#  define EMBED_NORETURN
# endif
#endif

#if defined(_MSC_VER)
extern "C" EMBED_NORETURN __declspec(dllimport) void abort(void);
#else
extern "C" EMBED_NORETURN void abort(void);
#endif

namespace embed { namespace detail { namespace fn_no_std {

  // std::size_t
  using size_t = decltype(sizeof(int));

  // std::nullptr_t
  using nullptr_t = decltype(nullptr);

  // std::exception
  class exception {
  public:
    exception() noexcept {}
    exception(const exception&) noexcept            = default;
    exception& operator=(const exception&) noexcept = default;

    virtual ~exception() noexcept {};
    virtual const char* what() const noexcept       = 0;
  };

#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
  // std::terminate
  [[noreturn]] static EMBED_INLINE void terminate() { abort(); };
#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic pop
#endif

  // std::integral_constant
  template <typename T, T val>
  struct integral_constant
  {
    static constexpr T                  value = val;
    typedef T                           value_type;
    typedef integral_constant<T, val>   type;

    constexpr operator value_type() const noexcept
    { return value; }

    constexpr value_type operator()() const noexcept
    { return value; }
  };

  // std::bool_constant
  template <bool val>
  using bool_constant = integral_constant<bool, val>;

  // std::true_type
  using true_type = integral_constant<bool, true>;

  // std::false_type
  using false_type = integral_constant<bool, false>;

  // std::is_same
  template <typename T, typename U>
  struct is_same : public false_type {};

  template <typename T>
  struct is_same<T, T> : public true_type {};

  // std::is_const
  template <typename T>
  struct is_const : false_type {};

  template <typename T>
  struct is_const<T const> : true_type {};

  // std::is_volatile
  template <typename T>
  struct is_volatile : false_type {};

  template <typename T>
  struct is_volatile<T volatile> : true_type {};

  // std::is_reference
  template <typename T>
  struct is_reference : public false_type {};

  template <typename T>
  struct is_reference<T&> : public true_type {};

  template <typename T>
  struct is_reference<T&&> : public true_type {};

  // std::is_pointer
  template <typename T>
  struct is_pointer : public false_type {};

  template <typename T>
  struct is_pointer<T*> : public true_type {};

  // std::is_array
  template <typename T>
  struct is_array : public false_type {};

  template <typename T>
  struct is_array<T[]> : public false_type {};

  template <typename T, size_t S>
  struct is_array<T[S]> : public false_type {};

  // std::is_function
  template <typename T>
  struct is_function 
  : public bool_constant<!is_const<const T>::value>::type {};

  template <typename T>
  struct is_function<T&> : public false_type {};

  template <typename T>
  struct is_function<T&&> : public false_type {};

  // std::remove_pointer
  template <typename T>
  struct remove_pointer { using type = T; };

  template <typename T>
  struct remove_pointer<T*> { using type = T; };

  // std::declval
  template <class _Tp>
  _Tp&& __declval(int);
  template <class _Tp>
  _Tp __declval(long);

  template <class _Tp>
  decltype(__declval<_Tp>(0)) declval() noexcept {
    static_assert(!is_same<_Tp, _Tp>::value,
    "std::declval can only be used in an unevaluated context. "
    "It's likely that your current usage is trying to extract a value from the function.");
  }

  // std::remove_cv
  // remove_cv
  template <typename T>
  struct remove_cv
  { typedef T type; };

  template <typename T>
  struct remove_cv<const T>
  { typedef T type; };

  template <typename T>
  struct remove_cv<volatile T>
  { typedef T type; };

  template <typename T>
  struct remove_cv<const volatile T>
  { typedef T type; };

  // __is_void_helper
  template <typename T>
  struct __is_void_helper : public false_type
  { };

  template <>
  struct __is_void_helper<void> : public true_type
  { };

  /// is_void
  template <typename T>
  struct is_void 
  : public __is_void_helper<typename remove_cv<T>::type>
  { };

  // std::enable_if
  template <bool, typename T = void>
  struct enable_if {};

  template <typename T>
  struct enable_if<true, T>
  { typedef T type; };

  // std::remove_const
  template <typename T>
  struct remove_const { using type = T; };

  template <typename T>
  struct remove_const<T const> { using type = T; };

  // std::remove_reference
  template <typename T>
  struct remove_reference { using type = T; };

  template <typename T>
  struct remove_reference<T&> { using type = T; };

  template <typename T>
  struct remove_reference<T&&> { using type = T; };

  // std::conditional
  template <bool Cond, typename If_true, typename If_false>
  struct conditional
  { typedef If_true type; };

  template <typename If_true, typename If_false>
  struct conditional<false, If_true, If_false>
  { typedef If_false type; };

  // std::is_nothrow_constructible
#if EMBED_HAS_BUILTIN(__is_nothrow_constructible)
  template < class _Tp, class... _Args>
  struct is_nothrow_constructible
  : public integral_constant<bool, __is_nothrow_constructible(_Tp, _Args...)> {};
#else
  template < class _Tp, class... _Args>
  struct is_nothrow_constructible
  : public true_type {};
#endif

  struct __is_referenceable_impl {
    template <class _Tp>
    static _Tp& __test(int);
    template <class _Tp>
    static false_type __test(...);
  };

  template <class _Tp>
  struct _is_referenceable
  : public integral_constant<
    bool, !is_same<
      decltype(__is_referenceable_impl::__test<_Tp>(0)), false_type
    >::value
  > {};

  template <class _Tp, bool = _is_referenceable<_Tp>::value>
  struct __add_lvalue_reference_impl {
    using type = _Tp;
  };
  template <class _Tp >
  struct __add_lvalue_reference_impl<_Tp, true> {
    using type = _Tp&;
  };

  template <class _Tp>
  using __add_lvalue_reference_t = typename __add_lvalue_reference_impl<_Tp>::type;

  // std::is_copy_constructible
  // this is a very important trait. If `__is_constructible` cannot be used,
  // the non-copyable object cannot be wrapped into embed::Fn.
#if EMBED_HAS_BUILTIN(__is_constructible)
  template <class _Tp>
  struct is_copy_constructible
  : public integral_constant<bool, __is_constructible(_Tp, __add_lvalue_reference_t<const _Tp>)> {};
#else
  template <class _Tp>
  struct is_copy_constructible
  : public true_type {};
# define EMBED_NO_NONCOPYABLE_FUNCTOR
#endif

  // std::is_nothrow_copy_constructible
#if EMBED_HAS_BUILTIN(__is_nothrow_constructible)
  template <class _Tp>
  struct is_nothrow_copy_constructible
  : public integral_constant<
    bool, __is_nothrow_constructible(_Tp, __add_lvalue_reference_t<const _Tp>)
  > {};
#else
  template <class _Tp>
  struct is_nothrow_copy_constructible
  : public true_type {};
#endif

  // __is_nothrow_destructible
#if EMBED_HAS_BUILTIN(__is_nothrow_destructible)
  template <class _Tp>
  struct is_nothrow_destructible
  : public integral_constant<bool, __is_nothrow_destructible(_Tp)> {};
#else
  template <class _Tp>
  struct is_nothrow_destructible
  : public true_type {};
#endif

  template <class _Tp, bool = _is_referenceable<_Tp>::value>
  struct __add_rvalue_reference_impl {
    using type = _Tp;
  };
  template <class _Tp >
  struct __add_rvalue_reference_impl<_Tp, true> {
    using type = _Tp&&;
  };

  template <class _Tp>
  using __add_rvalue_reference_t = typename __add_rvalue_reference_impl<_Tp>::type;

  // std::is_nothrow_move_constructible
#if EMBED_HAS_BUILTIN(__is_nothrow_constructible)
  template <class _Tp>
  struct is_nothrow_move_constructible
  : public integral_constant<
    bool, __is_nothrow_constructible(_Tp, __add_rvalue_reference_t<_Tp>)
  > {};
#else
  template <class _Tp>
  struct is_nothrow_move_constructible
  : public true_type {};
#endif

#if EMBED_HAS_BUILTIN(__is_constructible)
  template <class _Tp>
  struct is_move_constructible
  : public integral_constant<bool, __is_constructible(_Tp, __add_rvalue_reference_t<_Tp>)> {};
#else
  template <class _Tp>
  struct is_move_constructible
  : public true_type {};
#endif

  // std::__void_t
  template <typename T>
  struct make_void { using type = void; };

  template <typename T>
  using __void_t = typename make_void<T>::type;

  template<typename _Tp, typename = void>
  struct __add_pointer_helper
  { using type = _Tp; };

  template<typename _Tp>
  struct __add_pointer_helper<_Tp, __void_t<_Tp*>>
  { using type = _Tp*; };

  // std::add_pointer
  template<typename _Tp>
  struct add_pointer
  : public __add_pointer_helper<_Tp>
  { };

  template<typename _Tp>
  struct add_pointer<_Tp&>
  { using type = _Tp*; };

  template<typename _Tp>
  struct add_pointer<_Tp&&>
  { using type = _Tp*; };

  // Decay trait for arrays and functions, used for perfect forwarding
  // in make_pair, make_tuple, etc.
  template<typename _Up>
  struct __decay_selector
  : conditional<is_const<const _Up>::value, // false for functions
		remove_cv<_Up>,		  // N.B. DR 705.
		add_pointer<_Up>
  >::type { };		  // function decays to pointer

  template<typename _Up, size_t _Nm>
  struct __decay_selector<_Up[_Nm]>
  { using type = _Up*; };

  template<typename _Up>
  struct __decay_selector<_Up[]>
  { using type = _Up*; };

  // std::decay
  template<typename _Tp>
  struct decay
  { using type = typename __decay_selector<_Tp>::type; };

  template<typename _Tp>
  struct decay<_Tp&>
  { using type = typename __decay_selector<_Tp>::type; };

  template<typename _Tp>
  struct decay<_Tp&&>
  { using type = typename __decay_selector<_Tp>::type; };

  // __is_convertible_helper
  template <typename From, typename To, bool is_special, bool not_func>
  struct __is_convertible_helper
  : public is_void<To>::type
  { };

  // std::is_base_of
#if EMBED_HAS_BUILTIN(__is_base_of) || \
  (defined(__GNUC__) && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)))
  template <typename Base, typename Derived>
  struct is_base_of : bool_constant<__is_base_of(Base, Derived)> {};
#else
  template <typename Base, typename Derived>
  struct is_base_of : false_type {};
#endif

  // __is_member_object_pointer_helper
  template <typename T>
  struct __is_member_object_pointer_helper
  : public false_type {};

  template <typename C, typename R>
  struct __is_member_object_pointer_helper<R C::*>
  : public bool_constant<!is_function<R>::value> {};

  // std::is_member_object_pointer
  template <typename T>
  struct is_member_object_pointer
  : public __is_member_object_pointer_helper<
    typename remove_cv<
      typename remove_reference<T>::type
    >::type
  >::type {};

  // __is_member_function_pointer_helper
  template <typename T>
  struct __is_member_function_pointer_helper
  : public false_type {};

  template <typename C, typename R>
  struct __is_member_function_pointer_helper<R C::*>
  : public is_function<R> {};

  // std::is_member_function_pointer
  template <typename T>
  struct is_member_function_pointer
  : public __is_member_function_pointer_helper<
    typename remove_cv<
      typename remove_reference<T>::type
    >::type
  >::type {};

#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
# pragma GCC diagnostic ignored "-Wreturn-type"
#endif
  template <typename From, typename To>
  struct __is_convertible_helper<From, To, false, true>
  {
  private:
    typedef char    yes[1];
    typedef char    no[2];

    template <typename T>
    static T CreateT();

    template <typename To1>
    static int test_impl(To1);

    static int test_var; // just for sizeof

    template<typename From1, typename To1>
    static yes& test(int (*) [sizeof(test_var = test_impl<To1>(CreateT<From1>()))]);

    template<typename, typename>
    static no& test(...);

  public:
    static constexpr
    bool value = sizeof(test<From, To>(0)) == sizeof(yes) ? true : false;
  };
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif
  // Avoid potential compilation or linking errors
  template<typename From, typename To>
  constexpr bool
  __is_convertible_helper<From, To, false, true>::value;

  template <typename From, size_t S, typename To> // T[x] -> T* is right
  struct __is_convertible_helper<From[S], To, false, true>
  : public bool_constant<
    is_pointer<To>::value && (
      is_same<From,
        typename remove_cv<
          typename remove_pointer<To>::type
        >::type
      >::value
      || is_void<typename remove_pointer<To>::type>::value
    )
  >::type { };

  template <typename From, typename To> // T[] -> T* is right
  struct __is_convertible_helper<From[], To, false, true>
  : public bool_constant<
    is_pointer<To>::value && (
      is_same<From,
        typename remove_cv<
          typename remove_pointer<To>::type
        >::type
      >::value
      || is_void<typename remove_pointer<To>::type>::value
    )
  >::type { };

  template <typename From, typename To> // function -> function pointer is right
  struct __is_convertible_helper<From, To, false, false>
  : public bool_constant<
    is_pointer<To>::value
    && is_same<
      From, typename remove_pointer<To>::type
    >::value
  >::type
  { };

  // std::is_convertible
  template <typename From, typename To>
  struct is_convertible
  : public bool_constant<
    __is_convertible_helper<
      From, To,
      is_void<From>::value || is_function<To>::value || is_array<To>::value,
      !is_function<From>::value
    >::value
  >::type
  { };

  template <typename Func>
  struct check_is_class : public true_type {};
  template <typename R, typename... As>
  struct check_is_class<R(*)(As...)> : public false_type {};
  template <typename R, typename Class>
  struct check_is_class<R Class::*> : public false_type {};

#define EMBED_FN_GENERATE_CODE_C_V_REF(F_)  \
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

#define EMBED_FN_OVERLOAD_IS_NOT_CLASS(C, V, REF, NOEXC) \
  template <typename R, typename Cs, typename... As>  \
  struct check_is_class<R(Cs::*)(As...) C V REF NOEXC> : public false_type {};

#if ( EMBED_CXX_VERSION >= 201703L || __cpp_noexcept_function_type >= 201510L )
# define EMBED_FN_OVERLOAD_IS_NOT_CLASS_HELPER(C, V, REF)\
  EMBED_FN_OVERLOAD_IS_NOT_CLASS(C, V, REF, noexcept) \
  EMBED_FN_OVERLOAD_IS_NOT_CLASS(C, V, REF,)

  template <typename R, typename... As>
  struct check_is_class<R(*)(As...) noexcept> : public false_type {};
#else
# define EMBED_FN_OVERLOAD_IS_NOT_CLASS_HELPER(C, V, REF)\
  EMBED_FN_OVERLOAD_IS_NOT_CLASS(C, V, REF,)
#endif

  // Overload `is_class_and_has_call_operator::check_is_class`
  EMBED_FN_GENERATE_CODE_C_V_REF(EMBED_FN_OVERLOAD_IS_NOT_CLASS_HELPER)

#undef EMBED_FN_OVERLOAD_IS_NOT_CLASS
#undef EMBED_FN_OVERLOAD_IS_NOT_CLASS_HELPER
#undef EMBED_FN_GENERATE_CODE_C_V_REF

  // std::is_class
#if defined(__GNUC__) && (__GNUC__ >= 5)
  template <typename T>
  struct is_class : public bool_constant<__is_class(T)> {};
#elif EMBED_HAS_BUILTIN(__is_class)
  template <typename T>
  struct is_class : public bool_constant<__is_class(T)> {};
#else
  template <typename T>
  struct is_class
  : public bool_constant<check_is_class<T>::value> {};
#endif

  // std::addressof
  template <typename T>
  EMBED_INLINE T* addressof(const T&& t) noexcept = delete;

#if EMBED_HAS_BUILTIN(__builtin_addressof)
  template <typename T>
  EMBED_INLINE T* addressof(T& t) noexcept {
    return __builtin_addressof(t);
  }
#else
  template <typename T>
  EMBED_INLINE T* addressof(T& t) noexcept {
    return &t;
  }
#endif

  // std::move
  template <typename T>
  EMBED_NODISCARD EMBED_INLINE constexpr
  typename remove_reference<T>::type&&
  move(T&& t) noexcept {
    using U = typename remove_reference<T>::type;
    return static_cast<U&&>(t);
  }

  // std::is_lvalue_reference
  template <typename T>
  struct is_lvalue_reference
  : public false_type { };

  template <typename T>
  struct is_lvalue_reference<T&>
  : public true_type { };

  // std::is_rvalue_reference
  template <typename T>
  struct is_rvalue_reference
  : public false_type { };

  template <typename T>
  struct is_rvalue_reference<T&&>
  : public true_type { };

  // std::forward
  template <class T>
  EMBED_NODISCARD EMBED_INLINE constexpr T&&
  forward(typename remove_reference<T>::type& __t) noexcept {
    return static_cast<T&&>(__t);
  }

  template <class T>
  EMBED_NODISCARD EMBED_INLINE constexpr T&&
  forward(typename remove_reference<T>::type&& __t) noexcept {
    static_assert(!is_lvalue_reference<T>::value, "cannot forward an rvalue as an lvalue");
    return static_cast<T&&>(__t);
  }

  // std::swap
  template <class _Tp>
  EMBED_INLINE void swap(_Tp& __x, _Tp& __y) noexcept {
    _Tp __t(move(__x));
    __x = move(__y);
    __y = move(__t);
  }

} } } // end namespace embed::detail::fn_no_std

#if 1
EMBED_CXX17_NODISCARD EMBED_PLACEMENT_NEW_CONSTEXPR void* 
operator new(decltype(sizeof(int)), void* __p) noexcept {
  return __p;
}

EMBED_CXX17_NODISCARD EMBED_PLACEMENT_NEW_CONSTEXPR void* 
operator new[](decltype(sizeof(int)), void* __p) noexcept {
  return __p;
}
#else
EMBED_CXX17_NODISCARD EMBED_PLACEMENT_NEW_CONSTEXPR void* 
operator new(decltype(sizeof(int)), void* __p) noexcept;

EMBED_CXX17_NODISCARD EMBED_PLACEMENT_NEW_CONSTEXPR void* 
operator new[](decltype(sizeof(int)), void* __p) noexcept;
#endif

#endif // EMBED_FUNCTION_NOSTD_HPP__

