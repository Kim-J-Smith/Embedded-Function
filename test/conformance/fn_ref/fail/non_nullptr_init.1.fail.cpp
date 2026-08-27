#include "test_function.hpp"

int main() {
#if defined(__GNUC__) && !defined(__clang__) || defined(EBD_TEST_USE_FALLBACK)
  static_assert(false, "");
#else
  // ebd::fn_ref shouldn't be created from nullptr.
  ebd::fn_ref<int()> f = static_cast<int(*)()>(nullptr); // FAIL
  (void)f;
#endif
}
