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
 * Version: v1.7
 *
 * Copyright (c) 2021-... reacfen
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

#ifndef UNINTTP_UNI_AUTO_HPP
#define UNINTTP_UNI_AUTO_HPP

#include <type_traits>
#include <iterator>
#include <cstddef>
#include <utility>

#ifdef UNINTTP_USE_STD_ARRAY
    #include <array>
    namespace uninttp {
        template <typename T, std::size_t N = 1, bool IsInitializedAsArray = false, bool = false>
        struct uni_auto : public std::array<T, N> {
            using type = std::conditional_t<IsInitializedAsArray, std::array<T, N>, T>;

            constexpr uni_auto(const T v) : std::array<T, N>{v} {}
            template <std::size_t ...Indices>
            constexpr uni_auto(const T* v, std::index_sequence<Indices...>) : std::array<T, N>{ v[Indices]... } {}
            constexpr uni_auto(const T* v) : uni_auto(v, std::make_index_sequence<N>()) {}

            constexpr operator type() const {
                if constexpr (IsInitializedAsArray)
                    return *this;
                else
                    return (*this)[0];
            }
        };
    }
#else
    namespace uninttp {
        template <typename T, std::size_t N = 1, bool IsInitializedAsArray = false, bool = false>
        struct uni_auto {
            using type = std::conditional_t<IsInitializedAsArray, const T[N], const T>&;
            const T values[N];

            constexpr uni_auto(const T v) : values{v} {}
            template <std::size_t ...Indices>
            constexpr uni_auto(const T* v, std::index_sequence<Indices...>) : values{ v[Indices]... } {}
            constexpr uni_auto(const T* v) : uni_auto(v, std::make_index_sequence<N>()) {}

            constexpr operator type() const {
                if constexpr (IsInitializedAsArray)
                    return values;
                else
                    return values[0];
            }

            constexpr auto begin() const
            requires IsInitializedAsArray {
                return std::cbegin(values);
            }
            constexpr auto end() const
            requires IsInitializedAsArray {
                return std::cend(values);
            }
        };
    }
#endif

namespace uninttp {
    template <typename T>
    struct uni_auto<T, 1, false, true> : public T {
        using type = const T&;

        constexpr uni_auto(const T& v) : T{v} {}
    };

    template <typename T, std::size_t N>
    uni_auto(T(&)[N]) -> uni_auto<std::remove_cv_t<T>, N, true, false>;
    template <typename T>
    requires std::is_class_v<T>
    uni_auto(T) -> uni_auto<std::remove_cv_t<T>, 1, false, true>;

    template <uni_auto Value>
    using uni_auto_t = typename decltype(Value)::type;

#ifdef UNINTTP_USE_STD_ARRAY
    template <uni_auto Value>
    constexpr auto uni_auto_v = static_cast<uni_auto_t<Value>>(Value);
#else
    template <uni_auto Value>
    constexpr auto& uni_auto_v = static_cast<uni_auto_t<Value>>(Value);
#endif

    template <uni_auto Value>
    constexpr auto uni_auto_len = std::size(uni_auto_v<Value>);

    namespace detail {
        template <typename, uni_auto Value>
        struct uni_auto_simplify_v_impl;
        template <typename T, bool IsClassType, uni_auto Value>
        struct uni_auto_simplify_v_impl<uni_auto<T, 1, false, IsClassType>, Value> {
            static constexpr auto value = uni_auto_v<Value>;
        };
        template <typename T, std::size_t N, uni_auto Value>
        struct uni_auto_simplify_v_impl<uni_auto<T, N, true, false>, Value> {
#ifdef UNINTTP_USE_STD_ARRAY
            static constexpr auto value = Value.data();
#else
            static constexpr auto value = &Value[0];
#endif
        };
    }

    template <uni_auto Value>
    constexpr auto uni_auto_simplify_v = detail::uni_auto_simplify_v_impl<std::remove_cv_t<decltype(Value)>, Value>::value;

    template <uni_auto Value>
    using uni_auto_simplify_t = std::decay_t<decltype(uni_auto_simplify_v<Value>)>;
}

#endif /* UNINTTP_UNI_AUTO_HPP */
