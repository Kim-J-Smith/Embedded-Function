/// make_fn_deduction.cpp — auto-deduced signature, member function wrapping, operator* fallback
#include <iostream>
#include <embed/embed_function.hpp>

static void greet(const char* name) {
    std::cout << "Hello, " << name << "!\n";
}

struct Counter {
    int count;
    explicit Counter(int c = 0) : count(c) {}

    int get() const { return count; }
    void set(int c) { count = c; }
    void inc() { ++count; }
};

struct Doubler {
    int operator()(int x) const { return x * 2; }
};

/*
=== Typical output:
Hello, make_fn from free function!
Hello, raw function pointer!
lambda sum: 30
Doubler(21) = 42
getter(c) = 0
after inc: 1
after set: 99
filled later
unique_fn!
===
*/

int main() {
    // Deduce from free function pointer
    auto fn1 = ebd::make_fn(&greet);
    fn1("make_fn from free function");

    // operator* recovers the raw function pointer
    void(*raw_ptr)(const char*) = *fn1;
    raw_ptr("raw function pointer");

    // Deduce from lambda (signature + buffer size auto-inferred)
    auto fn2 = ebd::make_fn([](int a, int b) { return a + b; });
    std::cout << "lambda sum: " << fn2(10, 20) << '\n';

    // Deduce from functor
    auto fn3 = ebd::make_fn(Doubler{});
    std::cout << "Doubler(21) = " << fn3(21) << '\n';

    // Wrap member functions
    Counter c(0);

    auto getter = ebd::make_fn(&Counter::get);
    std::cout << "getter(c) = " << getter(c) << '\n';

    auto increment = ebd::make_fn(&Counter::inc);
    increment(c);
    std::cout << "after inc: " << getter(c) << '\n';

    auto setter = ebd::make_fn(&Counter::set);
    setter(c, 99);
    std::cout << "after set: " << getter(c) << '\n';

    // Explicit signature (e.g., for overload disambiguation)
    auto empty_fn = ebd::make_fn<void()>();
    empty_fn = [] { std::cout << "filled later\n"; };
    empty_fn();

    // Explicit wrapper type
    auto ufn = ebd::make_fn<ebd::unique_fn>([]() { std::cout << "unique_fn!\n"; });
    ufn();

    return 0;
}
