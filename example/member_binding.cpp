/// Member function wrapping, object binding, wrapper conversions
#include <iostream>
#include <string>
#include <embed/embed_function.hpp>

struct Dog {
    std::string name;
    explicit Dog(const char* n) : name(n) {}

    void speak() const {
        std::cout << name << " barks: Woof!\n";
    }
    void rename(const char* n) {
        name = n;
    }
};

struct Cat {
    std::string name;
    explicit Cat(const char* n) : name(n) {}

    void speak() const {
        std::cout << name << " meows: Meow~\n";
    }
};

// fn_ref as parameter — accepts fn, unique_fn, fn_ref, etc.
void process(ebd::fn_ref<void() const> action) {
    action();
}

/*
=== Typical output:
Buddy barks: Woof!
Whiskers meows: Meow~
Buddy barks: Woof!
Whiskers meows: Meow~
Buddy barks: Woof!
Buddy barks: Woof!
Rex barks: Woof!
===
*/

int main() {
    Dog dog("Buddy");
    Cat cat("Whiskers");

    // Way 1: wrap member function pointer directly (object passed as 1st arg)
    ebd::fn<void(const Dog&) const> dog_memfn = ebd::make_fn(&Dog::speak);
    ebd::fn<void(const Cat&) const> cat_memfn = ebd::make_fn(&Cat::speak);

    dog_memfn(dog);
    cat_memfn(cat);

    // Way 2: bind object via lambda -> zero-arg callable
    ebd::fn<void() const> dog_lambda = [&dog] { dog.speak(); };
    ebd::fn<void() const> cat_lambda = [&cat] { cat.speak(); };

    process(dog_lambda);
    process(cat_lambda);

    // Wrapper conversions: unique_fn -> fn_ref
    ebd::unique_fn<void() const> utemp = [&dog] { dog.speak(); };
    ebd::fn_ref<void() const> ref_from_unique = utemp;
    process(ref_from_unique);

    // fn -> fn (copy)
    ebd::fn<void() const> fn_copy = dog_lambda;
    process(fn_copy);

    // Non-const member function with extra argument
    ebd::fn<void(Dog&, const char*)> rename = ebd::make_fn(&Dog::rename);
    rename(dog, "Rex");
    process(dog_lambda);  // name changed

    return 0;
}
