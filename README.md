# Embedded Function

<p align="center">
  <img src="https://img.shields.io/badge/Version-2.0.4-yellow?style=for-the-badge&logo=github" alt="Version - 2.0.4">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C++-11/14/17/20/23-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 11/14/17/20/23">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/GCC_C++11/14/17/20/23-passing-brightgreen?style=flat" alt="gcc-C++11/14/17/20/23 - passing">
  <img src="https://img.shields.io/badge/Clang_C++11/14/17/20/23-passing-brightgreen?style=flat" alt="clang-C++11/14/17/20/23 - passing">
  <img src="https://img.shields.io/badge/MSVC_C++14/17/20/23-passing-brightgreen?style=flat" alt="msvc-C++14/17/20/23 - passing">
</p>

> *Embedded [std::function](http://en.cppreference.com/w/cpp/utility/functional/function) alternative: lightweight, deterministic, heap-free.*

## 📌 Overview

**Embedded Function** is an embedded-friendly lightweight function wrapper implemented based on the C++11 standard, tailored specifically for embedded systems. 

While functionally and conceptually analogous to *std::function*, it offers substantially reduced overhead and superior real-time performance characteristics. **Notably, embed-function eliminates dynamic heap memory allocations entirely**, ensuring deterministic execution behavior and predictable real-time performance for embedded applications.

A function wrapper is declared as following:

```cpp
ebd::fn<int (int, float, char) const, 3*sizeof(void*)>
//       ^     ^     ^     ^     ^        ^
//       |     |     |     |     |        |
// Return type |     |     |     |        |
// Parameters ~|~~~~~|~~~~~|     |        |
// Qualifier ~~~~~~~~~~~~~~~~~~~~|        |
// Buffer size ~~~~~~~~~~~~~~~~~~~~~~~~~~~|
```

- Note: This `Qualifier` is used to restrict the callable objects wrapped within `ebd::fn`, rather than `ebd::fn` itself.

## ⚡ Quick start
- Clone the repository or download the `header_only.zip` in the "Release".

- Add include path `<repo_root>/include`.

- In program `#include "embed/embed_function.hpp"`.

- Use the `ebd::fn` template class.

```cpp
#include "embed/embed_function.hpp"

struct Example {
    static void static_mem_fn(int) {};
    void mem_fn(int) {};
    void operator()(int) {};
};

auto main() -> int {
    Example e;
    ebd::fn<void(int)> fn_;

    fn_ = &Example::static_mem_fn;
    fn_(123);

    fn_ = [e](int arg) { e.mem_fn(arg); };
    fn_(456);

    fn_ = e;
    fn_(789);
}
```

## 🧠 Design goals driving the design

  - Should behave close to a normal function pointer. Small, efficient, no heap allocation.

  - Support the packaging of all callable objects in C++, including:
    - Free function.
    - Lambda function.
    - Functor.
    - Static member function.
    - Member function.

  - Be usable with C++11 while offering more functionality for later editions.

  - Be constexpr and exception friendly. As much as possible should be declared constexpr and noexcept.

  - Following the above design goals, `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn` and `ebd::fn_view` were designed for developers to use.

## ✨ Core function wrappers

### Summary table

| Wrapper Type | Copyable | View (Non-owning) | Throws on Empty Call | Assert No-Throw (Ctor/Dtor) | Buffer Size | Primary Use Case |
| :----------- | :---: | :---: | :---: | :---: | :---: | :---: |
| `ebd::fn`    |  Yes  |   No  | Yes (`std::bad_function_call`) | No | Configurable (aligned, default: `sizeof(void(Class::*)())`) | Copyable callable wrapper |
| `ebd::unique_fn`    |  No  |   No  | Yes (`std::bad_function_call`) | No | Configurable (aligned, default: `sizeof(void(Class::*)())`) | Move-only callable wrapper |
| `ebd::safe_fn`    |  Yes  |   No  | No (`std::terminate()`) | Yes | Configurable (aligned, default: `sizeof(void(Class::*)())`) | Exception-safe copyable callable wrapper |
| `ebd::fn_view`    |  Yes  |   Yes  | No (`std::terminate()`) | No | Fixed (`sizeof(void(Class::*)())`, template param unused) | Lightweight non-owning view of callables |

### Key takeaways

1. **Ownership & Copy**: `fn`/`safe_fn` own callables (copyable), `unique_fn` owns but is move-only, `fn_view` is non-owning (view).

2. **Exception Behavior**: Only `fn`/`unique_fn` throw on empty calls; `safe_fn`/`fn_view` terminate (no exceptions).

3. **Buffer Configuration**: `fn`/`unique_fn`/`safe_fn` support configurable buffer sizes (aligned), while `fn_view` uses a fixed buffer (unused template param).

## 🧩 Automatic deduction

### Brief introduction

In order to simplify the use of `ebd::fn`, function `ebd::make_fn()` is provided, which can automatically deduce the signature and buffer size of the callable object and create a `ebd::fn` or `ebd::unique_fn` object. (Return `ebd::unique_fn` only when the callable object is of the move-only type.)

> The [Concepts](https://en.cppreference.com/w/cpp/language/constraints.html) language feature is available for use provided that the compiler is configured to support the C++20 standard.

### Usage

- `[`Optional`]` means optional.
- `Signature`: The signature of the callable object. (such as `void(int)`)
- `BufferSize`: The buffer size of the callable object. (such as `2*sizeof(void*)`)

```cpp
// Create empty ebd::fn with specified signature and buffer size.
// If the BufferSize is omitted, it will be set by default (usually 2*sizeof(void*)).
auto f = ebd::make_fn<Signature[, BufferSize]>();
auto f = ebd::make_fn<Signature[, BufferSize]>(nullptr);
```

```cpp
// Create ebd::fn or ebd::unique_fn from unambiguous callable object.
// If the Signature is omitted, the signature will be deduced from Callable_Object.
auto f = ebd::make_fn[<Signature>](Callable_Object);
```

```cpp
// Create ebd::fn or ebd::unique_fn from ambiguous callable object with specified signature, such as overload free function, overload member function, etc.
auto f = ebd::make_fn<Signature>(Ambiguous_Callable_Object);
```

## 🔗 Back to function pointer

### Brief

In embedded MCU development, it is often necessary to pass a C-style free function pointer as an argument, as existing libraries are typically written in C. To address this, we have implemented a `operator*` overload that simplifies converting an object of type `ebd::fn` / `ebd::unique_fn` / `ebd::safe_fn` / `ebd::fn_view` to a C-style free function pointer.

If the object encapsulated by the function wrapper is a valid function pointer, this mechanism returns the pointer; otherwise, it returns nullptr.

### Example

```cpp
void free_function() {}
struct Functor { void operator()() {} };

ebd::fn<void()> fn_ = &free_function;
void(*free_function_pointer)() = *fn_;
ASSERT_EQ(free_function_pointer, &free_function);

fn_ = +[]() { /* ... */ }; // lambda -> function pointer
free_function_pointer = *fn_;
ASSERT_NE(free_function_pointer, nullptr); // NOT equal nullptr

fn_ = []() { /* ... */ };
free_function_pointer = *fn_;
ASSERT_EQ(free_function_pointer, nullptr);

fn_ = Functor{};
free_function_pointer = *fn_;
ASSERT_EQ(free_function_pointer, nullptr);
```

## ✅ Compatibility

Every compiler with modern C++11 support should work.
*embed-function* only depends on the standard library.

- GCC 5.1+
- Clang 3.7+
- MSVC v19.34+ (VS17.4+)

## 🧪 Test

Go to the `<root>/test/` directory, and follow the instructions in [`HOW-TO-TEST.md`](./test/HOW-TO-TEST.md) to run the tests.

## 📚 Similar implementations

- [std::function](http://en.cppreference.com/w/cpp/utility/functional/function)

- [Naios/function2](https://github.com/Naios/function2)

- [pmed/fixed_size_function](https://github.com/pmed/fixed_size_function)

- [rigtorp/Function](https://github.com/rigtorp/Function)

- [rosbacke/delegate](https://github.com/rosbacke/delegate)

- [winterscar/functional-avr](https://github.com/winterscar/functional-avr)

- [bdiamand/Delegate](https://github.com/bdiamand/Delegate)

- [potswa/cxx_function](https://github.com/potswa/cxx_function)
