## 🔧 Fixed Bugs
- None.

## ⚠️ Breaking Changes
- None.

## ✨ New Features
- Added a static assertion for the completeness of parameter types in the function wrappers. This helps to determine the cause of the error. (#80 #83)

## 🛠️ Optimizations and Improvements
- Replaced the internal `switch-case` branch dispatch with a `VTable` mechanism for better lifetime management and reduced function‑call overhead, resulting in improved performance. (#74 #84)
- Renamed all `HOW-TO-*` documentation files to `README.md` to provide better navigation on GitHub. (#86)

## 📌 Notes
- **The macro `EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER` will be removed in v2.0.0**. If you still rely on it, please inform us in the [issues](https://github.com/Kim-J-Smith/Embedded-Function/issues) section.
- As of v2.1.8, the usage of `std::constant_wrapper` and `std::meta::is_complete_type` remains **experimental**. These features have been officially adopted in the C++26 standard (`ISO/IEC 14882:2026`) and will become fully stable starting from **v2.2.0**.