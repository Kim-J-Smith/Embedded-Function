#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
  ebd::fn<int(int, int)> f_non_lref = ebd_test_free_func_iii_add;
  ebd::fn<int(int, int) &> f_lref = ebd_test_free_func_iii_add;

  // fn<Sig &> -> fn<Sig> is invalid.
  f_non_lref = f_lref; // FAIL
}
