#include "test_function.hpp"

int main() {
  ebd::fn<int(int, int)> f_non_rref = ebd_test_free_func_iii_add;
  ebd::fn<int(int, int) &&> f_rref = ebd_test_free_func_iii_add;

  // fn<Sig &&> -> fn<Sig> is invalid.
  f_non_rref = f_rref; // FAIL
}
