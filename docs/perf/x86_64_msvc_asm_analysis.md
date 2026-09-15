# x86_64 MSVC Assembly Analysis

## Free function & scalar parameters

### Source code

```cpp
#include "embed/embed_function.hpp"

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

auto main() -> int {
	auto ebd_add = ebd::make_fn<ebd::classic_fn>(add);
	auto ebd_sub = ebd::make_fn<ebd::classic_fn>(sub);

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

```bat
cl /std:c++latest /O2 /EHsc /Zc:__cplusplus /Zc:preprocessor   :: MSVC 19.51, x64
```

`make_fn<ebd::classic_fn>(add)` deduces an `ebd::classic_fn` whose underlying
type is `detail::function<8, 8, ...>`: an 8-byte buffer plus the manager and the
invoker, 24 bytes in total, against 64 bytes for the `std::function` used here.
Both dispatch through two levels: a per-type invoker stored in the object, and
then the callable itself.

### `ebd::classic_fn`: arguments stay in registers

```asm
; res = ebd_sub(0x2233, 0x1122);
	mov	edx, 8755			; 00002233H	; arg#1 -> register
	mov	r8d, 4386			; 00001122H	; arg#2 -> register
	lea	rcx, QWORD PTR ebd_sub$[rbp-153]	; the erased object
	call	QWORD PTR ebd_sub$[rbp-137]		; m_invoker

; InvokerImpl<8,8,...>::inplace::invoke<int (__cdecl*)(int,int)>(ErasurePass, int, int)
	mov	r9d, edx
	mov	rax, rcx
	mov	edx, r8d
	mov	ecx, r9d
	rex_jmp	QWORD PTR [rax]				; tail call to `add`/`sub`
```

No scalar argument touches memory, and no empty-state test is emitted either:
`EMBED_DETAIL_COLD` (`__declspec(noinline)` here) keeps MSVC from peeling the
empty sentinel into a per-call guard. With an opaque wrapper (a `const&`
parameter, the usual callback shape) the same compiler emits a bare
`rex_jmp QWORD PTR [rcx+24]`.

### `std::function`: arguments are spilled to memory

```asm
; res = std_add(0x2233, 0x1122);
	mov	DWORD PTR <_Args_1>$[rbp-153], 4386	; arg#2 -> stack
	mov	DWORD PTR <_Args_0>$[rbp-153], 8755	; arg#1 -> stack
	mov	rcx, QWORD PTR std_add$[rbp-97]		; _Impl
	test	rcx, rcx				; _Empty()
	je	$LN169@main				; -> _Xbad_function_call
	mov	rax, QWORD PTR [rcx]			; vftable
	lea	r8, QWORD PTR <_Args_1>$[rbp-153]	; &arg#2
	lea	rdx, QWORD PTR <_Args_0>$[rbp-153]	; &arg#1
	call	QWORD PTR [rax+16]			; _Do_call

; _Func_impl_no_alloc<int (__cdecl*)(int,int),int,int,int>::_Do_call(int&&, int&&)
	mov	rax, rdx
	mov	r9, rcx
	mov	edx, DWORD PTR [r8]			; arg#2 loaded back
	mov	ecx, DWORD PTR [rax]			; arg#1 loaded back
	rex_jmp	QWORD PTR [r9+8]			; tail call to the stored callable
```

MSVC's `std::function` does not dispatch through a stored invoker pointer: the
wrapper holds a pointer to a `_Func_impl_no_alloc` object and dispatches through
its vtable, so the callable is reached through a vtable load plus a tail jump,
and the scalar arguments are round-tripped through the stack. MSVC's own source mapping names
the pieces: `_Empty()` on line 1053 and `_Impl->_Do_call(...)` on line 1057 of
`<functional>`.

### Summary

| | `ebd::classic_fn` | `std::function` |
|---|---|---|
| dispatch levels per invocation | 2 | 2 |
| scalar arguments | registers | stored to stack, address passed, loaded back |
| empty state | none; `m_invoker` points to `empty::invoke` (`EMBED_DETAIL_COLD`) | `_Empty()` test -> `_Xbad_function_call` |
| call site | 4 instructions | 9 instructions |
| `sizeof` / `alignof` | 24 / 8 | 64 / 8 |

### Destruction and copy

```asm
; ebd: inlined, one dispatch through the manager vtable
	mov	rax, QWORD PTR ebd_sub$[rbp-145]	; m_manager
	mov	rdx, QWORD PTR [rax+16]			; vtable[2]
	test	rdx, rdx
	je	SHORT $LN153@main
	lea	rcx, QWORD PTR ebd_sub$[rbp-153]
	call	rdx

; std::function: a virtual call, always
	mov	rcx, QWORD PTR std_sub$[rbp-97]
	test	rcx, rcx
	je	SHORT $LN121@main
	mov	rax, QWORD PTR [rcx]
	lea	rdx, QWORD PTR std_sub$[rbp-153]
	cmp	rcx, rdx
	setne	dl					; !_Local()
	call	QWORD PTR [rax+32]			; _Delete_this(bool)
	mov	QWORD PTR std_sub$[rbp-97], 0
```

The manager vtable installed for a function pointer is (names elided):

```asm
	vtable	DQ	trivially_clone<int (__cdecl*)(int,int)>	; 8-byte copy
		DQ	trivially_move<int (__cdecl*)(int,int)>
		DQ	0						; NULL: nothing to destroy
```

and the copy itself:

```asm
	mov	rax, QWORD PTR [rdx]
	mov	QWORD PTR [rcx], rax
	ret	0
```

The NULL third entry is why the `ebd` destructor above skips the call for a
trivially destructible callable, while `std::function` always reaches its
`_Delete_this` virtual (MSVC annotates it as line 1138 of `<functional>`).
