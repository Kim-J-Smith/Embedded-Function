**🔧 Fixed Bugs**
- Fixed a bug where MSVC mistakenly regarded the empty-state invoker (`empty::invoke`, which throws `std::bad_function_call` or terminates) as a hot path and peeled it into a per-call guard in the calling code. The `empty::invoke` is now marked with the new internal `EMBED_DETAIL_COLD` macro to address this issue.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Updated the assembly analysis documents under `docs/perf/`: the x86_64 MSVC, RISC-V GCC and ARM GCC analyses now compare register argument passing against the stack spills of `std::function` and cover destruction/copy, the `ebd::fn_ref` zero-stack analysis was extended with a full comparison and a summary table, and a new `docs/perf/x86_64_gcc_asm_analysis.md` document was added.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- The `std::constant_wrapper` + `std::in_place_type_t` support is still **experimental**.
