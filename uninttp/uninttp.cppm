/*
 *               _       _   _         
 *              (_)     | | | |        
 *   _   _ _ __  _ _ __ | |_| |_ _ __  
 *  | | | | '_ \| | '_ \| __| __| '_ \ 
 *  | |_| | | | | | | | | |_| |_| |_) |
 *   \__,_|_| |_|_|_| |_|\__|\__| .__/ 
 *                              | |    
 *                              |_|    
 *
 * uninttp (Universal Non-Type Template Parameters)
 *
 * Version: v2.0
 *
 * Copyright (c) 2021 reacfen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

module;

#include <type_traits>
#include <iterator>
#include <cstddef>
#include <utility>

export module uninttp.uni_auto;

export namespace uninttp {
    /**
     * @brief The `uni_auto` class type implementation
     */
    template <typename, std::size_t, bool, bool, bool, bool>
    struct uni_auto;

    template <typename T, std::size_t N, bool IsClassType, bool IsFunctionType>
    struct uni_auto<T, N, true, IsClassType, IsFunctionType, false> {
        using type = T(&)[N];
        type value;

        constexpr uni_auto(type v) : value{v} {}

        constexpr operator type() const {
            return value;
        }

        static constexpr auto size() {
            return N;
        }

        constexpr auto begin() const {
            return std::begin(value);
        }

        constexpr auto end() const {
            return std::end(value);
        }
    };
    
    template <typename T, std::size_t N, bool IsClassType, bool IsFunctionType>
    struct uni_auto<const T, N, true, IsClassType, IsFunctionType, false> {
        using type = const T(&)[N];
        std::remove_reference_t<type> value;

        template <std::size_t ...Indices>
        constexpr uni_auto(type v, std::index_sequence<Indices...>) : value{v[Indices]...} {}

        constexpr uni_auto(type v) : uni_auto{v, std::make_index_sequence<N>()} {}

        constexpr operator type() const {
            return value;
        }

        static constexpr auto size() {
            return N;
        }

        constexpr auto begin() const {
            return std::begin(value);
        }

        constexpr auto end() const {
            return std::end(value);
        }
    };
    
    template <typename T, bool IsReferenceType>
    struct uni_auto<T, 0, false, true, false, IsReferenceType> : T {
        using type = T;

        constexpr uni_auto(type v) : T{v} {}
    };
    
    template <typename R, typename ...Args>
    struct uni_auto<R (Args...), 0, false, false, true, false> {
        using type = R (*)(Args...);
        type value;

        constexpr uni_auto(type v) : value{v} {}

        constexpr operator type() const {
            return value;
        }

        constexpr auto operator&() const = delete;

        constexpr R operator()(Args&&... args) const {
            return value(std::forward<Args>(args)...);
        }
    };
    
    template <typename T>
    struct uni_auto<T, 0, false, false, false, false> {
        using type = T;
        type value;

        constexpr uni_auto(type v) : value{v} {}

        constexpr operator type() const {
            return value;
        }
    };
    
    template <typename T>
    struct uni_auto<T, 0, false, false, false, true> {
        using type = T&;
        type value;

        constexpr uni_auto(type v) : value{v} {}
    };

    /* Deals with C-style arrays */
    template <typename T, std::size_t N>
    uni_auto(T(&)[N]) -> uni_auto<T, N, true, false, false, false>;

    /* Deals with function pointers */
    template <typename R, typename ...Args>
    uni_auto(R (Args...)) -> uni_auto<R (Args...), 0, false, false, true, false>;

    /* Deals with integral and enumeration types, pointers to objects, pointers to member functions and objects, nullptr */
    template <typename T>
        requires std::is_class_v<T>
    uni_auto(const T&) -> uni_auto<std::remove_cv_t<T>, 0, false, true, false, false>;
    template <typename T>
        requires (!std::is_class_v<T>)
    uni_auto(const T&) -> uni_auto<std::remove_cv_t<T>, 0, false, false, false, false>;

    /* Deals with lvalue references */
    template <typename T>
        requires std::is_class_v<T>
    uni_auto(T&) -> uni_auto<T, 0, false, true, false, true>;
    template <typename T>
        requires (!std::is_class_v<T>)
    uni_auto(T&) -> uni_auto<T, 0, false, false, false, true>;

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(T1&& a, const uni_auto<T2, 0, false, false, false, false>& b) {
        return [&] <typename ...Args>(Args&&... args) constexpr {
            return (std::forward<T1>(a).*static_cast<T2>(b))(std::forward<Args>(args)...);
        };
    }

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(T1* a, const uni_auto<T2, 0, false, false, false, false>& b) {
        return [&] <typename ...Args>(Args&&... args) constexpr {
            return (a->*static_cast<T2>(b))(std::forward<Args>(args)...);
        };
    }

    /**
     * @brief Fetches the type held by whatever `uni_auto` object is passed to it.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    using uni_auto_t = typename decltype(Value)::type;

    /**
     * @brief Fetches the actual value held by whatever `uni_auto` object is passed to it.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    constexpr uni_auto_t<Value> uni_auto_v = Value;

    /**
     * @brief Basically `uni_auto_v<Value>` but with an additional array-to-pointer conversion in case the passed object holds a reference to an array.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    constexpr auto uni_auto_simplify_v =
        static_cast<
            std::conditional_t<std::is_array_v<std::remove_reference_t<uni_auto_t<Value>>>,
                std::remove_extent_t<std::remove_reference_t<uni_auto_t<Value>>>*,
                uni_auto_t<Value>>
        >(Value);

    /**
     * @brief Fetches the type after "simplification" of a `uni_auto` object.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    using uni_auto_simplify_t = std::remove_cv_t<decltype(uni_auto_simplify_v<Value>)>;
}
