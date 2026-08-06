/// basic_usage.cpp — four wrapper types: fn, unique_fn, classic_fn, fn_ref
#include <iostream>
#include <memory>
#include <embed/embed_function.hpp>

static int add(int a, int b) { return a + b; }

struct Printer {
    void operator()(const char* msg) const noexcept {
        std::cout << "Printer: " << msg << '\n';
    }
};

struct MoveOnly {
    std::unique_ptr<int> val;
    explicit MoveOnly(int v) : val(new int(v)) {}
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
    int operator()(int x) const { return *val + x; }
};

/*
=== Typical output:
fn  add: 7
fn  mul: 12
fn2 mul: 30
ufn  : 101
ufn2 : 102
Printer: Hello classic_fn!
ref  : 70
empty_fn is empty
===
*/

int main() {
    // ebd::fn — copyable, owning
    ebd::fn<int(int, int)> fn1;
    fn1 = &add;
    std::cout << "fn  add: " << fn1(3, 4) << '\n';

    fn1 = [](int a, int b) { return a * b; };
    std::cout << "fn  mul: " << fn1(3, 4) << '\n';

    ebd::fn<int(int, int)> fn2 = fn1;  // copy
    std::cout << "fn2 mul: " << fn2(5, 6) << '\n';

    // ebd::unique_fn — move-only, owning
    ebd::unique_fn<int(int)> ufn;
    ufn = MoveOnly(100);
    std::cout << "ufn  : " << ufn(1) << '\n';

    ebd::unique_fn<int(int)> ufn2 = std::move(ufn);  // move
    std::cout << "ufn2 : " << ufn2(2) << '\n';

    // ebd::classic_fn — copyable, throws on empty (like std::function)
    ebd::classic_fn<void(const char*)> cfn;
    cfn = Printer{};
    cfn("Hello classic_fn!");

    // ebd::fn_ref — non-owning view (zero overhead)
    int factor = 10;
    auto lambda = [&factor](int x) { return x * factor; };
    ebd::fn_ref<int(int)> ref = lambda;
    std::cout << "ref  : " << ref(7) << '\n';

    // Empty state check
    ebd::fn<void()> empty_fn;
    std::cout << "empty_fn is "
              << (empty_fn ? "not empty" : "empty") << '\n';

    return 0;
}
