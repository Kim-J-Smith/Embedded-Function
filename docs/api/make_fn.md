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
          bool NoThrow = detail::is_nothrow_construct_from_functor<Functor&&>::value>
EMBED_NODISCARD inline fn<Signature, sizeof(Class)>
make_fn(Functor&& functor) noexcept(NoThrow);
```

Creates an `ebd::fn` for a class-type callable when the signature is specified explicitly. The buffer size is automatically deduced as `sizeof(Class)`.

### 2. Move-only functor with explicit signature

```cpp
template <typename Signature, typename Functor,
          bool NoThrow = std::is_nothrow_move_constructible<Functor>::value>
EMBED_NODISCARD inline unique_fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept(NoThrow);
```

Creates an `ebd::unique_fn` for a move-only functor when the signature is specified explicitly.

### 3. Empty wrapper with explicit signature

```cpp
template <typename Signature,
          std::size_t BufferSize = detail::default_buffer_size::value>
EMBED_NODISCARD inline fn<Signature, BufferSize>
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
EMBED_NODISCARD inline fn<Ret(Args...) const noexcept, sizeof(Ret(*)(Args...))>
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
template <std::size_t Buf, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Cfg, Sig>
make_fn(const detail::function<Buf, Cfg, Sig>& fn)
noexcept(Cfg::isView || Cfg::assertNoThrow);
```

Creates a wrapper by copying another `ebd::detail::function`.

### 7. Move from another wrapper

```cpp
template <std::size_t Buf, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Cfg, Sig>
make_fn(detail::function<Buf, Cfg, Sig>&& fn)
noexcept(Cfg::isView || Cfg::assertNoThrow);
```

Creates a wrapper by moving another `ebd::detail::function`.

### 8. Lambda or uniquely callable functor

```cpp
template <typename Lambda,
          typename Class = detail::remove_cvref_t<Lambda>,
          std::size_t BufferSize = sizeof(Class),
          typename Signature = detail::get_unique_signature_t<Class>,
          typename Fn = detail::conditional_t<
              std::is_copy_constructible<Class>::value,
              fn<Signature, BufferSize>,
              unique_fn<Signature, BufferSize>>,
          bool NoThrow = detail::is_nothrow_construct_from_functor<Lambda&&>::value>
EMBED_NODISCARD inline Fn make_fn(Lambda&& fn) noexcept(NoThrow);
```

Creates a wrapper from a lambda or other functor with exactly one viable `operator()`. The signature is deduced automatically.

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

Constructs the callable directly inside the wrapper buffer. The returned wrapper type is deduced from the functor.

### 13. Explicit wrapper type

```cpp
template <template <class, std::size_t> class Fn,
          typename SpecifiedSig = void,
          typename Functor,
          typename Deduction = decltype(make_fn(std::declval<Functor>())),
          typename RawSig = typename detail::is_ebd_fn<Deduction>::signature,
          typename Signature = detail::conditional_t<
              std::is_void<SpecifiedSig>::value,
              detail::get_correct_signature_t<Fn, RawSig>,
              SpecifiedSig>,
          std::size_t BufferSize = sizeof(detail::decay_t<Functor>),
          typename FnWrapper = Fn<Signature, BufferSize>,
          bool NoThrow = noexcept(FnWrapper(std::declval<Functor>()))>
EMBED_NODISCARD inline FnWrapper make_fn(Functor&& functor) noexcept(NoThrow);
```

Creates a wrapper with an explicitly chosen wrapper template such as `ebd::fn`, `ebd::unique_fn`, `ebd::classic_fn`, or `ebd::fn_ref`. If `SpecifiedSig` is omitted, the signature is deduced from `make_fn(functor)`.

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

## Notes

- `ebd::make_fn` returns `ebd::fn` for copyable deduced callables and `ebd::unique_fn` for move-only deduced callables.
- When the callable is ambiguous, such as an overloaded function, specify the signature explicitly.
- The explicit-wrapper overload accepts `ebd::fn`, `ebd::unique_fn`, `ebd::classic_fn`, and `ebd::fn_ref`.
- `ebd::fn_view` is still available as a deprecated alias of `ebd::fn_ref`.
- When deduction fails, the fallback overload triggers a static assertion with guidance.

## See Also

- [`ebd::fn`](./fn.md) - Copyable owning wrapper
- [`ebd::unique_fn`](./unique_fn.md) - Move-only owning wrapper
- [`ebd::classic_fn`](./classic_fn.md) - Classic wrapper with `std::bad_function_call` on empty
- [`ebd::fn_ref`](./fn_ref.md) - Non-owning wrapper
