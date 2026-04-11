## New

- Added automated release workflow for packaging `header_only.zip` and `header_only.tar.gz`.

- Made `ebd::fn_view` trivially relocatable across all compilers.

- Added "Future learning & evolution reference" section to README.md.

## Fix

- Fixed `ebd::fn_view` to support construction from non-movable callable objects.
