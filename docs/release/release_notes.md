**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Relaxed `is_convertible_from_specialization`: signatures that differ only in value/`&&` for parameters of non-register-passable types are now convertible, e.g. `ebd::fn<int(std::string)>` -> `ebd::fn<int(std::string&&)>`. The conversion is direct, avoiding the extra indirection and enlarged buffer of re-wrapping.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
