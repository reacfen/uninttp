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
 * Version: v3.0
 *
 * Copyright (c) 2021-23 reacfen
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
#include <array>

namespace uninttp {
    /**
     * @brief The `uni_auto` class type implementation
     */
    template <typename T>
    struct uni_auto;

    template <typename T, std::size_t N>
    struct uni_auto<T[N]> {
        using type = T(&)[N];
        type value;

        constexpr uni_auto(type v) noexcept(std::is_nothrow_constructible_v<type, type&>) : value{v} {}

        constexpr operator type() const noexcept {
            return value;
        }

        static constexpr auto size() noexcept {
            return N;
        }
        
        constexpr auto swap(const uni_auto& other) const noexcept(noexcept(std::swap(value, other.value))) {
            std::swap(value, other.value);
        }

        constexpr auto data() const noexcept {
            return std::data(value);
        }

        constexpr auto empty() const noexcept {
            return std::empty(value);
        }

        constexpr auto begin() const noexcept {
            return std::begin(value);
        }

        constexpr auto end() const noexcept {
            return std::end(value);
        }

        constexpr auto cbegin() const noexcept {
            return std::cbegin(value);
        }

        constexpr auto cend() const noexcept {
            return std::cend(value);
        }

        constexpr auto rbegin() const noexcept {
            return std::rbegin(value);
        }

        constexpr auto rend() const noexcept {
            return std::rend(value);
        }

        constexpr auto crbegin() const noexcept {
            return std::crbegin(value);
        }

        constexpr auto crend() const noexcept {
            return std::crend(value);
        }

        constexpr decltype(auto) operator&() const noexcept {
            return &value;
        }
    };

    template <typename T, std::size_t N>
    struct uni_auto<const T[N]> {
        using type = const T(&)[N];
        std::remove_reference_t<type> value;

        template <std::size_t ...Indices>
        constexpr uni_auto(type v, std::index_sequence<Indices...>) noexcept(std::is_nothrow_constructible_v<T, T&>) : value{v[Indices]...} {}

        constexpr uni_auto(type v) noexcept(noexcept(uni_auto{v, std::make_index_sequence<N>()})) : uni_auto{v, std::make_index_sequence<N>()} {}

        constexpr operator type() const noexcept {
            return value;
        }

        static constexpr auto size() noexcept {
            return N;
        }
        
        constexpr auto data() const noexcept {
            return std::data(value);
        }

        constexpr auto empty() const noexcept {
            return std::empty(value);
        }

        constexpr auto begin() const noexcept {
            return std::begin(value);
        }

        constexpr auto end() const noexcept {
            return std::end(value);
        }

        constexpr auto cbegin() const noexcept {
            return std::cbegin(value);
        }

        constexpr auto cend() const noexcept {
            return std::cend(value);
        }

        constexpr auto rbegin() const noexcept {
            return std::rbegin(value);
        }

        constexpr auto rend() const noexcept {
            return std::rend(value);
        }

        constexpr auto crbegin() const noexcept {
            return std::crbegin(value);
        }

        constexpr auto crend() const noexcept {
            return std::crend(value);
        }

        constexpr decltype(auto) operator&() const noexcept {
            return &value;
        }
    };

    template <typename T>
        requires (!std::is_class_v<T>)
    struct uni_auto<T&> {
        using type = T&;
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

        constexpr operator type() const noexcept {
            return value;
        }

        template <typename U>
        constexpr decltype(auto) operator=(U&& b) const noexcept
            requires requires { value = std::forward<U>(b); } {
            return value = std::forward<U>(b);
        }

        constexpr decltype(auto) operator&() const noexcept {
            return &value;
        }

        constexpr decltype(auto) operator->() const noexcept
            requires std::is_pointer_v<T> {
            return value;
        }
    };

    template <typename T>
        requires std::is_class_v<T>
    struct uni_auto<T&> {
        using type = T&;
        type value;

        constexpr uni_auto(type v) noexcept(std::is_nothrow_constructible_v<T, T&>) : value{v} {}

        constexpr operator type() const noexcept(std::is_nothrow_constructible_v<T, T&>) {
            return value;
        }

        template <typename U>
        constexpr decltype(auto) operator=(U&& b) const noexcept(noexcept(value = std::forward<U>(b)))
            requires requires { value = std::forward<U>(b); } {
            return value = std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator+=(U&& b) const noexcept(noexcept(value += std::forward<U>(b)))
            requires requires { value += std::forward<U>(b); } {
            return value += std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator-=(U&& b) const noexcept(noexcept(value -= std::forward<U>(b)))
            requires requires { value -= std::forward<U>(b); } {
            return value -= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator*=(U&& b) const noexcept(noexcept(value *= std::forward<U>(b)))
            requires requires { value *= std::forward<U>(b); } {
            return value *= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator/=(U&& b) const noexcept(noexcept(value /= std::forward<U>(b)))
            requires requires { value /= std::forward<U>(b); } {
            return value /= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator%=(U&& b) const noexcept(noexcept(value %= std::forward<U>(b)))
            requires requires { value %= std::forward<U>(b); } {
            return value %= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator&=(U&& b) const noexcept(noexcept(value &= std::forward<U>(b)))
            requires requires { value &= std::forward<U>(b); } {
            return value &= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator|=(U&& b) const noexcept(noexcept(value |= std::forward<U>(b)))
            requires requires { value |= std::forward<U>(b); } {
            return value |= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator^=(U&& b) const noexcept(noexcept(value ^= std::forward<U>(b)))
            requires requires { value ^= std::forward<U>(b); } {
            return value ^= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator<<=(U&& b) const noexcept(noexcept(value <<= std::forward<U>(b)))
            requires requires { value <<= std::forward<U>(b); } {
            return value <<= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator>>=(U&& b) const noexcept(noexcept(value >>= std::forward<U>(b)))
            requires requires { value >>= std::forward<U>(b); } {
            return value >>= std::forward<U>(b);
        }

        constexpr decltype(auto) operator++() const noexcept(noexcept(++value))
            requires requires { ++value; } {
            return ++value;
        }

        constexpr decltype(auto) operator--() const noexcept(noexcept(--value))
            requires requires { --value; } {
            return --value;
        }

        constexpr decltype(auto) operator++(int) const noexcept(noexcept(value++))
            requires requires { value++; } {
            return value++;
        }

        constexpr decltype(auto) operator--(int) const noexcept(noexcept(value--))
            requires requires { value--; } {
            return value--;
        }

        constexpr decltype(auto) operator+() const noexcept(noexcept(+value))
            requires requires { +value; } {
            return +value;
        }

        constexpr decltype(auto) operator-() const noexcept(noexcept(-value))
            requires requires { -value; } {
            return -value;
        }

        template <typename U>
        constexpr decltype(auto) operator+(U&& b) const noexcept(noexcept(value + std::forward<U>(b)))
            requires requires { value + std::forward<U>(b); } {
            return value + std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator-(U&& b) const noexcept(noexcept(value - std::forward<U>(b)))
            requires requires { value - std::forward<U>(b); } {
            return value - std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator*(U&& b) const noexcept(noexcept(value * std::forward<U>(b)))
            requires requires { value * std::forward<U>(b); } {
            return value * std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator/(U&& b) const noexcept(noexcept(value / std::forward<U>(b)))
            requires requires { value / std::forward<U>(b); } {
            return value / std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator%(U&& b) const noexcept(noexcept(value % std::forward<U>(b)))
            requires requires { value % std::forward<U>(b); } {
            return value % std::forward<U>(b);
        }

        constexpr decltype(auto) operator~() const noexcept(noexcept(~value))
            requires requires { ~value; } {
            return ~value;
        }

        template <typename U>
        constexpr decltype(auto) operator&(U&& b) const noexcept(noexcept(value & std::forward<U>(b)))
            requires requires { value & std::forward<U>(b); } {
            return value & std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator|(U&& b) const noexcept(noexcept(value | std::forward<U>(b)))
            requires requires { value | std::forward<U>(b); } {
            return value | std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator^(U&& b) const noexcept(noexcept(value ^ std::forward<U>(b)))
            requires requires { value ^ std::forward<U>(b); } {
            return value ^ std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator<<(U&& b) const noexcept(noexcept(value << std::forward<U>(b)))
            requires requires { value << std::forward<U>(b); } {
            return value << std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator>>(U&& b) const noexcept(noexcept(value >> std::forward<U>(b)))
            requires requires { value >> std::forward<U>(b); } {
            return value >> std::forward<U>(b);
        }

        constexpr decltype(auto) operator!() const noexcept(noexcept(!value))
            requires requires { !value; } {
            return !value;
        }

        template <typename U>
        constexpr decltype(auto) operator&&(U&& b) const noexcept(noexcept(value && std::forward<U>(b)))
            requires requires { value && std::forward<U>(b); } {
            return value && std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator||(U&& b) const noexcept(noexcept(value || std::forward<U>(b)))
            requires requires { value || std::forward<U>(b); } {
            return value || std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator==(U&& b) const noexcept(noexcept(value == std::forward<U>(b)))
            requires requires { value == std::forward<U>(b); } {
            return value == std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator!=(U&& b) const noexcept(noexcept(value != std::forward<U>(b)))
            requires requires { value != std::forward<U>(b); } {
            return value != std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator<(U&& b) const noexcept(noexcept(value < std::forward<U>(b)))
            requires requires { value < std::forward<U>(b); } {
            return value < std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator>(U&& b) const noexcept(noexcept(value > std::forward<U>(b)))
            requires requires { value > std::forward<U>(b); } {
            return value > std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator<=(U&& b) const noexcept(noexcept(value <= std::forward<U>(b)))
            requires requires { value <= std::forward<U>(b); } {
            return value <= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator>=(U&& b) const noexcept(noexcept(value >= std::forward<U>(b)))
            requires requires { value >= std::forward<U>(b); } {
            return value >= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator<=>(U&& b) const noexcept(noexcept(value <=> std::forward<U>(b)))
            requires requires { value <=> std::forward<U>(b); } {
            return value <=> std::forward<U>(b);
        }

        template <typename ...Args>
        constexpr decltype(auto) operator[](Args&&... args) const noexcept(noexcept(value.operator[](std::forward<Args>(args)...)))
            requires requires { value.operator[](std::forward<Args>(args)...); } {
            return value.operator[](std::forward<Args>(args)...);
        }

        constexpr decltype(auto) operator*() const noexcept(noexcept(*value))
            requires requires { *value; } {
            return *value;
        }

        constexpr decltype(auto) operator&() const noexcept(noexcept(&value))
            requires requires { &value; } {
            return &value;
        }

        constexpr decltype(auto) operator->() const noexcept(noexcept(value.operator->()))
            requires requires { value.operator->(); } {
            return value.operator->();
        }

        template <typename U>
        constexpr decltype(auto) operator->*(U&& b) const noexcept(noexcept(value->*std::forward<U>(b)))
            requires requires { value->*std::forward<U>(b); } {
            return value->*std::forward<U>(b);
        }

        template <typename ...Args>
        constexpr decltype(auto) operator()(Args&&... args) const noexcept(noexcept(value(std::forward<Args>(args)...)))
            requires requires { value(std::forward<Args>(args)...); } {
            return value(std::forward<Args>(args)...);
        }

        template <typename U>
        constexpr decltype(auto) operator,(U&& b) const noexcept(noexcept(value, std::forward<U>(b)))
            requires requires { value, std::forward<U>(b); } {
            return value, std::forward<U>(b);
        }
    };

    template <typename T>
        requires (!std::is_lvalue_reference_v<T> && !std::is_class_v<T>)
    struct uni_auto<T> {
        using type = std::conditional_t<std::is_function_v<T>, T*, T>;
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

        constexpr operator type() const noexcept {
            return value;
        }

        constexpr decltype(auto) operator&() const noexcept {
            return &value;
        }
    };

    template <typename T>
        requires (!std::is_lvalue_reference_v<T> && std::is_class_v<T>)
    struct uni_auto<T> : T {
        using type = T;

        constexpr uni_auto(type v) noexcept(std::is_nothrow_constructible_v<type, type&>) : type{v} {}

        constexpr operator type() const noexcept(std::is_nothrow_constructible_v<type, type&>) {
            return *this;
        }

        constexpr decltype(auto) operator&() const noexcept(noexcept(T::operator&()))
            requires requires { T::operator&(); } {
            return T::operator&();
        }

        constexpr decltype(auto) operator&() const noexcept
            requires (!requires { T::operator&(); }) {
            return this;
        }
    };
    
    /* Deals with C-style arrays and fixed-size C-Strings */
    template <typename T, std::size_t N>
    uni_auto(T(&)[N]) -> uni_auto<T[N]>;

    /* Deals with lvalue references, function pointers, integral and enumeration types, pointers to objects, pointers to member functions and objects, nullptr, etc. (Basically everything else) */
    template <typename T>
        requires (!std::is_const_v<T>)
    uni_auto(T&) -> uni_auto<T&>;
    template <typename T>
        requires (!std::is_pointer_v<T> || std::is_function_v<std::remove_pointer_t<T>>)
    uni_auto(const T&) -> uni_auto<const T>;
    template <typename T>
        requires (std::is_pointer_v<T> && !std::is_function_v<std::remove_pointer_t<T>>)
    uni_auto(const T&) -> uni_auto<const T&>;
    template <typename T>
    uni_auto(T&&) -> uni_auto<T>;

    template <typename T, typename U>
    constexpr decltype(auto) operator+=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) += b.value))
        requires requires { std::forward<T>(a) += b.value; } {
        return std::forward<T>(a) += b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator-=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) -= b.value))
        requires requires { std::forward<T>(a) -= b.value; } {
        return std::forward<T>(a) -= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator*=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) *= b.value))
        requires requires { std::forward<T>(a) *= b.value; } {
        return std::forward<T>(a) *= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator/=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) /= b.value))
        requires requires { std::forward<T>(a) /= b.value; } {
        return std::forward<T>(a) /= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator%=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) %= b.value))
        requires requires { std::forward<T>(a) %= b.value; } {
        return std::forward<T>(a) %= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator&=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) &= b.value))
        requires requires { std::forward<T>(a) &= b.value; } {
        return std::forward<T>(a) &= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator|=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) |= b.value))
        requires requires { std::forward<T>(a) |= b.value; } {
        return std::forward<T>(a) |= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator^=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) ^= b.value))
        requires requires { std::forward<T>(a) ^= b.value; } {
        return std::forward<T>(a) ^= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator<<=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) <<= b.value))
        requires requires { std::forward<T>(a) <<= b.value; } {
        return std::forward<T>(a) <<= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator>>=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) >>= b.value))
        requires requires { std::forward<T>(a) >>= b.value; } {
        return std::forward<T>(a) >>= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator+(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) + b.value))
        requires requires { std::forward<T>(a) + b.value; } {
        return std::forward<T>(a) + b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator-(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) - b.value))
        requires requires { std::forward<T>(a) - b.value; } {
        return std::forward<T>(a) - b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator*(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) * b.value))
        requires requires { std::forward<T>(a) * b.value; } {
        return std::forward<T>(a) * b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator/(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) / b.value))
        requires requires { std::forward<T>(a) / b.value; } {
        return std::forward<T>(a) / b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator%(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) % b.value))
        requires requires { std::forward<T>(a) % b.value; } {
        return std::forward<T>(a) % b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator&(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) & b.value))
        requires requires { std::forward<T>(a) & b.value; } {
        return std::forward<T>(a) & b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator|(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) | b.value))
        requires requires { std::forward<T>(a) | b.value; } {
        return std::forward<T>(a) | b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator^(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) ^ b.value))
        requires requires { std::forward<T>(a) ^ b.value; } {
        return std::forward<T>(a) ^ b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator<<(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) << b.value))
        requires requires { std::forward<T>(a) << b.value; } {
        return std::forward<T>(a) << b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator>>(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) >> b.value))
        requires requires { std::forward<T>(a) >> b.value; } {
        return std::forward<T>(a) >> b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator&&(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) && b.value))
        requires requires { std::forward<T>(a) && b.value; } {
        return std::forward<T>(a) && b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator||(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) || b.value))
        requires requires { std::forward<T>(a) || b.value; } {
        return std::forward<T>(a) || b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator==(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) == b.value))
        requires requires { std::forward<T>(a) == b.value; } {
        return std::forward<T>(a) == b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator!=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) != b.value))
        requires requires { std::forward<T>(a) != b.value; } {
        return std::forward<T>(a) != b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator<(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) < b.value))
        requires requires { std::forward<T>(a) < b.value; } {
        return std::forward<T>(a) < b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator>(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) > b.value))
        requires requires { std::forward<T>(a) > b.value; } {
        return std::forward<T>(a) > b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator<=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) <= b.value))
        requires requires { std::forward<T>(a) <= b.value; } {
        return std::forward<T>(a) <= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator>=(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) >= b.value))
        requires requires { std::forward<T>(a) >= b.value; } {
        return std::forward<T>(a) >= b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator<=>(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) <=> b.value))
        requires requires { std::forward<T>(a) <=> b.value; } {
        return std::forward<T>(a) <=> b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator->*(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a) ->* b.value))
        requires requires { std::forward<T>(a) ->* b.value; } {
        return std::forward<T>(a) ->* b.value;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator,(T&& a, uni_auto<U> b) noexcept(noexcept(std::forward<T>(a), b.value))
        requires requires { std::forward<T>(a), b.value; } {
        return std::forward<T>(a), b.value;
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
     * @brief Similar to `uni_auto_v` but reduces array types to pointers and casts away lvalue and rvalue references.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    constexpr std::decay_t<uni_auto_t<Value>> uni_auto_simplify_v = Value;

    /**
     * @brief Similar to `uni_auto_t` but converts array types to pointers and removes any lvalue and rvalue references.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    using uni_auto_simplify_t = std::remove_const_t<decltype(uni_auto_simplify_v<Value>)>;
}

namespace std {
    template <typename T>
        requires (!std::is_const_v<T>)
    constexpr auto swap(const uninttp::uni_auto<T>& a, const uninttp::uni_auto<T>& b) noexcept(noexcept(std::swap(a.value, b.value))) {
        std::swap(a.value, b.value);
    }
    template <typename T1, typename T2>
        requires (!std::is_const_v<T1>)
    constexpr auto swap(const uninttp::uni_auto<T1>& a, T2& b) noexcept(noexcept(std::swap(a.value, b))) {
        std::swap(a.value, b);
    }
    template <typename T1, typename T2>
        requires (!std::is_const_v<T2>)
    constexpr auto swap(T1& a, const uninttp::uni_auto<T2>& b) noexcept(noexcept(std::swap(a, b.value))) {
        std::swap(a, b.value);
    }
    template <typename T>
    constexpr auto to_array(const uninttp::uni_auto<T>& c) noexcept(noexcept(std::to_array(c.value))) {
        return std::to_array(c.value);
    }
}

#endif /* UNINTTP_UNI_AUTO_HPP */
