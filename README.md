# uninttp

A universal type for non-type template parameters for C++20 or later.

## Installation:

uninttp (***Uni***versal ***N***on-***T***ype ***T***emplate ***P***arameters) is a header-only library, meaning you only need to include the required header(s) to start using it in your project/environment. In this case, simply cloning this repository and doing `#include <uninttp/uni_auto.hpp>` should suffice.

## Usage:

Using uninttp's `uninttp::uni_auto` is pretty straightforward and is synonymous to `auto` in *most* of the cases: [<kbd>Demo</kbd>](https://godbolt.org/z/fhWhaf5hx)

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

However, a template parameter declared with `uni_auto` can do much more than a template parameter declared with `auto` in the sense that you can also pass string literals and `constexpr`-marked arrays through it: [<kbd>Demo</kbd>](https://godbolt.org/z/fvGssW49x)

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

    // Index-based for loop
    for (std::size_t i = 0; i < std::size(Array); i++)
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

You can also use it with parameter packs, obviously: [<kbd>Demo</kbd>](https://godbolt.org/z/Kqhr3dYE7)

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

You can also enforce a type by adding a constraint: [<kbd>Demo</kbd>](https://godbolt.org/z/ETfG3Y5G5)

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

> **Note**: One can also "*exploit*" the above combination of constraints and `uni_auto` to achieve a sort of "*function overloading through template parameters*" mechanism: [<kbd>Demo</kbd>](https://godbolt.org/z/sW9Thfaex)
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

Unsurprisingly, one can pass trivial `struct`s through `uni_auto` as well: [<kbd>Demo</kbd>](https://godbolt.org/z/PzhcoM8eW)

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

You can also pass lambdas and compile-time functor objects through `uni_auto` as well: [<kbd>Demo</kbd>](https://godbolt.org/z/nbPxTjKz5)

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

And it doesn't end there! `uni_auto` can also work with pointers of almost any type:

Example using a pointer to an object: [<kbd>Demo</kbd>](https://godbolt.org/z/jxM6PoWqj)
```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

template <uni_auto p>
void print_pointer() {
    std::cout << p << std::endl; // Prints the location of 'x' in memory
}

int main() {
    static constexpr int x = 2;
    print_pointer<&x>();
}
```

Example using function pointers: [<kbd>Demo</kbd>](https://godbolt.org/z/fEPa56a1f)
```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

constexpr auto some_fun() {
    return 42;
}

template <uni_auto Func>
constexpr auto call_fun() {
    return Func();
}

int main() {
    static_assert(call_fun<some_fun>() == 42); // OK
}
```

Example using member function pointers: [<kbd>Demo</kbd>](https://godbolt.org/z/xTo7487cE)
```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

struct some_class {
    void some_member(int& p) const {
        p = 2;
    }
};

template <uni_auto Value>
void call_member(some_class const& x, int& y) {
    (x->*Value)(y); // Alternative: '(x.*uni_auto_v<Value>)(y);'
}

int main() {
    some_class x;
    int y;

    call_member<&some_class::some_member>(x, y);

    std::cout << y << std::endl;
}
```

All the examples shown have used function templates to demonstrate the capability of `uni_auto`. However, it can readily be used in any context.
## Cheat sheet:

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Description |
| --- | --- |
| `uninttp::uni_auto_t<uni_auto Value>` | Gives the type of the underlying value held by the `uni_auto` object passed to it. |
| `uninttp::uni_auto_simplify_t<uni_auto Value>` | Gives the decayed type of the value held by the `uni_auto` object.<br/>If the `uni_auto` object holds an array, it decays it into a pointer and returns the pointer as the type.<br/>This feature is often useful for doing compile-time type-checking, SFINAE and for defining certain constraints on the types held by the `uni_auto` object. |
| `uninttp::uni_auto_v<uni_auto Value>` | Effectively extracts the underlying value held by the `uni_auto` object passed to it. |
| `uninttp::uni_auto_simplify_v<uni_auto Value>` | Converts the underlying value of the `uni_auto` object into its simplest form. If the value held is an array, it converts it into a pointer and returns that, otherwise it does the exact same thing as `uni_auto_v`. |

## Tell uninttp that you'd like to use `std::array` instead of normal C-style arrays:

It is completely up to the choice of the user to choose between working with C-style arrays or `std::array`, the former is used by uninttp by default. In order to use the latter, one needs to define `UNINTTP_USE_STD_ARRAY` before including any of uninttp's header(s): [<kbd>Demo</kbd>](https://godbolt.org/z/xP7d7W7KP)

```cpp
#define UNINTTP_USE_STD_ARRAY
#include <uninttp/uni_auto.hpp>

// Now 'uninttp::uni_auto' and co. will use 'std::array' instead of C-Style arrays to store their value(s)...

#include <iostream>
#include <cstddef>
#include <array>

using namespace uninttp;

template <typename T, std::size_t N>
void fun1(std::array<T, N> const& a) {
    std::cout << a.data() << std::endl;
}

template <uni_auto U>
void fun2() {
    fun1(U);
}

int main() {
    fun2<"Hello from std::array!">();
}
```

## How to fetch the type and value from a `uni_auto` object explicitly (Limitations of `uni_auto`):

1) The datatype of the value held by a `uni_auto` object cannot be fetched using `decltype(X)` as is done with `auto`-template parameters. Instead, one would have to do something like this instead: [<kbd>Demo</kbd>](https://godbolt.org/z/Kvx1vYzqf)
    ```cpp
    #include <uninttp/uni_auto.hpp>
    #include <concepts>

    using namespace uninttp;

    template <uni_auto X = 1.89>
    void fun() {
        // This doesn't work for obvious reasons
        // static_assert(std::same_as<decltype(X), double>);                        // Error

        // Using 'uni_auto_simplify_t':
        static_assert(std::same_as<uni_auto_simplify_t<X>, double>);                // OK

        // Using 'uni_auto_t' and decaying the type returned:
        static_assert(std::same_as<std::decay_t<uni_auto_t<X>>, double>);           // OK

        // Using 'uni_auto_v' and then using 'decltype()' and decaying the type returned:
        static_assert(std::same_as<std::decay_t<decltype(uni_auto_v<X>)>, double>); // OK
    }

    int main() {
        fun<>();
    }
    ```
2) There may be some cases where conversion operator of the `uni_auto` object doesn't get invoked. In such a scenario, one would need to explicitly notify the compiler to extract the value out of the `uni_auto` object: [<kbd>Demo</kbd>](https://godbolt.org/z/a4jExs9nz)
    ```cpp
    #include <uninttp/uni_auto.hpp>
    #include <concepts>

    using namespace uninttp;

    template <uni_auto X = 42>
    void fun() {
        // Using an explicit conversion statement
        constexpr int answer1 = X;

        // Using 'uni_auto_v'
        constexpr auto answer2 = uni_auto_v<X>;

        // Using 'uni_auto_simplify_v':
        constexpr auto answer3 = uni_auto_simplify_v<X>;

        static_assert(std::same_as<std::decay_t<decltype(answer1)>, int>); // OK
        static_assert(std::same_as<std::decay_t<decltype(answer2)>, int>); // OK
        static_assert(std::same_as<std::decay_t<decltype(answer3)>, int>); // OK
    }

    int main() {
        fun<>();
    }
    ```

## Playground:

If you'd like to play around with `uni_auto` yourself, [here](https://godbolt.org/z/r7qhvGP4a) you go!
