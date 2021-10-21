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
 * Version: v1.9
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

#ifndef UNINTTP_UNI_AUTO_HPP
#define UNINTTP_UNI_AUTO_HPP

#include <type_traits>
#include <iterator>
#include <cstddef>
#include <utility>

#ifdef UNINTTP_USE_STD_ARRAY
    #include <array>
    namespace uninttp {
        template <typename T, std::size_t N = 1, bool IsInitializedAsArray = false, bool = false, bool = false>
        struct uni_auto : public std::array<T, N> {
            using type = std::conditional_t<IsInitializedAsArray, std::array<T, N>, T>;

            constexpr uni_auto(T v) : std::array<T, N>{ v } {}
            template <std::size_t ...Indices>
            constexpr uni_auto(const T(&v)[N], std::index_sequence<Indices...>) : std::array<T, N>{ v[Indices]... } {}
            constexpr uni_auto(const T(&v)[N]) : uni_auto(v, std::make_index_sequence<N>()) {}

            constexpr operator type() const {
                if constexpr (IsInitializedAsArray)
                    return *this;
                else
                    return (*this)[0];
            }
            constexpr operator const T*() const
            requires IsInitializedAsArray {
                return this->data();
            }
        };
    }
#else
    namespace uninttp {
        template <typename T, std::size_t N = 1, bool IsInitializedAsArray = false, bool = false, bool = false>
        struct uni_auto {
            using type = std::conditional_t<IsInitializedAsArray, const T[N], const T>&;
            const T values[N];

            constexpr uni_auto(T v) : values{ v } {}
            template <std::size_t ...Indices>
            constexpr uni_auto(const T(&v)[N], std::index_sequence<Indices...>) : values{ v[Indices]... } {}
            constexpr uni_auto(const T(&v)[N]) : uni_auto(v, std::make_index_sequence<N>()) {}

            constexpr operator type() const {
                if constexpr (IsInitializedAsArray)
                    return values;
                else
                    return values[0];
            }

            constexpr auto size() const
            requires IsInitializedAsArray {
                return N;
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
    struct uni_auto<T, 1, false, true, false> : public T {
        using type = const T&;

        constexpr uni_auto(T v) : T{ std::move(v) } {}
    };
    template <typename R, typename ...Args>
    struct uni_auto<R(Args...), 1, false, false, true> {
        using type = R(*)(Args...);
        type value;

        constexpr uni_auto(R v(Args...)) : value(v) {}

        constexpr operator type() const {
            return value;
        }

        constexpr auto operator&() const = delete;

        constexpr R operator()(Args&&... args) const {
            return value(std::forward<Args>(args)...);
        }
    };

    template <typename T, std::size_t N>
    uni_auto(const T(&)[N]) -> uni_auto<std::remove_cv_t<T>, N, true, false, false>;
    template <typename T, std::size_t N>
    uni_auto(const T(&&)[N]) -> uni_auto<std::remove_cv_t<T>, N, true, false, false>;
    template <typename R, typename ...Args>
    uni_auto(R(Args...)) -> uni_auto<R(Args...), 1, false, false, true>;
    template <typename T>
    requires std::is_class_v<T>
    uni_auto(T) -> uni_auto<std::remove_cv_t<T>, 1, false, true, false>;

    template <typename T1, typename T2>
    constexpr auto operator->*(T1&& a, const uni_auto<T2, 1, false, false, false>& b) {
        return [&](auto&&... args) constexpr {
            return (std::forward<T1>(a).*static_cast<T2>(b))(std::forward<decltype(args)>(args)...);
        };
    }

    template <uni_auto Value>
    using uni_auto_t = typename decltype(Value)::type;

    template <uni_auto Value>
    constexpr uni_auto_t<Value> uni_auto_v = Value;

    namespace detail {
        template <typename, uni_auto Value>
        struct uni_auto_simplify_v_impl;
        template <typename T, bool IsClassType, bool IsFunctionType, uni_auto Value>
        struct uni_auto_simplify_v_impl<uni_auto<T, 1, false, IsClassType, IsFunctionType>, Value> {
            static constexpr auto value = uni_auto_v<Value>;
        };
        template <typename T, std::size_t N, uni_auto Value>
        struct uni_auto_simplify_v_impl<uni_auto<T, N, true, false, false>, Value> {
            static constexpr auto value = static_cast<const T*>(Value);
        };
    }

    template <uni_auto Value>
    constexpr auto uni_auto_simplify_v = detail::uni_auto_simplify_v_impl<std::remove_cv_t<decltype(Value)>, Value>::value;

    template <uni_auto Value>
    using uni_auto_simplify_t = std::remove_cv_t<decltype(uni_auto_simplify_v<Value>)>;
}

#endif /* UNINTTP_UNI_AUTO_HPP */
