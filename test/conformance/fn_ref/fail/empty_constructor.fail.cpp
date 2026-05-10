#include "embed/embed_function.hpp"

int main() {
  // fn_ref cannot be created by empty constructor.
  ebd::fn_ref<int()> f(nullptr); // FAIL
}
