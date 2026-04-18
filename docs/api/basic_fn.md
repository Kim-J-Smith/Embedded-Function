# `ebd::basic_fn`

## Overview

`ebd::basic_fn` is a basic function wrapper that users can customize. It is an alias of `ebd::detail::function` that allows direct specification of all configuration parameters, providing the most flexible way to instantiate a function wrapper.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Size of the internal storage (in bytes). The value will be automatically aligned. |
| `IsCopyable` | If `true`, the stored callable object must be copy-constructible; otherwise, move-only is sufficient. |
| `IsView` | If `true`, the wrapper acts as a non-owning view (no copy/move/destroy of the target). |
| `IsThrowing` | If `true`, calling an empty wrapper throws `std::bad_function_call` (if exceptions are enabled); otherwise, `std::terminate` is called. |
| `AssertObjectNoThrow` | If `true`, the wrapper requires that the callable object's construction, destruction, copy, and move operations are `noexcept`. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::basic_fn`. For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Custom Move-Only, Non-Throwing Wrapper

```cpp
#include "embed/embed_function.hpp"

// Define a custom move-only, non-throwing function wrapper
template <typename Signature, std::size_t BufferSize = ebd::detail::default_buffer_size::value>
using unique_safe_fn = ebd::basic_fn<
    Signature,
    BufferSize,
    false, // IsCopyable (move-only)
    false, // IsView (owning)
    false, // IsThrowing (call std::terminate() when empty)
    true   // AssertObjectNoThrow (require noexcept operations)
>;

// Usage
unique_safe_fn<void(int)> fn;

// Assign a move-only, noexcept lambda
std::unique_ptr<int> value = std::make_unique<int>(42);
fn = [value = std::move(value)](int x) noexcept { /* use value */ };
fn(100);
```

### Custom View Wrapper with Larger Buffer

```cpp
// Define a custom view wrapper with a larger buffer
template <typename Signature>
using large_fn_view = ebd::basic_fn<
    Signature,
    64,    // Larger buffer size
    true,  // IsCopyable (views are copyable)
    true,  // IsView (non-owning)
    false, // IsThrowing (call std::terminate() when empty)
    false  // AssertObjectNoThrow (no requirement)
>;

// Usage
void foo(int x) { /* do something */ }
large_fn_view<void(int)> view = &foo;
view(42);
```

## Notes

- `ebd::basic_fn` is marked as **unstable and experimental** in v2.0.x. The exact set of template parameters and their semantics may change in future versions.
- Prefer using the predefined aliases (`ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn`, `ebd::fn_view`) unless you need a combination not covered by them.
- The buffer size is automatically aligned to the nearest word size.
- If the callable object is too large for the specified buffer size, a `static_assert` will be triggered at compile time.

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::safe_fn`](./safe_fn.md) - For exception-safe callables
- [`ebd::fn_view`](./fn_view.md) - For non-owning views of callables