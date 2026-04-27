# `ebd::detail::function`

## Overview

`ebd::detail::function` is the core implementation class for all function wrappers in the Embedded Function library. It provides a lightweight, heap-free wrapper for callable objects, similar to `std::function` but with reduced overhead and deterministic performance characteristics.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `BufferSize` | Specifies the size reserved to store the callable object. Must be at least `sizeof(void*)`. |
| `Config` | Specifies the configuration attributes of the wrapper. Must be a `config_package` type. |
| `Signature` | The function signature of the wrapper, e.g., `Ret(Args...)` or `Ret(Args...) const`. |

## Configuration Package

The `Config` parameter is a `config_package` struct with the following boolean fields:

| Field | Description |
|-------|-------------|
| `isCopyable` | Whether the function wrapper is copyable. |
| `isView` | Whether the function wrapper is a non-owning view. |
| `isThrowing` | Whether the wrapper throws `std::bad_function_call` when called in an empty state. |
| `assertNoThrow` | Whether the wrapper asserts that the callable object is nothrow-constructible and nothrow-destructible. |

## Member Types

| Type | Description |
|------|-------------|
| `result_type` | The return type of the function signature. |

## Member Functions

### Constructor

#### Default Constructor

```cpp
function() noexcept;
```

Creates an empty function wrapper.

#### Nullptr Constructor

```cpp
function(std::nullptr_t) noexcept;
```

Creates an empty function wrapper.

#### Copy Constructor

```cpp
function(const function& other) = default;
```

Copies another function wrapper. Only available if `Config::isCopyable` or `Config::isView` is `true`.

#### Move Constructor

```cpp
function(function&& other) = default;
```

Moves another function wrapper.

#### Conversion Constructor

```cpp
template <std::size_t OtherSize, typename OtherCfg, typename OtherSig>
function(const function<OtherSize, OtherCfg, OtherSig>& other);

template <std::size_t OtherSize, typename OtherCfg, typename OtherSig>
function(function<OtherSize, OtherCfg, OtherSig>&& other);
```

Converts from another function wrapper with a different buffer size or configuration, if compatible.

#### Functor Constructor

```cpp
template <typename Functor>
function(Functor&& functor);
```

Constructs a function wrapper from a callable object. The callable object must be compatible with the signature and fit within the buffer size.

#### In-place Constructor (C++17+)

```cpp
template <typename Fn, typename... CArgs>
explicit function(std::in_place_type_t<Fn>, CArgs&&... args);

template <typename Fn, typename U, typename... CArgs>
explicit function(std::in_place_type_t<Fn>, std::initializer_list<U> il, CArgs&&... args);
```

Constructs a function wrapper by in-place constructing the callable object within the internal buffer.

### Assignment Operators

#### Nullptr Assignment

```cpp
function& operator=(std::nullptr_t) noexcept;
```

Assigns a null pointer, clearing the function wrapper.

#### Copy Assignment

```cpp
function& operator=(const function& other) = default;
```

Copies another function wrapper. Only available if `Config::isCopyable` or `Config::isView` is `true`.

#### Move Assignment

```cpp
function& operator=(function&& other) = default;
```

Moves another function wrapper.

#### Functor Assignment

```cpp
template <typename Functor>
function& operator=(Functor&& fn) noexcept;
```

Assigns a callable object to the function wrapper.

#### Conversion Assignment

```cpp
template <std::size_t OtherSize, typename OtherCfg, typename OtherSig>
function& operator=(const function<OtherSize, OtherCfg, OtherSig>& other);
```

Assigns from another function wrapper with a different buffer size or configuration, if compatible.

### Invocation

```cpp
Ret operator()(Args... args) C V REF NOEXCEPT;
```

Invokes the wrapped callable object with the given arguments. The cv-qualifiers and ref-qualifiers match those in the signature.

### State Management

#### is_empty

```cpp
constexpr bool is_empty() const noexcept;
```

Returns `true` if the function wrapper is empty, `false` otherwise.

#### operator bool

```cpp
constexpr explicit operator bool() const noexcept;
```

Returns `true` if the function wrapper is not empty, `false` otherwise.

#### clear

```cpp
void clear() noexcept(Config::assertNoThrow);
```

Clears the function wrapper, destroying the wrapped callable object if necessary.

#### swap

```cpp
void swap(function& fn) noexcept(Config::assertNoThrow);
```

Swaps the contents of two function wrappers.

### Utility

#### get_buffer_size

```cpp
static constexpr std::size_t get_buffer_size() noexcept;
```

Returns the buffer size of the function wrapper.

#### is_copyable

```cpp
static constexpr bool is_copyable() noexcept;
```

Returns `true` if the function wrapper is copyable, `false` otherwise.

#### operator*

```cpp
function_ptr_t operator*() const noexcept;
```

If the wrapped object is a function pointer, returns that pointer; otherwise, returns `nullptr`.

## Non-member Functions

### Comparison Operators

```cpp
template <std::size_t Buf, typename Cfg, typename Sig>
bool operator==(const function<Buf, Cfg, Sig>& fn, std::nullptr_t) noexcept;

template <std::size_t Buf, typename Cfg, typename Sig>
bool operator==(std::nullptr_t, const function<Buf, Cfg, Sig>& fn) noexcept;

template <std::size_t Buf, typename Cfg, typename Sig>
bool operator!=(const function<Buf, Cfg, Sig>& fn, std::nullptr_t) noexcept;

template <std::size_t Buf, typename Cfg, typename Sig>
bool operator!=(std::nullptr_t, const function<Buf, Cfg, Sig>& fn) noexcept;
```

Compare a function wrapper with `nullptr` to check if it is empty.

## Performance Characteristics

- **Heap-free**: All storage is allocated inline within the function wrapper, eliminating dynamic memory allocations.
- **Branch elimination**: Runtime checks for empty function states are eliminated during invocation.
- **Smart forwarding**: Scalar arguments and small-sized trivial arguments are passed via registers instead of the stack.
- **Deterministic**: No dynamic memory operations mean predictable real-time performance.

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a function wrapper for a void() signature
ebd::detail::function<
    sizeof(void(*)()),
    ebd::detail::config_package<true, false, true, false>,
    void()
> fn;

// Assign a lambda
fn = []() { /* do something */ };

// Invoke the function
if (fn) {
    fn();
}
```

### With Custom Configuration

```cpp
// Create a move-only, non-throwing function wrapper
typedef ebd::detail::function<
    32, // 32-byte buffer
    ebd::detail::config_package<false, false, false, true>, // move-only, non-throwing, assert no-throw
    int(int, int)
> custom_fn;

custom_fn add = [](int a, int b) { return a + b; };
int result = add(10, 20); // result = 30
```

## Notes

- The `ebd::detail::function` class is not intended for direct use. Instead, use the predefined aliases such as `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn`, and `ebd::fn_view`.
- The buffer size is automatically aligned to the nearest word size.
- The function wrapper supports all callable objects, including free functions, lambdas, functors, static member functions, and member functions.
- When `Config::isView` is `true`, the wrapper acts as a non-owning view, similar to `std::function_ref`.