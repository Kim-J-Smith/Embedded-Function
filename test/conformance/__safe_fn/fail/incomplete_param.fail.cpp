#include "test_function.hpp"

struct IncompleteStruct;

int main() {
  // Parameter types cannot be incomplete.
  ebd::__safe_fn<int(IncompleteStruct)> f; // FAIL
}
