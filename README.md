# embed-function

![Version - 1.1.2](https://img.shields.io/badge/Version-1.1.2-green?style=flat&logo=github) ![License - MIT](https://img.shields.io/badge/License-MIT-orange?style=flat) ![c++ - 11/14/17/20](https://img.shields.io/badge/C++-11/14/17/20-blue?style=flat)

![gcc-c++11/14/17/20/23 - passing](https://img.shields.io/badge/GCC_C++11/14/17/20/23-passing-brightgreen?style=flat) ![clang-c++11/14/17/20/23 - passing](https://img.shields.io/badge/Clang_C++11/14/17/20/23-passing-brightgreen?style=flat) ![msvc-c++14/17/20/23 - passing](https://img.shields.io/badge/MSVC_C++14/17/20/23-passing-brightgreen?style=flat)


*Embedded friendly [`std::function`](http://en.cppreference.com/w/cpp/utility/functional/function) alternative. The usage method is consistent with `std::function`.*

## Overview

- ✅ **Easy to use**
  
  The `embed::function` is used in the same way as `std::function`.

  We also provide [`embed::make_function`](./doc/detail/make_function.md) to automatically deduce the template type.

- ✅ **No dynamic memory allocation**

  Never allocates memory on the heap. Ensure the real-time performance of the code.

- ✅ **Adjustable buffer size**

  Users can specify the buffer size used by each embed::function instance, which is used to wrap callable objects of different sizes.

  The default value of buffer size `FnDefaultBufSize` is defined at the beginning of the embed_function.hpp header file, and it is set to the size of one pointer. Users can customize this value.

- ✅ **Extremely minimal memory usage**

  By default, it occupies 2 pointer-sized(manager + buffer) amounts of RAM.

  If you need to package larger callable objects, you can manually adjust the buffer size of the `embed::function<Signature, BufSize>`.

- ✅ **Environmentally friendly for disabling exceptions**

  The `embed::function` itself supports both enabling and disabling of exceptions, but it is more recommended to use it in the mode where exceptions are disabled.

  When exceptions are disabled, calling an empty `embed::function` instance by the user will invoke the `bad_function_call_handler` function for processing (by default, it simply terminates the program), and the logic of the `bad_function_call_handler` function is at the beginning of the embed_function.hpp file. Users can customize it.

- ✅ **Support non-copyable objects**

  According to the C++ proposal [N4159](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4159.pdf), the `embed::function` class, in addition to supporting the basic functions of `std::function`, also adds support for non-copyable objects.

  When a user attempts to perform a copy operation (copy constructor, copy assignment, etc) on an `embed::function` instance that is wrapped with an uncopyable type, the program will call the `bad_function_copy_handler` function (which defaults to terminating the program). The `bad_function_copy_handler` function is located at the beginning of embed_function.hpp and is customizable.

#### A function wrapper is declared as following:

```cpp
embed::function<int(int, float, char) const, 3*sizeof(void*)>
// Return type ~~^   ^     ^      ^     ^       ^
// Parameters ~~~~~~~|~~~~~|~~~~~~|     |       |
// Qualifier ~~~~~~~~~~~~~~~~~~~~~~~~~~~|       |
// Buffer size ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
```

- Note: This "Qualifier" is used to restrict the callable objects wrapped within "embed::function", rather than "embed::function" itself.

## Quick start

- Clone the repo.
- Add include path <repo_root>/include
- In program `#include "embed/embed_function.hpp"`
- Use the `embed::function` template class.

```cpp
#include "embed/embed_function.hpp"

struct Example {
    static void staticFkn(int) {};
    void memberFkn(int) {};
    void operator()(int) {};
};

int main() 
{
    Example e;

    // Here, the second template parameter can be omitted.
    // Same as: embed::function<void(int), sizeof(void*)> fn;
    embed::function<void(int)> fn;

    fn = [&e](int p) { e.memberFkn(p); };
    fn(123);

    fn = &Example::staticFkn;
    fn(456);

    fn = e;
    fn(789);

    // The type of auto_fn is 
    // embed::function<void(Example&,int) const, sizeof(&Example::memberFkn)>
    auto auto_fn = embed::make_function(&Example::memberFkn);
    auto_fn(e, 101112);
}

```

## Design goals driving the design

  - Should behave close to a normal function pointer. Small, efficient, no heap allocation.

  - Should be used in a manner similar to `std::function`, being able to truly "wrap" callable objects and manage their lifetimes.

  - Support the packaging of all callable objects in C++, including:
    - Free function.
    - Lambda function.
    - Functor.
    - Static member function.
    - Member function. (should be wrapped by lambda function)

  - Be usable with C++11 while offering more functionality for later editions.
    - C++11 provides most of the functionality.
    - C++17 introduces a template type deduction guide, allowing users to use the underlying type `embed::Fn` (where `embed::function` is an alias for `embed::Fn`) without having to specify the template parameters.

  - Be constexpr and exception friendly. As much as possible should be declared constexpr and noexcept.

## Detail APIs

  See [doc/API_embed_function.md](./doc/API_embed_function.md) for further details.

## Solution without the C++ standard library

  In some embedded situations, the compiler may not support the standard libraries of C++, such as &lt;type_traits&gt;, &lt;new&gt;, and so on. Therefore, we provide [`embed_function_nostd.hpp`](./include/embed/embed_function_nostd.hpp) as a solution. This file implements all the standard library contents required by this project. Users only need to define the macro **`EMBED_NO_STD_HEADER`** during compilation to enable it.

## Tests

  The test cases have been provided in the [test](./test/) directory. Refer to the commands in the [test/ReadMe](./test/ReadMe.md) file for testing.

## License

```
MIT License

https://github.com/Kim-J-Smith/embed-function

Copyright (c) 2025 Kim-J-Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Similar implementations

- [std::function](http://en.cppreference.com/w/cpp/utility/functional/function)

- [Naios/function2](https://github.com/Naios/function2)

- [pmed/fixed_size_function](https://github.com/pmed/fixed_size_function)

- [rigtorp/Function](https://github.com/rigtorp/Function)

- [rosbacke/delegate](https://github.com/rosbacke/delegate)

- [winterscar/functional-avr](https://github.com/winterscar/functional-avr)

- [bdiamand/Delegate](https://github.com/bdiamand/Delegate)

- [potswa/cxx_function](https://github.com/potswa/cxx_function)
