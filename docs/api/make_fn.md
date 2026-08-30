# `ebd::make_fn`

## Overview

`ebd::make_fn` is a factory for creating Embedded Function wrappers from callable objects, function pointers, member pointers, and existing wrappers.

It can:

- deduce the signature of a lambda or other uniquely callable functor,
- choose `ebd::fn` or `ebd::unique_fn` automatically,
- build an empty wrapper with an explicit signature,
- create a wrapper with a specific wrapper type such as `ebd::classic_fn` or `ebd::fn_ref`.

## Overloads

### 1. Copyable functor with explicit signature

```cpp
template <typename Signature, typename Functor,
          typename Class = detail::remove_cvref_t<Functor>,
          bool NoThrow = detail::is_nothrow_construct_from_functor<Functor&&>::value,
          std::size_t Alignment = detail::enough_alignment<alignof(Class)>::value>
EMBED_NODISCARD inline fn<Signature, sizeof(Class), Alignment>
make_fn(Functor&& functor) noexcept(NoThrow);
```

Creates an `ebd::fn` for a class-type callable when the signature is specified explicitly. The buffer size is automatically deduced as `sizeof(Class)`, and the alignment as `alignof(Class)` (never less than `detail::default_values::owning::alignment`).

### 2. Move-only functor with explicit signature

```cpp
template <typename Signature, typename Functor,
          bool NoThrow = std::is_nothrow_move_constructible<Functor>::value,
          std::size_t Alignment = detail::enough_alignment<alignof(Functor)>::value>
EMBED_NODISCARD inline unique_fn<Signature, sizeof(Functor), Alignment>
make_fn(Functor&& functor) noexcept(NoThrow);
```

Creates an `ebd::unique_fn` for a move-only functor when the signature is specified explicitly.

### 3. Empty wrapper with explicit signature

```cpp
template <typename Signature,
          std::size_t BufferSize = detail::default_values::owning::buffer_size,
          std::size_t Alignment = detail::default_values::owning::alignment>
EMBED_NODISCARD inline fn<Signature, BufferSize, Alignment>
make_fn(std::nullptr_t = nullptr) noexcept;
```

Creates an empty `ebd::fn` with the given signature and buffer size.

### 4. Function pointer with deduced signature

```cpp
template <typename Ret, typename... Args>
EMBED_NODISCARD inline fn<Ret(Args...) const, sizeof(Ret(*)(Args...))>
make_fn(Ret (*func_ptr)(Args...)) noexcept;
```

Creates an `ebd::fn` from a free-function pointer and deduces both signature and buffer size.

### 4b. noexcept function pointer with deduced signature (C++17+)

```cpp
template <typename Ret, typename... Args>
EMBED_NODISCARD inline fn<Ret(Args...) const noexcept, sizeof(Ret(*)(Args...) noexcept)>
make_fn(Ret (*func_ptr)(Args...) noexcept) noexcept;
```

Creates an `ebd::fn` from a noexcept free-function pointer. The noexcept qualifier is preserved in the signature. Only available when noexcept is part of the type system (C++17 or `__cpp_noexcept_function_type >= 201510L`).

### 5. Function pointer with explicit signature

```cpp
template <typename Signature,
          typename FunctionPtr = typename detail::unwrap_signature<Signature>::pure_sig_noex*>
EMBED_NODISCARD inline fn<Signature, sizeof(FunctionPtr)>
make_fn(FunctionPtr func_ptr) noexcept;
```

Creates an `ebd::fn` from a free-function pointer using the specified signature.

### 6. Copy from another wrapper

```cpp
template <std::size_t Buf, std::size_t Align, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Align, Cfg, Sig>
make_fn(const detail::function<Buf, Align, Cfg, Sig>& fn)
noexcept(Cfg::isView || Cfg::assertNoThrow);
```

Creates a wrapper by copying another `ebd::detail::function`.

### 7. Move from another wrapper

```cpp
template <std::size_t Buf, std::size_t Align, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Align, Cfg, Sig>
make_fn(detail::function<Buf, Align, Cfg, Sig>&& fn)
noexcept(Cfg::isView || Cfg::assertNoThrow);
```

Creates a wrapper by moving another `ebd::detail::function`.

### 8. Lambda or uniquely callable functor

```cpp
template <typename Lambda,
          typename Class = detail::remove_cvref_t<Lambda>,
          std::size_t BufferSize = sizeof(Class),
          std::size_t Alignment = detail::enough_alignment<alignof(Class)>::value,
          typename Signature = detail::get_unique_signature_t<Class>,
          typename Fn = detail::conditional_t<
              std::is_copy_constructible<Class>::value,
              fn<Signature, BufferSize, Alignment>,
              unique_fn<Signature, BufferSize, Alignment>>,
          bool NoThrow = detail::is_nothrow_construct_from_functor<Lambda&&>::value>
EMBED_NODISCARD inline Fn make_fn(Lambda&& fn) noexcept(NoThrow);
```

Creates a wrapper from a lambda or other functor with exactly one viable `operator()`. The signature is deduced automatically; the buffer size and alignment are deduced from `sizeof(Class)` and `alignof(Class)` respectively.

### 9. Pointer to member function

```cpp
template <typename Class, typename Ret, typename... Args>
EMBED_NODISCARD inline auto
make_fn(Ret(Class::* memfunc)(Args...) C V REF NOEXCEPT) noexcept
-> fn<
    Ret(detail::get_qualified_with_t<int REF, C V Class>, Args...) const NOEXCEPT,
    sizeof(memfunc)
   >;
```

Creates an `ebd::fn` from a pointer to member function. Cv/ref/noexcept qualifiers are preserved in the generated signature family.

### 10. Member function pointer with explicit signature

```cpp
template <typename Signature,
          typename MemFuncPtr = detail::get_member_fn_type_t<Signature>,
          std::size_t BufferSize = sizeof(MemFuncPtr)>
EMBED_NODISCARD inline fn<Signature, BufferSize>
make_fn(MemFuncPtr memfunc_ptr) noexcept;
```

Creates an `ebd::fn` from a pointer to member function using the specified signature.

### 11. Pointer to member object

```cpp
template <typename Class, typename T,
          typename Ret = typename detail::invoke_result<T Class::*, Class&>::type>
EMBED_NODISCARD inline auto make_fn(T Class::* ptr_memobj) noexcept
-> fn<Ret(Class&) const noexcept(detail::is_nothrow_invocable_r<Ret, T Class::*, Class&>::value),
     sizeof(ptr_memobj)>;
```

Creates an `ebd::fn` that reads a member object from an instance. The noexcept qualifier is deduced from the member access expression.

### 12. In-place construction (C++17+)

```cpp
template <typename Functor, typename... CArgs>
EMBED_NODISCARD inline auto
make_fn(std::in_place_type_t<Functor>, CArgs&&... args)
noexcept(std::is_nothrow_constructible<Functor, CArgs...>::value);

template <typename Functor, typename U, typename... CArgs>
EMBED_NODISCARD inline auto
make_fn(std::in_place_type_t<Functor>, std::initializer_list<U> il, CArgs&&... args)
noexcept(std::is_nothrow_constructible<Functor, std::initializer_list<U>&, CArgs...>::value);
```

Constructs the callable directly inside the wrapper buffer. The returned wrapper type is deduced from the functor, and the buffer size and alignment are deduced from `sizeof(Functor)` and `alignof(Functor)` respectively.

### 13. From `std::constant_wrapper` (C++26+)

```cpp
template <auto Cw, typename Fn>
EMBED_NODISCARD constexpr auto make_fn(std::constant_wrapper<Cw, Fn>) noexcept;
```

Creates an `ebd::fn_ref` from a `std::constant_wrapper` (P3948) of a free function or other callable. The signature is deduced automatically. Only available when `__cpp_lib_constant_wrapper >= 202603L`.

> [!WARNING] Deprecated
> This overload is **deprecated**: its API will be changed in v2.4.0. Use `make_fn<ebd::fn_ref>(std::cw<...>)` instead.

### 14. From `std::constant_wrapper` of a member pointer and an object (C++26+)

```cpp
template <auto Cw, typename Fn, typename Tp>
EMBED_NODISCARD constexpr auto make_fn(std::constant_wrapper<Cw, Fn>, Tp&& obj) noexcept;
```

Creates an `ebd::fn_ref` from a `std::constant_wrapper` together with an object (`obj` or `&obj`). The object binds to the **first parameter** of the wrapped callable (the *instance* for a member function or member object pointer, or the *first argument* of a free function), and that parameter is removed from the deduced signature.

> [!WARNING] Deprecated
> This overload is **deprecated**: its API will be changed in v2.4.0. Use `make_fn<ebd::fn_ref>(std::cw<...>, obj)` instead.

### 15. Explicit wrapper type

```cpp
template <template <class, std::size_t, std::size_t> class Fn,
          typename SpecifiedSig = void,
          typename... Args,
          typename Deduction = decltype(make_fn(std::declval<Args>()...)),
          typename RawSig = typename detail::is_ebd_fn<Deduction>::signature,
          typename Signature = detail::conditional_t<
              std::is_void<SpecifiedSig>::value,
              detail::get_correct_signature_t<Fn, RawSig>,
              SpecifiedSig>,
          std::size_t BufferSize =
              detail::get_correct_buffer_size<Fn<int(), 0, alignof(int*)>, Deduction::get_buffer_size(), Args...>::value,
          std::size_t Alignment = detail::is_ebd_fn<Fn<int(), 0, alignof(int*)>>::config::isView ?
              detail::default_values::non_owning::alignment : Deduction::get_alignment(),
          typename FnWrapper = Fn<Signature, BufferSize, Alignment>,
          bool NoThrow = noexcept(FnWrapper(std::declval<Args>()...))>
EMBED_NODISCARD inline FnWrapper make_fn(Args&&... args) noexcept(NoThrow);
```

Creates a wrapper with an explicitly chosen wrapper template such as `ebd::fn`, `ebd::unique_fn`, `ebd::classic_fn`, or `ebd::fn_ref`. Accepts multiple arguments: the wrapper type and signature are deduced from `make_fn(args...)`, and the buffer size is taken from the deduced result (when an argument is another ebd wrapper that is not directly config-convertible, the buffer is sized to fit that wrapper object for indirect wrapping). For owning wrappers the alignment is taken from the deduced result, while view wrappers use the non-owning default alignment. If `SpecifiedSig` is omitted, the signature is deduced from `make_fn(args...)`. This also enables in-place construction with a specific wrapper, e.g. `make_fn<ebd::fn>(std::in_place_type<Functor>, 0)`.

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

void foo() {}

auto fn1 = ebd::make_fn(&foo);
fn1();

auto fn2 = ebd::make_fn([](int x) { return x + 1; });
int y = fn2(41);

auto fn3 = ebd::make_fn<void(int)>([](int x) { /* ... */ });
fn3(42);

auto fn4 = ebd::make_fn<void()>();
if (!fn4) {
    // Handle empty wrapper
}
```

### Member Pointers

```cpp
struct MyClass {
    void method(int x) { value += x; }
    int value = 42;
};

MyClass obj;

auto mem_fn = ebd::make_fn(&MyClass::method);
mem_fn(obj, 8);

auto mem_obj = ebd::make_fn(&MyClass::value);
int value = mem_obj(obj);
```

### Explicit Wrapper Type

```cpp
auto safe = ebd::make_fn<ebd::classic_fn>([]() { /* ... */ });

void bar() {}
auto ref = ebd::make_fn<ebd::fn_ref>(&bar);

auto ref2 = ebd::make_fn<ebd::fn_ref, void()>(bar);

// Multiple arguments, e.g. in-place construction with a specific wrapper.
struct Counter {
    explicit Counter(int v) : value(v) {}
    int operator()() const { return value; }
    int value;
};
auto fn = ebd::make_fn<ebd::fn>(std::in_place_type<Counter>, 42);
int value = fn();
```

### In-place Construction

```cpp
struct Functor {
    explicit Functor(int x) : value(x) {}
    int operator()() const { return value; }
    int value;
};

auto fn = ebd::make_fn(std::in_place_type<Functor>, 42);
int value = fn();
```

### `std::constant_wrapper` (C++26+)

```cpp
void free_func(int a, int b) { /* ... */ }
struct MyClass {
    void method(int a, int b) { /* ... */ }
    int value;
};

// From a constant_wrapper of a free function.
auto cw_fn = ebd::make_fn<ebd::fn_ref>(std::cw<&free_func>);

// From a constant_wrapper of a member function + instance.
MyClass obj;
auto cw_mem = ebd::make_fn<ebd::fn_ref>(std::cw<&MyClass::method>, obj);

// From a constant_wrapper of a member object + instance.
auto cw_mem_obj = ebd::make_fn<ebd::fn_ref>(std::cw<&MyClass::value>, &obj);
```

## Notes

- `ebd::make_fn` returns `ebd::fn` for copyable deduced callables and `ebd::unique_fn` for move-only deduced callables.
- The buffer size and alignment of the resulting wrapper are deduced from the functor (`sizeof`/`alignof`). For over-aligned functors (with `alignof` greater than `detail::default_values::owning::alignment`), the alignment is automatically raised so the wrapper can store the object safely.
- When the callable is ambiguous, such as an overloaded function, specify the signature explicitly.
- The explicit-wrapper overload accepts multiple arguments and works with `ebd::fn`, `ebd::unique_fn`, `ebd::classic_fn`, and `ebd::fn_ref`. The buffer size is deduced from the `make_fn(args...)` result instead of `sizeof(Functor)`.
- `ebd::fn_view` is still available as a deprecated alias of `ebd::fn_ref`.
- When deduction fails, the fallback overload triggers a static assertion with guidance.
- The `std::constant_wrapper` overloads (C++26+) return `ebd::fn_ref` and are `constexpr`. They replace the removed `ebd::fn_ref` CTAD deduction guides for `std::constant_wrapper`.

## See Also

- [`ebd::fn`](./fn.md) - Copyable owning wrapper
- [`ebd::unique_fn`](./unique_fn.md) - Move-only owning wrapper
- [`ebd::classic_fn`](./classic_fn.md) - Classic wrapper with `std::bad_function_call` on empty
- [`ebd::fn_ref`](./fn_ref.md) - Non-owning wrapper
