**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added the `EMBED_FN_CONFIG_EMPTY_TRIVIAL_STATEFUL` macro. When defined, empty trivial functors are no longer treated as stateless (except standard operator wrappers). (#156)

**🛠️ Optimizations and Improvements**
- Achieved full (100%) line coverage of the library's measurable runtime code (gcov); the only lines gcov reports as unexecuted are the empty-call terminate paths executed inside death-test subprocesses, which are not measurable because the child process aborts before flushing profile data. Removed unused header includes from tests. (#153)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
