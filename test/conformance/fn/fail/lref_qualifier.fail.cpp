#include "test_function.hpp"

int main() {
  ebd_test_operator_unambiguous lref_callable{};
  // '&'-qualifier is necessary for Lvalue-Callable functor.
  ebd::fn<int(int)> f = lref_callable; // FAIL
}
