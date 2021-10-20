# uninttp

A universal type for non-type template parameters for C++20 or later.

## Installation:

uninttp (**Uni**versal **N**on-**T**ype **T**emplate **P**arameters) is a header-only library, meaning you only need to include the required header(s) to start using it in your project/environment. In this case, simply cloning this repository and doing `#include <uninttp/uni_auto.hpp>` should suffice.

## Usage:

Using uninttp's `uninttp::uni_auto` is pretty straightforward and is synonymous to `auto` in *most* of the cases: [<kbd>Demo</kbd>](https://godbolt.org/z/sbxve5G6M)

```cpp
#include <uninttp/uni_auto.hpp>

using namespace uninttp;

template <uni_auto Value>
constexpr auto add20() {
    return Value + 20;
}

int main() {
    static_assert(add20<20>() == 40); // OK
}
```

And if you thought "Can't I just use something like `template <auto Value>` instead?", then you'd be absolutely correct. One can safely replace `uni_auto` with `auto`, at least for *this* example.

However, a template parameter declared with `uni_auto` can do much more than a template parameter declared with `auto` in the sense that you can also pass string literals and `constexpr`-marked arrays through it: [<kbd>Demo</kbd>](https://godbolt.org/z/4eMfK8KEW)

```cpp
#include <uninttp/uni_auto.hpp>
#include <string_view>
#include <iostream>
#include <cstddef>
#include <array>

using namespace uninttp;

template <uni_auto Value, std::size_t X>
constexpr auto shift() {
    return Value + X;
}

template <uni_auto Array>
void print_array() {
    // Using range-based for loop
    for (auto const& elem : Array)
        std::cout << elem << " ";
    
    std::cout << std::endl;

    // Using iterators
    for (auto it = std::begin(Array); it != std::end(Array); ++it)
        std::cout << *it << " ";
    
    std::cout << std::endl;

    // Index-based for loop (1st version)
    /* (The usage of 'uni_auto_v' is required here as the compiler cannot do implicit
        conversions in this context) */
    for (std::size_t i = 0; i < std::size(uni_auto_v<Array>); i++)
        std::cout << Array[i] << " ";
    
    std::cout << std::endl;

    // Index-based for loop (2nd version)
    for (std::size_t i = 0; i < uni_auto_len<Array>; i++)
        std::cout << Array[i] << " ";
    
    std::cout << std::endl;
}

int main() {
    // Passing a string literal
    static_assert(std::string_view(shift<"foobar", 3>()) == "bar"); // OK

    // Passing an array marked as 'constexpr'
    constexpr int arr[] = { 1, 8, 9, 20 };
    print_array<arr>();                                             // 1 8 9 20

    // Passing an 'std::array' object
    print_array<std::array { 1, 4, 6, 9 }>();                       // 1 4 6 9
}
```

You can also use it with parameter packs, obviously: [<kbd>Demo</kbd>](https://godbolt.org/z/3Eax886zT)

```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

template <uni_auto ...Values>
void print() {
    ((std::cout << Values << " "), ...) << std::endl;
}

int main() {
    print<1, 3.14159, 6.3f, "foo">(); // 1 3.14159 6.3 foo
}
```

You can also enforce a type by adding a constraint: [<kbd>Demo</kbd>](https://godbolt.org/z/oK3sfsTKs)

```cpp
#include <uninttp/uni_auto.hpp>
#include <concepts>

using namespace uninttp;

template <uni_auto Value>
// 'uni_auto_simplify_t<Value>' gives you the simplified type for type-checking convenience
requires std::same_as<uni_auto_simplify_t<Value>, const char*>
void only_accepts_strings() {}

int main() {
    only_accepts_strings<"foobar">(); // OK
    // only_accepts_strings<123>();   // Error! Constraint not satisfied!
}
```

> **Note**: One can also "*exploit*" the above combination of constraints and `uni_auto` to achieve a sort of "*function overloading through template parameters*" mechanism: [<kbd>Demo</kbd>](https://godbolt.org/z/dvxvxaK86)
> 
> ```cpp
> #include <uninttp/uni_auto.hpp>
> #include <concepts>
> #include <iostream>
> 
> using namespace uninttp;
> 
> template <uni_auto Value>
> requires std::same_as<uni_auto_simplify_t<Value>, const char*>
> void do_something() {
>     std::cout << "A string was passed" << std::endl;
> }
> template <uni_auto Value>
> requires std::same_as<uni_auto_simplify_t<Value>, int>
> void do_something() {
>     std::cout << "An integer was passed" << std::endl;
> }
> 
> int main() {
>     do_something<"foobar">(); // A string was passed
>     do_something<123>();      // An integer was passed
>     // do_something<12.3>();  // Error!
> }
> ```

Unsurprisingly, one can pass trivial `struct`s through `uni_auto` as well: [<kbd>Demo</kbd>](https://godbolt.org/z/reT6eEjj6)

```cpp
#include <uninttp/uni_auto.hpp>

using namespace uninttp;

struct X {
    int val{6};
};

struct Y {
    int val{7};
};

template <uni_auto A, uni_auto B>
constexpr auto mul() {
    return A.val * B.val;
}

int main() {
    static_assert(mul<X{}, Y{}>() == 42); // OK
}
```

You can also pass lambdas and compile-time functor objects through `uni_auto` as well: [<kbd>Demo</kbd>](https://godbolt.org/z/9zjPnz6zc)

```cpp
#include <uninttp/uni_auto.hpp>

using namespace uninttp;

template <uni_auto F>
constexpr auto call() {
    return F();
}

struct Funct {
    constexpr auto operator()() const {
        return 86;
    }
};

int main() {
    static_assert(call<[] { return 69; }>() == 69); // OK
    static_assert(call<Funct{}>() == 86);           // OK
}
```

All the examples shown have used function templates to demonstrate the capability of `uni_auto`. However, it can readily be used in any context.

## Cheat sheet:

| <img width=3000/> | Description |
| --- | --- |
| `uninttp::uni_auto_t<uni_auto Value>` | Gives the type of the underlying value held by the `uni_auto` class object passed to it. |
| `uninttp::uni_auto_simplify_t<uni_auto Value>` | Gives the decayed type of the value held by the `uni_auto` class object. If the `uni_auto` object holds an array, it condenses it into a pointer and returns the pointer as the type. This is mostly useful for doing portable compile-time type-checking and other things related to SFINAE.<br/><br/>*Equivalent to*: `std::remove_cv_t<decltype(uni_auto_simplify_v<Value>)>` |
| `uninttp::uni_auto_v<uni_auto Value>` | Effectively extracts the underlying value held by the `uni_auto` class object passed to it. |
| `uninttp::uni_auto_simplify_v<uni_auto Value>` | Converts the underlying value of the `uni_auto` object into its simplest form. If the value held is an array, it converts it into a pointer and returns that, otherwise it does the exact same thing as `uni_auto_v`. |
| `uninttp::uni_auto_len<uni_auto Value>` | As the name suggests, it returns the length/size of the array held by `uni_auto` (if any).<br/><br/>*Equivalent to*: `std::size(uni_auto_v<Value>)`. |

## Limitations:

There are two drawbacks to using `uni_auto`:

1) The datatype of the value held by a `uni_auto` object cannot be fetched using `decltype(X)` as is done with `auto`-template parameters. Instead, one would have to use `uni_auto_t<X>` and/or `uni_auto_simplify_t<X>` instead:
    ```cpp
    template <uni_auto X = 1.89>
    void fun() {
        // This doesn't work
        static_assert(std::same_as<decltype(X), double>);                 // Error

        static_assert(std::same_as<std::decay_t<uni_auto_t<X>>, double>); // OK
        // This would also work
        static_assert(std::same_as<uni_auto_simplify_t<X>, double>);      // OK
    }
    // ...
    ```
2) There may be some cases where conversion operator of the `uni_auto` object doesn't get invoked. In such a scenario, one would need to explicitly notify the compiler to extract the value out of the `uni_auto` object using `uni_auto_v`:
    ```cpp
    template <uni_auto X = 42>
    void fun() {
        constexpr auto answer = uni_auto_v<X>;
        /* You can also write the above line as follows:
           constexpr int answer = X; */
        static_assert(std::same_as<std::decay_t<decltype(answer)>, int>); // OK
    }
    // ...
    ```

## Tell uninttp that you'd like to use `std::array` instead of normal C-style arrays:

It is completely up to the choice of the user to choose between working with C-style arrays or `std::array`, the former is used by uninttp by default. In order to use the latter, one needs to define `UNINTTP_USE_STD_ARRAY` before including any of uninttp's header(s):

```cpp
#define UNINTTP_USE_STD_ARRAY
#include <uninttp/uni_auto.hpp>

// Now 'uninttp::uni_auto' and co. will use 'std::array' instead of C-Style arrays to store their value(s)...
```

## Playground:

If you'd like to play around with `uni_auto` yourself, [here](https://godbolt.org/z/r7qhvGP4a) you go!
