**🔧 Fixed Bugs**
- Fixed `__cpp_lib_constant_wrapper` feature test macro threshold from `202606L` to `202603L` to match the latest C++26 standard. (#104)
- Fixed MSVC `/analyze` warning C6031 in erasure `destroy`. (#109)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added fast-path swap for trivially copyable functors, bypassing the 3-move proxy pattern. (#109)

**🛠️ Optimizations and Improvements**
- Improved compile-time performance. (#108)
  - Reordered requires-clause constraints for early short-circuit evaluation.
  - Converted constraint traits to `std::true_type`-based specializations with `IsView` flag.
  - Removed redundant per-functor static assertions (`qualifier_of_signature_match_functor`, `is_class_call_operator`).
- Removed unused traits (`is_unbounded_array`, `get_stored_type`, `invoke_result_package`). (#112)
- Removed redundant `inline` specifiers from invoke functions and comparison operators. (#112)
- Guarded `<initializer_list>` include behind C++17 version check. (#107)
- Optimized internal error report format. (#112)

**📌 Notes**
- **The macro `EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER` will be removed in v2.2.0**. If you still rely on it, please inform us in the [issues](https://github.com/Kim-J-Smith/Embedded-Function/issues) section.
- As of v2.1.10, the usage of `std::constant_wrapper` and `std::meta::is_complete_type` remains **experimental**. These features have been officially adopted in the C++26 standard (`ISO/IEC 14882:2026`) and will become fully stable starting from **v2.2.0**.