#include "test_function.hpp"

struct MoveDelete {
    MoveDelete() = default;
    MoveDelete(const MoveDelete&) = default;
    MoveDelete(MoveDelete&&) = delete;

    int operator()() { return 42; }
};

int main() {
    // move-constructor must exist.
    ebd::fn<int()> f = MoveDelete{}; // FAIL
}
