# `ebd::fn`

## Overview

`ebd::fn` is a function object wrapper for copyable and callable objects. It is an alias of `ebd::detail::function` with specific configuration parameters optimized for copyable callables.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `BufferSize` | Buffer size used for storing the callable object. Defaults to `detail::default_values::owning::buffer_size` if omitted. |
| `Alignment` | Alignment of the internal storage (in bytes). Defaults to `detail::default_values::owning::alignment` if omitted. |

## Configuration

`ebd::fn` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `true` | The callable object must be copyable. |
| `IsView` | `false` | This is not a view; the wrapper owns the callable object. |
| `IsThrowing` | `false` | The wrapper will call `std::terminate()` when called in an empty state. |
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

### From `std::constant_wrapper` (C++26+)

```cpp
struct MyClass {
    void method(int a, int b) { /* do something */ }
};

// Bind an instance to a member function constant_wrapper; the first parameter (`MyClass&`)
// is removed from the signature and the instance is stored inside the wrapper.
MyClass obj;
ebd::fn<void(int, int)> fn(std::cw<&MyClass::method>, obj);
fn(0, 42);

// Construct the instance in place inside the wrapper buffer instead of copying an
// existing one. The buffer size must be large enough to hold `MyClass`.
ebd::fn<void(int, int), sizeof(MyClass)> fn2(std::cw<&MyClass::method>, std::in_place_type<MyClass>);
fn2(0, 42);

// The in-place form also accepts constructor arguments and a leading `std::initializer_list`.
struct Sum {
    Sum(std::initializer_list<int> il) : value(0) {
        for (int i : il) { value += i; }
    }
    int sum() const { return value; }
    int value;
};
ebd::fn<int(), sizeof(Sum)> fn3(std::cw<&Sum::sum>, std::in_place_type<Sum>, {1, 2, 3});
int total = fn3(); // total == 6

// A free-function constant_wrapper can also be stored (see ebd::make_fn).
```

`ebd::fn` can be constructed from a `std::constant_wrapper` (P3948) together with an object, which is stored by value in the wrapper buffer, or by in-place constructing that object from `std::in_place_type<T>` plus constructor arguments. This is available when `__cpp_lib_constant_wrapper >= 202603L` (C++26).

## Notes

- `ebd::fn` is copyable and owns the callable object it wraps.
- The buffer size is automatically aligned to a multiple of the `Alignment` parameter.
- If the callable object is too large or has a larger alignment than the specified `BufferSize`/`Alignment`, a `static_assert` will be triggered at compile time.
- When called in an empty state, `ebd::fn` will call `std::terminate()`.

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::classic_fn`](./classic_fn.md) - For callables with `std::bad_function_call` on empty
- [`ebd::fn_ref`](./fn_ref.md) - For non-owning views of callables