#include "test_function.hpp"

int main() {
  ebd_test_move_only_callable move_only{};
  // fn requires the functor copyable.
  ebd::fn<int(char) &&> f = move_only; // FAIL
}
