# `ebd::basic_fn`

## Overview

`ebd::basic_fn` is a basic function wrapper that users can customize. It is an alias of `ebd::detail::function` that allows direct specification of all configuration parameters, providing the most flexible way to instantiate a function wrapper.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Size of the internal storage (in bytes). The value will be automatically aligned to a multiple of `Alignment`. |
| `Alignment` | Alignment of the internal storage (in bytes). |
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
template <
    typename Signature,
    std::size_t BufferSize = ebd::detail::default_values::owning::buffer_size,
    std::size_t Alignment = ebd::detail::default_values::owning::alignment
>
using unique_safe_fn = ebd::basic_fn<
    Signature,
    BufferSize,
    Alignment,
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

## Notes

- Prefer using the predefined aliases (`ebd::fn`, `ebd::unique_fn`, `ebd::classic_fn`, `ebd::fn_ref`) unless you need a combination not covered by them.
- The buffer size is automatically aligned to a multiple of `Alignment`.
- If the callable object is too large or requires a larger alignment than the specified `BufferSize`/`Alignment`, a `static_assert` will be triggered at compile time.

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::classic_fn`](./classic_fn.md) - For callables with `std::bad_function_call` on empty
- [`ebd::fn_ref`](./fn_ref.md) - For non-owning views of callables