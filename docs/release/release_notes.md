**🔧 Fixed Bugs**
- Fixed incorrect stateless detection in `is_stateless`: functors with copy side effects (non-trivially-copyable) were treated as stateless, causing copies to be elided. (#145)

**⚠️ Breaking Changes**
- Deprecated `operator bool`. Use `!f.is_empty()` instead. (#143)

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
