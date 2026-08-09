#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"

struct IncompleteStruct;

int main() {
  // Parameter types cannot be incomplete.
  ebd::fn<int(IncompleteStruct)> f; // FAIL
}
