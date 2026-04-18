# `ebd::safe_fn`

## Overview

`ebd::safe_fn` is a safe function object wrapper for copyable and callable objects. It is an alias of `ebd::detail::function` with specific configuration parameters optimized for exception-safe operations.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Buffer size used for storing the callable object. Defaults to `detail::default_buffer_size::value` if omitted. |

## Configuration

`ebd::safe_fn` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `true` | The callable object must be copyable. |
| `IsView` | `false` | This is not a view; the wrapper owns the callable object. |
| `IsThrowing` | `false` | The wrapper will not throw `std::bad_function_call` when called in an empty state; instead, it will call `std::terminate()`. |
| `AssertObjectNoThrow` | `true` | The callable object must be nothrow-constructible, nothrow-destructible, nothrow-copy-constructible, and nothrow-move-constructible. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::safe_fn`. For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a safe function wrapper for a void(int) signature
ebd::safe_fn<void(int)> fn;

// Assign a function pointer
void foo(int x) noexcept { /* do something */ }
fn = &foo;
fn(42);

// Assign a noexcept lambda
fn = [](int x) noexcept { /* do something */ };
fn(42);

// Assign a noexcept functor
struct NoThrowFunctor {
    void operator()(int x) noexcept { /* do something */ }
};
fn = NoThrowFunctor{};
fn(42);
```

### With Custom Buffer Size

```cpp
// Create a safe function wrapper with a custom buffer size
ebd::safe_fn<void(int), 32> fn; // 32-byte buffer

// Assign a larger noexcept lambda with captures
int value = 42;
fn = [value](int x) noexcept { /* use value */ };
fn(100);
```

## Notes

- `ebd::safe_fn` is copyable and owns the callable object it wraps.
- The buffer size is automatically aligned to the nearest word size.
- The callable object must be noexcept in all operations (construction, destruction, copy, move).
- If the callable object is too large for the specified buffer size, a `static_assert` will be triggered at compile time.
- When called in an empty state, `ebd::safe_fn` will call `std::terminate()` instead of throwing an exception.

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables (non-noexcept)
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::fn_view`](./fn_view.md) - For non-owning views of callables