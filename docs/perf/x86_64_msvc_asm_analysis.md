# x86_64 MSVC Assembly Analysis

## Free function & scalar parameters

### Source code

```cpp
#include "embed/embed_function.hpp"

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

auto main() -> int {
	auto ebd_add = ebd::make_fn(add);
	auto ebd_sub = ebd::make_fn(sub);

	auto std_add = std::function<int(int, int)>{ add };
	auto std_sub = std::function<int(int, int)>{ sub };

	volatile int res = 0;

	res = ebd_add(0x2233, 0x1122);
	res = ebd_sub(0x2233, 0x1122);
	res = std_add(0x2233, 0x1122);
	res = std_sub(0x2233, 0x1122);
}
```

### Assembly analysis

> MSVC C++23 Release

```asm
	res = ebd_add(0x2233, 0x1122);
00007FF7138C109E  mov         edx,2233h     ; using register to pass scalar arguments.
00007FF7138C10A3  mov         r8d,1122h     ; using register to pass scalar arguments.
00007FF7138C10A9  lea         rcx,[rbp-51h]
; call <int ebd::(...)::InvokerImpl<...>(ErasureBase const*, int, int)>
00007FF7138C10AD  call        r9 
00007FF7138C10B0  mov         dword ptr [rbp-79h],eax 

<int ebd::(...)::InvokerImpl<...>(ErasureBase const*, int, int)>:
00007FF7138C1250  mov         eax,edx  
00007FF7138C1252  mov         r9,rcx  
00007FF7138C1255  mov         edx,r8d  
00007FF7138C1258  mov         ecx,eax  
00007FF7138C125A  jmp         qword ptr [r9]  ; call free function `add`

; --------------------------------------------------------------------------------

	res = std_add(0x2233, 0x1122);
00007FF7138C10C8  mov         dword ptr [rbp-79h],1122h  ; using stack to pass scalar arguments.
00007FF7138C10CF  mov         dword ptr [rbp-71h],2233h  ; using stack to pass scalar arguments.
00007FF7138C10D6  mov         rcx,qword ptr [rbp+3Fh]  
00007FF7138C10DA  test        rcx,rcx  
00007FF7138C10DD  jne         main+0C6h (07FF7138C10E6h) ; branch to check empty.
00007FF7138C10DF  call        qword ptr [__imp_std::_Xbad_function_call (07FF7138C2040h)]  
00007FF7138C10E5  int         3  
00007FF7138C10E6  mov         rax,qword ptr [rcx]  
00007FF7138C10E9  lea         r8,[rbp-79h]  
00007FF7138C10ED  lea         rdx,[rbp-71h]  
; call <std::_Function_handler<...>::_M_invoke(std::_Any_data const&, int&&, int&&)>
00007FF7138C10F1  call        qword ptr [rax+10h]  
00007FF7138C10F4  mov         dword ptr [rbp-79h],eax  

<std::_Function_handler<...>::_M_invoke(std::_Any_data const&, int&&, int&&)>:
00007FF7138C1220  mov         rax,rdx  
00007FF7138C1223  mov         r9,rcx  
00007FF7138C1226  mov         edx,dword ptr [r8]  
00007FF7138C1229  mov         ecx,dword ptr [rax]  
00007FF7138C122B  jmp         qword ptr [r9+8]  ; call free function `add` by vtable
```

