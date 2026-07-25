**🔧 Fixed Bugs**
- Fixed a bug where `ebd::fn`, `ebd::unique_fn` and `ebd::safe_fn` could not wrap callable objects that were aligned to `alignof(std::max_align_t)` on some platforms. (#96)

**⚠️ Breaking Changes**
- The alignment of `ebd::fn`, `ebd::unique_fn` and `ebd::safe_fn` has been changed to `alignof(std::max_align_t)`. (#96)

**✨ New Features**
- Added some code examples in `example/`. (#95)

**🛠️ Optimizations and Improvements**
- Optimized debug information to provide better diagnostics. (#97)

**📌 Notes**
- **The macro `EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER` will be removed in v2.2.0**. If you still rely on it, please inform us in the [issues](https://github.com/Kim-J-Smith/Embedded-Function/issues) section.
- As of v2.1.9, the usage of `std::constant_wrapper` and `std::meta::is_complete_type` remains **experimental**. These features have been officially adopted in the C++26 standard (`ISO/IEC 14882:2026`) and will become fully stable starting from **v2.2.0**.