
# uninttp

A universal type for non-type template parameters for C++20 or later.

## Installation:

uninttp (***Uni***versal ***N***on-***T***ype ***T***emplate ***P***arameters) is a header-only library. Just simply clone this repository and you are ready to go.

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

However, a template parameter declared with `uni_auto` can do much more than a template parameter declared with `auto` in the sense that you can also pass string literals, `constexpr`-marked arrays, arrays of static storage duration, etc. through it: [<kbd>Demo</kbd>](https://godbolt.org/z/sb9KbfPeY)

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
    for (auto elem : Array)
        std::cout << elem << " ";
    
    std::cout << '\n';

    // Using iterators
    for (auto it = std::begin(Array); it != std::end(Array); ++it)
        std::cout << *it << " ";
    
    std::cout << '\n';

    // Using index-based for loop
    for (std::size_t i = 0; i < std::size(Array); i++)
        std::cout << Array[i] << " ";
    
    std::cout << '\n';
}

int main() {
    // Passing a string literal
    static_assert(std::string_view(shift<"foobar", 3>()) == "bar"); // OK

    // Passing an array marked as `constexpr`
    constexpr int arr1[] = { 1, 8, 9, 20 };
    print_array<arr1>();                                            // 1 8 9 20

    // Passing an array of static storage duration
    static int arr2[] = {1, 2, 4, 8};
    print_array<arr2>();                                            // 1 2 4 8

    // Passing an `std::array` object
    print_array<std::array { 1, 4, 6, 9 }>();                       // 1 4 6 9
}
```

You can also use it with parameter packs, obviously: [<kbd>Demo</kbd>](https://godbolt.org/z/v1McfhGdf)

```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

template <uni_auto ...Values>
void print() {
    ((std::cout << Values << " "), ...) << '\n';
}

int main() {
    print<1, 3.14159, 6.3f, "foo">(); // 1 3.14159 6.3 foo
}
```

You can also enforce a type by adding a constraint: [<kbd>Demo</kbd>](https://godbolt.org/z/daxs6YGsK)

```cpp
#include <uninttp/uni_auto.hpp>
#include <concepts>

using namespace uninttp;

template <uni_auto Value>
// `uni_auto_simplify_t<Value>` gives you the simplified type for type-checking convenience
requires std::same_as<uni_auto_simplify_t<Value>, const char*>
void only_accepts_strings() {}

int main() {
    only_accepts_strings<"foobar">(); // OK
    // only_accepts_strings<123>();   // Error! Constraint not satisfied!
}
```

> **Note**: One can also use the above combination of constraints and `uni_auto` to achieve a sort of "*function overloading through template parameters*" mechanism: [<kbd>Demo</kbd>](https://godbolt.org/z/oYWGoxb81)
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
>     std::cout << "A string was passed" << '\n';
> }
> template <uni_auto Value>
> requires std::same_as<uni_auto_simplify_t<Value>, int>
> void do_something() {
>     std::cout << "An integer was passed" << '\n';
> }
> 
> int main() {
>     do_something<"foobar">(); // A string was passed
>     do_something<123>();      // An integer was passed
>     // do_something<12.3>();  // Error!
> }
> ```

Unsurprisingly, one can pass trivial `struct`s through `uni_auto` as well: [<kbd>Demo</kbd>](https://godbolt.org/z/n5rqYdd95)

```cpp
#include <uninttp/uni_auto.hpp>

using namespace uninttp;

struct X {
    int val = 6;
};

struct Y {
    int val = 7;
};

template <uni_auto A, uni_auto B>
constexpr auto mul() {
    return A.val * B.val;
}

int main() {
    static_assert(mul<X{}, Y{}>() == 42); // OK
}
```

You can pass lambdas and functors through `uni_auto` as well: [<kbd>Demo</kbd>](https://godbolt.org/z/nbPxTjKz5)

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

And it doesn't end there! `uni_auto` can also work with pointers to objects:

Example using a pointer to an object: [<kbd>Demo</kbd>](https://godbolt.org/z/hPsnT4fP7)
```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

template <uni_auto X>
void mutate_pointer() {
    *X = 42;
}

template <uni_auto X>
void print_pointer() {
    std::cout << *X << '\n';
}

int y = 3;

int main() {
    static constexpr int x = 2;
    print_pointer<&x>();  // 2
    mutate_pointer<&y>(); // Modifies the value of `y` indirectly through pointer access
    print_pointer<&y>();  // 42
}
```

Example using function pointers: [<kbd>Demo</kbd>](https://godbolt.org/z/3hTojnere)
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

Example using a pointer to a member function: [<kbd>Demo</kbd>](https://godbolt.org/z/n11T94v7E)
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
void call_member(const some_class& x, int& y) {
    (x.*uni_auto_v<Value>)(y);
}

int main() {
    some_class x;
    int y;

    call_member<&some_class::some_member>(x, y);

    std::cout << y << '\n';
}
```

Example using lvalue references: [<kbd>Demo</kbd>](https://godbolt.org/z/qx4na1fba)
```cpp
#include <uninttp/uni_auto.hpp>
#include <iostream>

using namespace uninttp;

template <uni_auto Value>
void fun() {
    Value = 49;
}

int main() {
    static int x = 42;
    std::cout << x << '\n'; // 42
    fun<x>();
    std::cout << x << '\n'; // 49
}
```

All the examples shown have used function templates to demonstrate the capability of `uni_auto`. However, it can readily be used in any context.

## Test suite:

An exhaustive test on uninttp's `uninttp::uni_auto` has been done to ensure that it consistently works for almost every non-type template argument allowed.

The test suite can be found [here](https://godbolt.org/z/146W8ssj8).

(*P.S.*: For reference, one can look up [this](https://en.cppreference.com/w/cpp/language/template_parameters) link.)

## Cheat sheet:

<table>
    <thead>
        <tr>
            <th width="45%"></th>
            <th width="55%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>uninttp::uni_auto_t&lt;uni_auto Value&gt;</code></td>
            <td>Gives the type of the underlying value held by <code>Value</code>.</td>
        </tr>
        <tr>
            <td><code>uninttp::uni_auto_simplify_t&lt;uni_auto Value&gt;</code></td>
            <td>Gives the simplified type of the value held by <code>Value</code>.<br>If <code>Value</code> holds an array, it condenses it into a pointer and returns the pointer as the type. It also removes any lvalue or rvalue references (if possible) from the type returned.<br>This feature is often useful for doing compile-time type-checking, SFINAE and for defining certain constraints on the types held by <code>Value</code>.</td>
        </tr>
        <tr>
            <td><code>uninttp::uni_auto_v&lt;uni_auto Value&gt;</code></td>
            <td>Extracts the underlying value held by <code>Value</code>.</td>
        </tr>
        <tr>
            <td><code>uninttp::uni_auto_simplify_v&lt;uni_auto Value&gt;</code></td>
            <td>Converts the underlying value of <code>Value</code> into its simplest form. If the value held is an array, it converts it into a pointer and also casts away any lvalue and rvalue references (if possible).</td>
        </tr>
    </tbody>
</table>

## Restrictions:

1) The datatype of the value held by a `uni_auto` object cannot be fetched using `decltype(X)` as is done with `auto`-template parameters. Instead, one would have to use `uni_auto_t` or `uni_auto_simplify_t`: [<kbd>Demo</kbd>](https://godbolt.org/z/n355orPcs)
    ```cpp
    #include <uninttp/uni_auto.hpp>
    #include <type_traits>

    using namespace uninttp;

    template <uni_auto X = 1.89>
    void fun() {
        // This doesn't work for obvious reasons:
        // static_assert(std::same_as<decltype(X), double>);                                          // Error

        // Using `uni_auto_t`:
        static_assert(std::is_same_v<uni_auto_t<X>, double>);                                         // OK

        // Using `uni_auto_v` and then using `decltype()` and removing the const specifier from the type returned:
        static_assert(std::is_same_v<std::remove_const_t<decltype(uni_auto_v<X>)>, double>);          // OK

        // Using `uni_auto_simplify_t`:
        static_assert(std::is_same_v<uni_auto_simplify_t<X>, double>);                                // OK

        // Using `uni_auto_simplify_v` and then using `decltype()` and removing the const specifier from the type returned:
        static_assert(std::is_same_v<std::remove_const_t<decltype(uni_auto_simplify_v<X>)>, double>); // OK
    }

    int main() {
        fun<1.89>();
    }
    ```
2) There may be some cases where conversion operator of the `uni_auto` object doesn't get invoked. In such a scenario, one would need to explicitly notify the compiler to extract the value out of the `uni_auto` object: [<kbd>Demo</kbd>](https://godbolt.org/z/xMqzWWhET)
    ```cpp
    #include <uninttp/uni_auto.hpp>
    #include <type_traits>

    using namespace uninttp;

    template <uni_auto X>
    void fun() {
        // The conversion operator doesn't get invoked in this case:
        // constexpr auto answer0 = X;
        
        // Using an explicit conversion statement:
        constexpr int answer1 = X;

        // Using `uni_auto_v`:
        constexpr auto answer2 = uni_auto_v<X>;

        // Using `uni_auto_simplify_v`:
        constexpr auto answer3 = uni_auto_simplify_v<X>;

        static_assert(std::is_same_v<decltype(answer1), const int>); // OK
        static_assert(std::is_same_v<decltype(answer2), const int>); // OK
        static_assert(std::is_same_v<decltype(answer3), const int>); // OK
    }

    int main() {
        fun<42>();
    }
    ```
3)  This is more or less an extension to the (2) restriction.

    Using lvalue references of class types with `uninttp::uni_auto` is a little tricky as the dot operator does not function as expected. Instead, one would have to use `uni_auto_v` to extract the value manually: [<kbd>Demo</kbd>](https://godbolt.org/z/YTj6WY5MM)
    ```cpp
    #include <uninttp/uni_auto.hpp>
    #include <iostream>
    #include <cassert>

    using namespace uninttp;

    struct some_class {
        int p = 0;
        int& operator=(const int& rhs) {
            return p = rhs;
        }
    };

    template <uni_auto X>
    void fun() {
        // Assignment operator works as expected
        X = 2;

        // auto a = X.p;                // This will NOT work since the C++ Standard does not allow
                                        // overloading the dot operator (yet)

        // If you want to access the member `p` directly, you would have to call `uni_auto_v` explicitly:
        const auto c = uni_auto_v<X>.p; // OK

        std::cout << c << '\n';         // 2
    }

    int main() {
        static some_class some_obj;
        fun<some_obj>();
    }
    ```

## Playground:

If you'd like to play around with `uni_auto` yourself, [here](https://godbolt.org/z/99YevEPd1) you go!
