**🔧 Fixed Bugs**
- Fixed a bug where wrappers could not be instantiated when a parameter is a reference to an incomplete type.
- Fixed a bug where static operator() with default argument was mis-deduced as deducing-this.
- Fixed undefined behavior caused by using an inactive union member in `clone` and during initialization.
- Fixed a bug where `is_convertible_from_specialization` did not remove cv qualifiers, so cv-qualified wrappers were not recognized as convertible.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Relaxed `is_convertible_from_specialization`: signatures that differ only in value/`&&` for parameters of non-register-passable types are now convertible, e.g. `ebd::fn<int(std::string)>` -> `ebd::fn<int(std::string&&)>`. The conversion is direct, avoiding the extra indirection and enlarged buffer of re-wrapping.
- Removed the internal `crtp_mixins::core_facade_impl` layer. `ebd::detail::function` now declares its owning/view constructors directly in the two partial specializations. No user-visible API change.
- An invalid `Config` passed to `ebd::detail::function` now produces a clear `static_assert` diagnostic ("Template arguments are invalid.") instead of an incomplete-type error.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
