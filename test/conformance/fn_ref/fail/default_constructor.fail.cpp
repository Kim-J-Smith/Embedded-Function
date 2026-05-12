#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"

int main() {
  // fn_ref cannot be created by default constructor.
  ebd::fn_ref<int()> f; // FAIL
}
