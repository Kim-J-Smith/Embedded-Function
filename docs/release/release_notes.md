**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- **The macro `EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER` will be removed in v2.2.0**. If you still rely on it, please inform us in the [issues](https://github.com/Kim-J-Smith/Embedded-Function/issues) section.
- **The function wrapper `ebd::fn_view` and `ebd::safe_fn` will be removed in v2.2.0**. If you still rely on it, please inform us in the [issues](https://github.com/Kim-J-Smith/Embedded-Function/issues) section.
- **The function wrapper `ebd::fn` and `ebd::unique_fn` will not throw exceptions on empty calls in v2.2.0**.
- As of v2.1.11, the usage of `std::constant_wrapper` and `std::meta::is_complete_type` remains **experimental**. These features have been officially adopted in the C++26 standard (`ISO/IEC 14882:2026`) and will become fully stable starting from **v2.2.0**.