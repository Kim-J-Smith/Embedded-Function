# `ebd::classic_fn`

## Overview

`ebd::classic_fn` is a function object wrapper for copyable and callable objects. It is an alias of `ebd::detail::function` with specific configuration parameters that match the traditional `std::function` behavior: calling an empty wrapper throws `std::bad_function_call`. (if exceptions are enabled)

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Buffer size used for storing the callable object. Defaults to `detail::default_values::owning::buffer_size` if omitted. |
| `Alignment` | Alignment of the internal storage (in bytes). Defaults to `detail::default_values::owning::alignment` if omitted. |

## Configuration

`ebd::classic_fn` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `true` | The callable object must be copyable. |
| `IsView` | `false` | This is not a view; the wrapper owns the callable object. |
| `IsThrowing` | `true` | The wrapper will throw `std::bad_function_call` when called in an empty state. |
| `AssertObjectNoThrow` | `false` | The callable object does not need to be nothrow-constructible or nothrow-destructible. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::classic_fn`. For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a classic function wrapper for a void(int) signature
ebd::classic_fn<void(int)> fn;

// Assign a function pointer
void foo(int x) { /* do something */ }
fn = &foo;
fn(42);

// Assign a lambda
fn = [](int x) { /* do something */ };
fn(42);
```

### With Custom Buffer Size

```cpp
// Create a classic function wrapper with a custom buffer size
ebd::classic_fn<void(int), 32> fn; // 32-byte buffer

// Assign a larger lambda with captures
int value = 42;
fn = [value](int x) { /* use value */ };
fn(100);
```

## Notes

- `ebd::classic_fn` is copyable and owns the callable object it wraps.
- The buffer size is automatically aligned to a multiple of the `Alignment` parameter.
- If the callable object is too large or has a larger alignment than the specified `BufferSize`/`Alignment`, a `static_assert` will be triggered at compile time.
- When called in an empty state, `ebd::classic_fn` will throw `std::bad_function_call` (if exceptions are enabled), matching `std::function` behavior.

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables (terminate on empty)
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::fn_ref`](./fn_ref.md) - For non-owning views of callables
