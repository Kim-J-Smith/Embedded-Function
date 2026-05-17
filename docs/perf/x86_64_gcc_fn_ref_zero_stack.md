# x86_64 GCC `fn_ref` Zero-stack overhead

### Source code

```cpp
#include "embed/embed_function.hpp"

bool func(ebd::fn_ref<bool(int, int)> f) {
    return f(1, 2);
}

bool func(std::function<bool(int, int)> f) {
    return f(1, 2);
}
```

### Assembly analysis

```bash
g++-16 -std=c++11 -Os
```

```asm
"func(ebd::detail::function<8ul, ebd::detail::fn_traits::config_package<true, true, false, false>, bool (int, int)>)":
        mov     rax, rsi
        mov     edx, 2
        mov     esi, 1
        jmp     rax ; tail call

; --------------------------------------------------------------------------------

"func(std::function<bool (int, int)>)":
        sub     rsp, 24
        mov     DWORD PTR [rsp+8], 1
        mov     DWORD PTR [rsp+12], 2
        cmp     QWORD PTR [rdi+16], 0
        jne     .L3
        call    "std::__throw_bad_function_call()"
.L3:
        lea     rdx, [rsp+12]
        lea     rsi, [rsp+8]
        call    [QWORD PTR [rdi+24]]
        add     rsp, 24
        ret
```
