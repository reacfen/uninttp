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

However, a template parameter declared with `uni_auto` can do much more than a template parameter declared with `auto` in the sense that you can also pass string literals and `constexpr`-marked arrays through it: [<kbd>Demo</kbd>](https://godbolt.org/z/65czTG76z)

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
    for (auto const& elem : Array)
        std::cout << elem << " ";
    std::cout << std::endl;
    /*
    If you want to write an index-based for-loop, you can do so like this:
    (Keep in mind that 'uni_auto_v' is required here as the compiler cannot do implicit
     conversions in this context)

    for (std::size_t i = 0; i < std::size( uni_auto_v<Array> ); i++)
        std::cout << Array[i] << " ";
    std::cout << std::endl;
    */
}

int main() {
    // Passing a string literal
    static_assert(std::string_view(shift<"foobar", 3>()) ==  "bar"); // OK

    // Passing an array marked as 'constexpr'
    constexpr int arr[] = { 1, 8, 9, 20 };
    print_array<arr>();                                              // 1 8 9 20

    // Passing an 'std::array' object
    print_array<std::array { 1, 4, 6, 9 }>();                        // 1 4 6 9
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

You can also enforce a type by adding a constraint: [<kbd>Demo</kbd>](https://godbolt.org/z/a3KMT8Ges)

```cpp
#include <uninttp/uni_auto.hpp>
#include <type_traits>
#include <concepts>

using namespace uninttp;

template <uni_auto Value>
/* You need to use 'uni_auto_t<Value>' to fetch the underlying type held by the value,
   and since the type returned is in the form of a reference to an array,
   you would need to decay it to a pointer type in order to do a proper comparison */
requires (std::same_as<std::decay_t<uni_auto_t<Value>>, const char*>)
void only_accepts_strings() {}

int main() {
    only_accepts_strings<"foobar">(); // OK
    only_accepts_strings<123>();      // Error! Constraint not satisfied!
}
```

> **Note**: One can also "*exploit*" the above combination of constraints and `uni_auto` to achieve a sort of "*function overloading through template parameters*" mechanism: [<kbd>Demo</kbd>](https://godbolt.org/z/j6rGh4hr8)
> 
> ```cpp
> #include <uninttp/uni_auto.hpp>
> #include <type_traits>
> #include <concepts>
> #include <iostream>
> 
> using namespace uninttp;
> 
> template <uni_auto Value>
> requires (std::same_as<std::decay_t<uni_auto_t<Value>>, const char*>)
> void do_something() {
>     std::cout << "A string was passed" << std::endl;
> }
> 
> template <uni_auto Value>
> requires (std::same_as<std::decay_t<uni_auto_t<Value>>, int>)
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

All the examples shown have used function templates to demonstrate the capability of `uni_auto`. However, it can readily be used in any context.

## Limitations:

There are two drawbacks to using `uni_auto`:

1) The datatype of the value held by a `uni_auto` object cannot be fetched using `decltype(X)` as is done with `auto`-template parameters. Instead, one would have to use `uni_auto_t<X>` instead:
    ```cpp
    template <uni_auto X = 1.89>
    void fun() {
        static_assert(std::same_as<uni_auto_t<X>, double>); // OK
    }
    // ...
    ```
2) There may be some cases where conversion operator of the `uni_auto` object doesn't get invoked. In such a scenario, one would need to explicitly notify the compiler to extract the value out of the `uni_auto` object using `uni_auto_v`:
    ```cpp
    template <uni_auto X = 42>
    void fun() {
        constexpr auto answer = uni_auto_v<x>;
        /* You can also write the above line as follows:
           constexpr int answer = X; */
        static_assert(std::same_as<decltype(answer), int>); // OK
    }
    // ...
    ```

## Playground:

If you'd like to play around with `uni_auto` yourself, [here](https://godbolt.org/z/9jdv48ehz) you go!
