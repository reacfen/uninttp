# uninttp

A universal non-type template parameter implementation for C++20 and later.

## Installation:

uninttp is header-only, meaning you only need to include the required headers to start using it in your project/environment.

## Usage:

Using uninttp is pretty straightforward, it usually acts synonymous with how the `auto` template parameter works:

```cpp
#include <uninttp/uni_auto.hpp>

using namespace uninttp;

template <uni_auto Value>
constexpr auto add20() {
    return Value + 20;
}

int main() {
    static_assert(add20<20>() == 40);
}
```

And if you thought "Can't I just use something like `template <auto Value>` instead?", then you'd be completely correct. One can safely replace `uni_auto` with `auto`, at least for *this* example.

However,  a template parameter declared with`uni_auto` can do much more than `auto` in the sense that you can also pass string literals  and `constexpr` arrays as non-type template parameters through it:

```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>
#include <cstddef>
#include <cstring>

using namespace uninttp;

template <uni_auto Value, std::size_t X>
constexpr auto shift() {
    return Value + X;
}

template <uni_auto Array>
void print_array() {
    for (auto const& elem : Array)
        std::cout << elem << " ";
}

int main() {
    // Passing a string literal
    static_assert(std::strcmp(shift<"foobar", 3>(), "bar") == 0);
    // Passing an array marked as 'constexpr'
	constexpr int arr[] = { 1, 8, 9, 20 };
	print_array<arr>(); // Prints 1 8 9 20
}
```

You can also use it with parameter packs, obviously:

```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

template <uni_auto ...Values>
void print() {
    ((std::cout << Values << " "), ...);
}

int main() {
    print<1, 2, "foo">(); // Prints 1 2 foo
}
```

You can also enforce a type by adding a constraint:

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

> **Note**: One can also "*exploit*" the above combination of constraints and `uni_auto` to achieve a sort of "*function overloading through template parameters*" mechanism:
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
> template <uni_auto Value>
> requires (std::same_as<std::decay_t<uni_auto_t<Value>>, int>)
> void do_something() {
>     std::cout << "An integer was passed" << std::endl;
> }
> 
> int main() {
>     do_something<"foobar">(); // Prints: "A string was passed"
>     do_something<123>();      // Prints: "An integer was passed"
>     // do_something<12.3>();  // Error!
> }
> ```

Unsurprisingly, one can pass trivial structs through `uni_auto` as well:

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
    /* 'uni_auto_v' needs to be used here to get the underlying value held by 'A' and ' B 'as
        the compiler won't be able to help us here */
    return uni_auto_v<A>.val * uni_auto_v<B>.val;
}

int main() {
    static_assert(mul<X{}, Y{}>() == 42);
}
```

All the examples shown have used function templates to demonstrate the capability of `uni_auto`. However, it can readily be used in any context.

## Limitations:

There are two drawbacks to using `uni_auto`:

1) The datatype of the value `uni_auto` cannot be fetched using `decltype(x)`. Instead, one would have to use `uni_auto_t` instead:
    ```cpp
    constexpr uni_auto x = 1.89;
    static_assert(std::same_as<uni_auto_t<x>, double>); // OK
    ```
2) There may be some cases where conversion operator of the `uni_auto` object doesn't get invoked. In such a scenario, one would need to explicitly notify the compiler to extract the value out of the `uni_auto` object using `uni_auto_v`:
    ```cpp
    constexpr uni_auto x = 42;
    constexpr auto answer = uni_auto_v<x>;
    /* You can also write the above line as follows:
       constexpr int answer = x; */
    static_assert(std::same_as<decltype(answer), int>); // OK
    ```

## Playground:

If you'd like to play around with `uni_auto` yourself, [here](https://godbolt.org/z/P99fnq17r) you go!