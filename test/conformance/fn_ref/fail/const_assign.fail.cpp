#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
  ebd::fn_ref<int(int, int)> f1 = ebd_test_free_func_iii_add;
  ebd::fn_ref<int(int, int) const> f2 = ebd_test_free_func_iii_add;

  // const -> non-const is not allowed.
  f2 = f1; // FAIL
}
