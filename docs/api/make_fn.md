# `ebd::make_fn`

## Overview

`ebd::make_fn` is a factory function that creates function wrapper objects. It automatically deduces the signature and buffer size of the callable object, and returns an appropriate function wrapper (`ebd::fn` or `ebd::unique_fn`).

## Overloads

### 1. Make function with specified signature for copyable functor

```cpp
template <typename Signature, typename Functor>
EMBED_NODISCARD inline fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept;
```

Creates an `ebd::fn` with the specified signature for a copyable functor.

### 2. Make function with specified signature for move-only functor

```cpp
template <typename Signature, typename Functor>
EMBED_NODISCARD inline unique_fn<Signature, sizeof(Functor)>
make_fn(Functor&& functor) noexcept;
```

Creates an `ebd::unique_fn` with the specified signature for a move-only functor.

### 3. Make an empty function with specified signature and buffer size

```cpp
template <typename Signature, std::size_t BufferSize = detail::default_buffer_size::value>
EMBED_NODISCARD inline fn<Signature, BufferSize>
make_fn(std::nullptr_t = nullptr) noexcept;
```

Creates an empty `ebd::fn` with the specified signature and buffer size.

### 4. Make function for function pointer (auto deduce signature and buffer size)

```cpp
template <typename Ret, typename... Args>
EMBED_NODISCARD inline fn<Ret(Args...) const, sizeof(Ret(*)(Args...))>
make_fn(Ret (*func_ptr) (Args...)) noexcept;
```

Creates an `ebd::fn` from a function pointer, automatically deducing the signature and buffer size.

### 5. Make function for function pointer with specified signature

```cpp
template <typename Signature, typename FunctionPtr = typename detail::unwrap_signature<Signature>::pure_sig*>
EMBED_NODISCARD inline fn<Signature, sizeof(FunctionPtr)>
make_fn(FunctionPtr func_ptr) noexcept;
```

Creates an `ebd::fn` from a function pointer with the specified signature.

### 6. Create a function from another function (Copy)

```cpp
template <std::size_t Buf, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Cfg, Sig>
make_fn(const detail::function<Buf, Cfg, Sig>& fn) noexcept;
```

Creates a function wrapper by copying another function wrapper.

### 7. Create a function from another function (Move)

```cpp
template <std::size_t Buf, typename Cfg, typename Sig>
EMBED_NODISCARD inline detail::function<Buf, Cfg, Sig>
make_fn(detail::function<Buf, Cfg, Sig>&& fn) noexcept;
```

Creates a function wrapper by moving another function wrapper.

### 8. Make a function from lambda or unique-operator() functor

```cpp
template <typename Lambda, typename Class = detail::remove_cvref_t<Lambda>,
          std::size_t BufferSize = sizeof(Class),
          typename Signature = detail::get_unique_signature_t<Class>,
          typename Fn = detail::conditional_t<
              std::is_copy_constructible<Class>::value,
              fn<Signature, BufferSize>, unique_fn<Signature, BufferSize>
          >>
EMBED_NODISCARD inline Fn make_fn(Lambda&& fn) noexcept;
```

Creates a function wrapper from a lambda or functor with a unique `operator()`, automatically deducing the signature and buffer size.

### 9. Make function for pointer to member function

```cpp
template <typename Class, typename Ret, typename... Args>
EMBED_NODISCARD inline auto
make_fn(Ret(Class::* memfunc)(Args...) C V REF NOEXCEPT) noexcept
-> fn<Ret(detail::get_qualified_with_t<int REF, C V Class>, Args...) const, sizeof(memfunc)>;
```

Creates an `ebd::fn` from a pointer to member function, automatically deducing the signature and buffer size.

### 10. Make function for member function pointer with specified signature

```cpp
template <typename Signature, typename MemFuncPtr = detail::get_member_fn_type_t<Signature>,
          std::size_t BufferSize = sizeof(MemFuncPtr)>
EMBED_NODISCARD inline fn<Signature, BufferSize>
make_fn(MemFuncPtr memfunc_ptr) noexcept;
```

Creates an `ebd::fn` from a member function pointer with the specified signature.

### 11. Make function for pointer to member object

```cpp
template <typename Class, typename T>
EMBED_NODISCARD inline auto make_fn(T Class::* ptr_memobj) noexcept
-> fn<T(Class&) const, sizeof(ptr_memobj)>;
```

Creates an `ebd::fn` from a pointer to member object.

### 12. In-place make function (C++17+)

```cpp
template <typename Functor, typename... CArgs>
EMBED_NODISCARD inline auto make_fn(std::in_place_type_t<Functor>, CArgs&&... args) noexcept;

template <typename Functor, typename U, typename... CArgs>
EMBED_NODISCARD inline auto
make_fn(std::in_place_type_t<Functor>, std::initializer_list<U> il, CArgs&&... args) noexcept;
```

Creates a function wrapper by in-place constructing the callable object within the internal buffer.

### 13. Make function with specified wrapper

```cpp
template <template <class, std::size_t> class Fn, typename Functor,
          typename Deduction = decltype(make_fn(std::declval<Functor>())),
          typename Signature = typename detail::is_ebd_fn<Deduction>::signature,
          std::size_t BufferSize = sizeof(detail::decay_t<Functor>),
          typename FnWrapper = Fn<Signature, BufferSize>>
EMBED_NODISCARD inline FnWrapper make_fn(Functor&& functor) noexcept;
```

Creates a function wrapper with the specified wrapper type (e.g., `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn`, or `ebd::fn_view`).

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a function wrapper from a function pointer
void foo() { /* do something */ }
auto fn1 = ebd::make_fn(&foo);
fn1();

// Create a function wrapper from a lambda
auto fn2 = ebd::make_fn([]() { /* do something */ });
fn2();

// Create a function wrapper with specified signature
auto fn3 = ebd::make_fn<void(int)>([](int x) { /* do something */ });
fn3(42);

// Create an empty function wrapper
auto fn4 = ebd::make_fn<void()>();
if (!fn4) {
    // Handle empty function
}
```

### With Member Functions

```cpp
struct MyClass {
    void method(int x) { /* do something */ }
    int value = 42;
};

MyClass obj;

// Create a function wrapper for a member function
auto mem_fn = ebd::make_fn(&MyClass::method);
mem_fn(obj, 100); // Call obj.method(100)

// Create a function wrapper for a member variable
auto mem_obj = ebd::make_fn(&MyClass::value);
int val = mem_obj(obj); // Get obj.value
```

### With Custom Wrapper Type

```cpp
// Create a safe_fn from a lambda
auto safe_fn = ebd::make_fn<ebd::safe_fn>([]() noexcept { /* do something */ });

// Create a fn_view from a function pointer
void bar() { /* do something */ }
auto view = ebd::make_fn<ebd::fn_view>(&bar);
```

### In-place Construction (C++17+)

```cpp
// In-place construct a functor within the function wrapper
auto fn = ebd::make_fn(std::in_place_type<MyFunctor>, /* constructor arguments */);
```

## Notes

- `ebd::make_fn` automatically deduces the appropriate function wrapper type based on the callable object:
  - Returns `ebd::fn` for copyable callables
  - Returns `ebd::unique_fn` for move-only callables
- The buffer size is automatically set to the size of the callable object.
- For ambiguous callables (e.g., overloaded functions), you must specify the signature explicitly.
- If `ebd::make_fn` cannot deduce an appropriate function wrapper, it will trigger a static assertion with a helpful error message.

## See Also

- [`ebd::fn`](./fn.md) - Copyable function wrapper
- [`ebd::unique_fn`](./unique_fn.md) - Move-only function wrapper
- [`ebd::safe_fn`](./safe_fn.md) - Exception-safe function wrapper
- [`ebd::fn_view`](./fn_view.md) - Non-owning view of callables