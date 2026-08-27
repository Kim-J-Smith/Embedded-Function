**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added the `EMBED_FN_CONFIG_EMPTY_TRIVIAL_STATEFUL` macro. When defined, empty trivial functors are no longer treated as stateless (except standard operator wrappers). (#156)

**🛠️ Optimizations and Improvements**
- Improved test coverage to 100% (gcov) and removed unused header includes from tests. (#153)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
