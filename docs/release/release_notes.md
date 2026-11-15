**🔧 Fixed Bugs**
- Fixed a bug where user-provided stateful specializations of standard function objects were incorrectly treated as stateless, causing the stored object to be ignored and a new instance to be constructed on every call. (#277)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added `aligned_memcpy` to preserve pointer alignment and avoid byte-by-byte copies on strict-alignment targets such as RISC-V, using it only where an aligned byte copy is actually needed. (#273, #274)
- CI now cancels superseded runs on the same pull request to save runner resources, while runs triggered by pushes or manual dispatches are not canceled. (#275)
- CD now publishes `-rc` / `-alpha` / `-beta` versions as GitHub pre-releases, so a pre-release no longer takes over the latest stable release. (#276)
- Removed the internal `Signature` template parameter from the management layer, so manager vtables and `clone` / `move` / `destroy` code are shared by wrappers with different signatures, reducing code size. (#280)

**📌 Notes**
- None.
