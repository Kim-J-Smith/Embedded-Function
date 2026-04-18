# `ebd::unique_fn`

## Overview

`ebd::unique_fn` is a function object wrapper for movable and callable objects. It is an alias of `ebd::detail::function` with specific configuration parameters optimized for move-only callables.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Buffer size used for storing the callable object. Defaults to `detail::default_buffer_size::value` if omitted. |

## Configuration

`ebd::unique_fn` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `false` | The callable object can be move-only (copyable objects are also accepted but only move operations will be used). |
| `IsView` | `false` | This is not a view; the wrapper owns the callable object. |
| `IsThrowing` | `true` | The wrapper will throw `std::bad_function_call` when called in an empty state. |
| `AssertObjectNoThrow` | `false` | The callable object does not need to be nothrow-constructible or nothrow-destructible. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::unique_fn`, except for copy operations which are deleted. For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Basic Usage with Move-Only Callables

```cpp
#include "embed/embed_function.hpp"

// Create a function wrapper for a void() signature
ebd::unique_fn<void()> fn;

// Assign a move-only lambda
fn = []() && { /* do something */ };
fn();

// Move the function wrapper
auto fn2 = std::move(fn);
fn2();

// Assign a move-only functor
struct MoveOnlyFunctor {
    MoveOnlyFunctor() = default;
    MoveOnlyFunctor(const MoveOnlyFunctor&) = delete;
    MoveOnlyFunctor& operator=(const MoveOnlyFunctor&) = delete;
    MoveOnlyFunctor(MoveOnlyFunctor&&) = default;
    MoveOnlyFunctor& operator=(MoveOnlyFunctor&&) = default;
    
    void operator()() { /* do something */ }
};

fn2 = MoveOnlyFunctor{};
fn2();
```

### With Custom Buffer Size

```cpp
// Create a function wrapper with a custom buffer size
ebd::unique_fn<void(int), 32> fn; // 32-byte buffer

// Assign a larger move-only lambda with captures
std::unique_ptr<int> value = std::make_unique<int>(42);
fn = [value = std::move(value)](int x) { /* use value */ };
fn(100);
```

## Notes

- `ebd::unique_fn` is move-only and owns the callable object it wraps.
- The buffer size is automatically aligned to the nearest word size.
- If the callable object is too large for the specified buffer size, a `static_assert` will be triggered at compile time.
- When called in an empty state, `ebd::unique_fn` will throw `std::bad_function_call` (if exceptions are enabled).

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables
- [`ebd::safe_fn`](./safe_fn.md) - For exception-safe callables
- [`ebd::fn_view`](./fn_view.md) - For non-owning views of callables