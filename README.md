
# uninttp

A universal type for non-type template parameters for C++20 or later.

## Installation:

uninttp (***Uni***versal ***N***on-***T***ype ***T***emplate ***P***arameters) is a header-only library. You can either clone this repository or fetch one of the recent [releases](https://github.com/reacfen/uninttp/releases) at the time.

Once that is done, you can simply include the header(s) and start using uninttp in your project:

```cpp
#include <uninttp/uni_auto.hpp>
```

uninttp also has a C++ module version, so if your compiler supports [C++20 modules](https://en.cppreference.com/w/cpp/language/modules), you can do something like this instead of including the whole header file into your project:

```cpp
import uninttp.uni_auto; // Improves compilation speed
```

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

## Test suite:

An exhaustive test on uninttp's `uninttp::uni_auto` has been done to ensure that it consistently works for almost every non-type template argument allowed.
> **Note**: *Some of the features portrayed in the test suite might not work on all compilers as C++20 support, as of writing this, is still in an experimental stage on most compilers*.

The test suite can be found [here](https://godbolt.org/z/Yanbbd8es).

(*P.S.*: For reference, one can look up [this](https://en.cppreference.com/w/cpp/language/template_parameters) link.)

## Cheat sheet:

<table>
    <thead>
        <tr>
            <th style="width: 380px"></th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>uninttp::uni_auto_t&lt;uni_auto Value&gt;</code></td>
            <td>Gives the type of the underlying value held by the <code>uni_auto</code> object passed to it.</td>
        </tr>
        <tr>
            <td><code>uninttp::uni_auto_simplify_t&lt;uni_auto Value&gt;</code></td>
            <td>Gives the decayed type of the value held by the <code>uni_auto</code> object.<br>If the <code>uni_auto</code> object holds an array, it decays it into a pointer and returns the pointer as the type.<br>This feature is often useful for doing compile-time type-checking, SFINAE and for defining certain constraints on the types held by the <code>uni_auto</code> object.</td>
        </tr>
        <tr>
            <td><code>uninttp::uni_auto_v&lt;uni_auto Value&gt;</code></td>
            <td>Effectively extracts the underlying value held by the <code>uni_auto</code> object passed to it.</td>
        </tr>
        <tr>
            <td><code>uninttp::uni_auto_simplify_v&lt;uni_auto Value&gt;</code></td>
            <td>Converts the underlying value of the <code>uni_auto</code> object into its simplest form. If the value held is an array, it converts it into a pointer and returns that, otherwise it does the exact same thing as <code>uni_auto_v</code>.</td>
        </tr>
    </tbody>
</table>

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
