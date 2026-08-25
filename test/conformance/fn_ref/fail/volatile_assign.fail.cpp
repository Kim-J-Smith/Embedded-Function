#include "test_function.hpp"

int main() {
  ebd::fn_ref<int(int, int)> f_non_volatile = ebd_test_free_func_iii_add;
  ebd::fn_ref<int(int, int) volatile> f_volatile = ebd_test_free_func_iii_add;

  // fn_ref<Sig> -> fn_ref<Sig volatile> is invalid.
  f_volatile = f_non_volatile; // FAIL
}
