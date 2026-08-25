#include "test_function.hpp"

int main() {
  ebd_test_move_only_callable rref_callable{};
  // '&&'-qualifier is not allowed for fn_ref.
  ebd::fn_ref<int(char) &&> f = rref_callable; // FAIL
}
