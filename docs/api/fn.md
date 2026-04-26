# `ebd::fn`

## Overview

`ebd::fn` is a function object wrapper for copyable and callable objects. It is an alias of `ebd::detail::function` with specific configuration parameters optimized for copyable callables.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Buffer size used for storing the callable object. Defaults to `detail::default_buffer_size::value` if omitted. |

## Configuration

`ebd::fn` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `true` | The callable object must be copyable. |
| `IsView` | `false` | This is not a view; the wrapper owns the callable object. |
| `IsThrowing` | `true` | The wrapper will throw `std::bad_function_call` when called in an empty state. |
| `AssertObjectNoThrow` | `false` | The callable object does not need to be nothrow-constructible or nothrow-destructible. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::fn`. For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a function wrapper for a void(int) signature
ebd::fn<void(int)> fn;

// Assign a function pointer
void foo(int x) { /* do something */ }
fn = &foo;
fn(42);

// Assign a lambda
fn = [](int x) { /* do something */ };
fn(42);

// Assign a functor
struct Functor {
    void operator()(int x) { /* do something */ }
};
fn = Functor{};
fn(42);
```

### With Custom Buffer Size

```cpp
// Create a function wrapper with a custom buffer size
ebd::fn<void(int), 32> fn; // 32-byte buffer

// Assign a larger lambda with captures
int value = 42;
fn = [value](int x) { /* use value */ };
fn(100);
```

## Notes

- `ebd::fn` is copyable and owns the callable object it wraps.
- The buffer size is automatically aligned to the nearest word size.
- If the callable object is too large for the specified buffer size, a `static_assert` will be triggered at compile time.
- When called in an empty state, `ebd::fn` will throw `std::bad_function_call` (if exceptions are enabled).

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::safe_fn`](./safe_fn.md) - For exception-safe callables
- [`ebd::fn_view`](./fn_view.md) - For non-owning views of callables