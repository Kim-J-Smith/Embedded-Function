# RISC-V GCC Assembly Analysis

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

```bash
riscv32-wch-elf-g++ -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 \
                    -msave-restore -std=c++11 -Os    # WCH RISC-V Embedded GCC 15.2.0
```

`make_fn<ebd::classic_fn>(add)` deduces an `ebd::classic_fn` whose underlying
type is `detail::function<16u, 16u, ...>`, as the symbol names below show. Only 4
bytes are needed for a function pointer, but the owning alignment defaults to
`alignof(std::max_align_t)`, which is 16 on this target (`long double` is 16
bytes), so the buffer is padded to 16 and the whole wrapper is 32 bytes against
16 bytes for the `std::function` used here.

Both wrappers dispatch through two levels: a per-type invoker stored inside the
object, and then the callable itself.

### `ebd::classic_fn`: arguments stay in registers

```asm
; the second call, which goes through memory
   1012c:	57d6                	lw	a5,116(sp)          ; m_invoker
   1012e:	6605                	lui	a2,0x1
   10130:	6589                	lui	a1,0x2
   10132:	c62a                	sw	a0,12(sp)          ; previous statement
   10134:	12260613          	addi	a2,a2,290	# 1122 ; arg#2
   10138:	23358593          	addi	a1,a1,563	# 2233 ; arg#1
   1013c:	1088                	addi	a0,sp,96            ; the erased object
   1013e:	9782                	jalr	a5

; second level: InvokerImpl<16u, 16u, ...>::inplace::invoke<int (*)(int, int)>(ErasurePass, int, int)
   101d0:	411c                	lw	a5,0(a0)
   101d2:	852e                	mv	a0,a1               ; arg#1 stays in a register
   101d4:	85b2                	mv	a1,a2               ; arg#2 stays in a register
   101d6:	8782                	jr	a5                   ; tail call to `add`/`sub`
```

7 instructions, 18 bytes: each immediate needs its own `lui`/`addi` pair, so the
call site is one instruction longer than on ARM even though it is six bytes
shorter. The first `ebd_add()` call is emitted as a direct `jal` to the very same
invoker.

### `std::function`: arguments are spilled to memory

```asm
; the first call (GCC scheduled the `res =` store of the previous statement
; into this sequence; it is not part of the call site)
   10140:	6789                	lui	a5,0x2
   10142:	23378793          	addi	a5,a5,563	# 2233 ; arg#1
   10146:	c83e                	sw	a5,16(sp)          ; arg#1 -> stack
   10148:	6785                	lui	a5,0x1
   1014a:	12278793          	addi	a5,a5,290	# 1122 ; arg#2
   1014e:	ca3e                	sw	a5,20(sp)          ; arg#2 -> stack
   10150:	57a2                	lw	a5,40(sp)          ; _M_manager
   10152:	c62a                	sw	a0,12(sp)          ; previous statement
   10154:	ef91                	bnez	a5,10170 <main+0x9c>   ; _M_empty()
   10156:	3f9d                	jal	100cc <std::__throw_bad_function_call()>
   10170:	57b2                	lw	a5,44(sp)          ; _M_invoker
   10172:	0850                	addi	a2,sp,20            ; &arg#2
   10174:	080c                	addi	a1,sp,16            ; &arg#1
   10176:	1008                	addi	a0,sp,32            ; &_M_functor
   10178:	9782                	jalr	a5

; second level: _Function_handler<...>::_M_invoke(_Any_data const&, int&&, int&&)
   101fa:	872e                	mv	a4,a1
   101fc:	411c                	lw	a5,0(a0)
   101fe:	420c                	lw	a1,0(a2)            ; arg#2 loaded back
   10200:	4308                	lw	a0,0(a4)            ; arg#1 loaded back
   10202:	8782                	jr	a5
```

13 instructions, 30 bytes on the hot path, plus a 2-byte compressed
`__throw_bad_function_call` (`c.jal`).

`std::function<R(Args...)>::operator()` takes `Args...` by value and forwards
them as `Args&&`, so every scalar argument is materialized in memory and passed
by address, only to be loaded again by the callee. `ebd` selects
`smart_forward_t<Args>`, which degenerates to by-value for register-passable
types, so its invoker stays `Ret (*)(ErasurePass, int, int)`.

### Summary

| | `ebd::classic_fn` | `std::function` |
|---|---|---|
| dispatch levels per invocation | 2 | 2 |
| scalar arguments | registers | stored to stack, address passed, loaded back |
| empty state | `m_invoker` points to `empty::invoke` -> throws `std::bad_function_call` | `_M_empty()` test and branch on every call |
| call site | 7 instructions, 18 bytes | 13 instructions, 30 bytes + 2 cold |
| `sizeof` / `alignof` | 32 / 16 | 16 / 4 |

GCC emits no empty-state test here: nothing compares `m_invoker` against a
sentinel, and with this source no `empty::invoke` symbol is emitted at all.

### Destruction and copy

```asm
; ebd: the payload is trivially destructible, so the NULL vtable slot is taken
   10210:	491c                	lw	a5,16(a0)
   10212:	479c                	lw	a5,8(a5)
   10214:	c391                	beqz	a5,10218 <ebd::detail::crtp_mixins::destructor_impl<ebd::detail::fn_traits::config_package<true, false, true, false>, ebd::detail::function<16u, 16u, ebd::detail::fn_traits::config_package<true, false, true, false>, int (int, int) const> >::~destructor_impl()+0x8>
   10216:	8782                	jr	a5
   10218:	8082                	ret

; std::function: _M_manager is always invoked with __destroy_functor
   10204:	451c                	lw	a5,8(a0)
   10206:	c781                	beqz	a5,1020e <std::_Function_base::~_Function_base()+0xa>
   10208:	85aa                	mv	a1,a0
   1020a:	460d                	li	a2,3
   1020c:	8782                	jr	a5
   1020e:	8082                	ret
```

The manager vtable installed for a function pointer is:

```asm
        .word	trivially_clone<int (*)(int, int)>   ; tail call to `memcpy`, size 4
        .word	trivially_move<int (*)(int, int)>
        .word	0                                    ; NULL: nothing to destroy
```
