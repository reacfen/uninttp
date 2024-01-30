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
 * Version: v4.0.1
 *
 * Copyright (c) 2021-24 reacfen
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

#if __cplusplus < 202002L
    #error uninttp can only be used with C++20 or later.
#endif

#include <string_view>
#include <type_traits>
#include <iterator>
#include <cstddef>
#include <utility>
#include <format>
#include <array>

namespace uninttp {
    /**
     * @brief The `uni_auto` class type implementation
     */
    template <typename T>
    struct uni_auto;

    namespace uninttp_internals {
        template <typename T>
        struct is_string_view : std::false_type {};

        template <typename CharT, typename Traits>
        struct is_string_view<std::basic_string_view<CharT, Traits>> : std::true_type {};

        template <typename T>
        struct is_uni_auto : std::false_type {};

        template <typename T>
        struct is_uni_auto<uni_auto<T>> : std::true_type {};
    }

    template <typename T, std::size_t N>
    struct uni_auto<const T[N]> final {
        using type = const T(&)[N];
        std::remove_reference_t<type> value;

        template <std::size_t ...Indices>
        constexpr uni_auto(type v, std::index_sequence<Indices...>) noexcept(std::is_nothrow_constructible_v<T, T&>) : value{ v[Indices]... } {}

        constexpr uni_auto(type v) noexcept(noexcept(uni_auto{ v, std::make_index_sequence<N>() })) : uni_auto{ v, std::make_index_sequence<N>() } {}

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
    };

    template <typename T>
    struct uni_auto<T&> final {
        using type = T&;
        type value;

        constexpr uni_auto(type v) noexcept : value{ v } {}

        constexpr operator type() const noexcept {
            return value;
        }

        static constexpr auto size() noexcept
            requires std::is_array_v<T> {
            return std::extent_v<T>;
        }

        constexpr auto swap(const uni_auto& other) const noexcept {
            using std::swap;
            swap(value, other.value);
        }

        constexpr auto data() const noexcept
            requires std::is_array_v<T> {
            return std::data(value);
        }

        constexpr auto empty() const noexcept
            requires std::is_array_v<T> {
            return std::empty(value);
        }

        constexpr auto begin() const noexcept
            requires std::is_array_v<T> {
            return std::begin(value);
        }

        constexpr auto end() const noexcept
            requires std::is_array_v<T> {
            return std::end(value);
        }

        constexpr auto cbegin() const noexcept
            requires std::is_array_v<T> {
            return std::cbegin(value);
        }

        constexpr auto cend() const noexcept
            requires std::is_array_v<T> {
            return std::cend(value);
        }

        constexpr auto rbegin() const noexcept
            requires std::is_array_v<T> {
            return std::rbegin(value);
        }

        constexpr auto rend() const noexcept
            requires std::is_array_v<T> {
            return std::rend(value);
        }

        constexpr auto crbegin() const noexcept
            requires std::is_array_v<T> {
            return std::crbegin(value);
        }

        constexpr auto crend() const noexcept
            requires std::is_array_v<T> {
            return std::crend(value);
        }

        template <typename U>
        constexpr decltype(auto) operator=(U&& b) const noexcept(noexcept(value = std::forward<U>(b)))
            requires requires { value = std::forward<U>(b); } {
            return value = std::forward<U>(b);
        }

        template <typename ...Args>
        constexpr decltype(auto) operator[](Args&&... args) const noexcept(noexcept(value.operator[](std::forward<Args>(args)...)))
            requires requires { value.operator[](std::forward<Args>(args)...); } {
            return value.operator[](std::forward<Args>(args)...);
        }

        constexpr decltype(auto) operator->() const noexcept(noexcept(value.operator->()))
            requires requires { value.operator->(); } {
            return value.operator->();
        }

        template <typename ...Args>
        constexpr decltype(auto) operator()(Args&&... args) const noexcept(noexcept(value(std::forward<Args>(args)...)))
            requires requires { value(std::forward<Args>(args)...); } {
            return value(std::forward<Args>(args)...);
        }
    };

    template <typename T>
        requires (!std::is_class_v<T>)
    struct uni_auto<T> final {
        using type = T;
        type value;

        constexpr uni_auto(type v) noexcept : value{ v } {}

        constexpr operator type() const noexcept {
            return value;
        }

        constexpr decltype(auto) operator->() const noexcept
            requires std::is_pointer_v<type> {
            return value;
        }
    };

    template <typename T>
        requires std::is_class_v<T>
    struct uni_auto<T> final : T {
        using type = T;

        constexpr uni_auto(const type& v) noexcept(std::is_nothrow_constructible_v<type, type&>) : type{ v } {}

        constexpr operator const type&() const noexcept {
            return *this;
        }
    };

    /* Deals with `constexpr`-marked built-in arrays and fixed-size C-Strings/string literals */
    template <typename T, std::size_t N>
    uni_auto(const T(&)[N]) -> uni_auto<const T[N]>;

    /* Deals with built-in arrays, lvalue references, function pointers, integral and enumeration types, pointers to objects, pointers to member functions and objects, nullptr, etc. (Basically everything else) */
    template <typename T>
    uni_auto(T&&) -> uni_auto<T>;
    template <typename T>
        requires (!std::is_const_v<T>)
    uni_auto(T&) -> uni_auto<T&>;
    template <typename T>
        requires (!(
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const char>          ||
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const signed char>   ||
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const unsigned char> ||
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const wchar_t>       ||
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const char8_t>       ||
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const char16_t>      ||
            std::is_same_v<std::remove_volatile_t<std::remove_pointer_t<T>>, const char32_t>
        ) && !std::is_volatile_v<T> && !uninttp_internals::is_string_view<T>::value && std::is_trivially_copyable_v<T> && (std::is_scalar_v<std::remove_pointer_t<T>> || std::is_class_v<std::remove_pointer_t<T>> || std::is_function_v<std::remove_pointer_t<T>>))
    uni_auto(const T&) -> uni_auto<const T>;
    template <typename T>
    uni_auto(const T&) -> uni_auto<const T&>;

    template <typename T>
    constexpr decltype(auto) operator++(const uni_auto<T>& a) noexcept(noexcept(++a.operator typename uni_auto<T>::type()))
        requires requires { ++a.operator typename uni_auto<T>::type(); } {
        return ++a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator--(const uni_auto<T>& a) noexcept(noexcept(--a.operator typename uni_auto<T>::type()))
        requires requires { --a.operator typename uni_auto<T>::type(); } {
        return --a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator++(const uni_auto<T>& a, int) noexcept(noexcept(a.operator typename uni_auto<T>::type()++))
        requires requires { a.operator typename uni_auto<T>::type()++; } {
        return a.operator typename uni_auto<T>::type()++;
    }

    template <typename T>
    constexpr decltype(auto) operator--(const uni_auto<T>& a, int) noexcept(noexcept(a.operator typename uni_auto<T>::type()--))
        requires requires { a.operator typename uni_auto<T>::type()--; } {
        return a.operator typename uni_auto<T>::type()--;
    }

    template <typename T>
    constexpr decltype(auto) operator+(const uni_auto<T>& a) noexcept(noexcept(+a.operator typename uni_auto<T>::type()))
        requires requires { +a.operator typename uni_auto<T>::type(); } {
        return +a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator-(const uni_auto<T>& a) noexcept(noexcept(-a.operator typename uni_auto<T>::type()))
        requires requires { -a.operator typename uni_auto<T>::type(); } {
        return -a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator~(const uni_auto<T>& a) noexcept(noexcept(~a.operator typename uni_auto<T>::type()))
        requires requires { ~a.operator typename uni_auto<T>::type(); } {
        return ~a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator!(const uni_auto<T>& a) noexcept(noexcept(!a.operator typename uni_auto<T>::type()))
        requires requires { !a.operator typename uni_auto<T>::type(); } {
        return !a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator*(const uni_auto<T>& a) noexcept(noexcept(*a.operator typename uni_auto<T>::type()))
        requires requires { *a.operator typename uni_auto<T>::type(); } {
        return *a.operator typename uni_auto<T>::type();
    }

    template <typename T>
    constexpr decltype(auto) operator&(const uni_auto<T>& a) noexcept(noexcept(&a.operator typename uni_auto<T>::type())) {
        return &a.operator typename uni_auto<T>::type();
    }

    template <typename T>
        requires std::is_class_v<T>
    constexpr decltype(auto) operator&(const uni_auto<T>& a) noexcept {
        return &a.operator const typename uni_auto<T>::type&();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator+=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() += b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() += b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() += b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator-=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() -= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() -= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() -= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator*=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() *= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() *= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() *= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator/=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() /= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() /= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() /= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator%=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() %= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() %= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() %= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator&=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() &= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() &= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() &= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator|=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() |= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() |= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() |= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator^=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() ^= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() ^= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() ^= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator<<=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() <<= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() <<= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() <<= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator>>=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() >>= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() >>= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() >>= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator+(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() + b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() + b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() + b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator-(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() - b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() - b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() - b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator*(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() * b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() * b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() * b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator/(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() / b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() / b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() / b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator%(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() % b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() % b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() % b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator&(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() & b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() & b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() & b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator|(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() | b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() | b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() | b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator^(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() ^ b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() ^ b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() ^ b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator<<(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() << b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() << b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() << b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator>>(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() >> b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() >> b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() >> b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator&&(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() && b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() && b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() && b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator||(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() || b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() || b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() || b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator==(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() == b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() == b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() == b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator!=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() != b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() != b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() != b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator<(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() < b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() < b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() < b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator>(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() > b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() > b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() > b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator<=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() <= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() <= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() <= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator>=(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() >= b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() >= b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() >= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator<=>(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() <=> b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() <=> b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() <=> b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator->*(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() ->* b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type() ->* b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type() ->* b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!std::is_class_v<T>)
    constexpr decltype(auto) operator,(const uni_auto<T>& a, const uni_auto<U>& b) noexcept(noexcept(a.operator typename uni_auto<T>::type(), b.operator typename uni_auto<U>::type()))
        requires requires { a.operator typename uni_auto<T>::type(), b.operator typename uni_auto<U>::type(); } {
        return a.operator typename uni_auto<T>::type(), b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator+=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) += b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) += b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) += b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator-=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) -= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) -= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) -= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator*=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) *= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) *= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) *= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator/=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) /= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) /= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) /= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator%=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) %= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) %= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) %= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator&=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) &= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) &= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) &= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator|=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) |= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) |= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) |= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator^=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) ^= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) ^= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) ^= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator<<=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) <<= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) <<= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) <<= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator>>=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) >>= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) >>= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) >>= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator+(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) + b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) + b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) + b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator-(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) - b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) - b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) - b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator*(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) * b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) * b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) * b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator/(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) / b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) / b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) / b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator%(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) % b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) % b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) % b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator&(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) & b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) & b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) & b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator|(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) | b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) | b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) | b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator^(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) ^ b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) ^ b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) ^ b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator<<(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) << b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) << b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) << b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator>>(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) >> b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) >> b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) >> b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator&&(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) && b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) && b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) && b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator||(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) || b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) || b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) || b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator==(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) == b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) == b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) == b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator!=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) != b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) != b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) != b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator<(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) < b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) < b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) < b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator>(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) > b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) > b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) > b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator<=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) <= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) <= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) <= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator>=(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) >= b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) >= b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) >= b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator<=>(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) <=> b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) <=> b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) <=> b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator->*(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a) ->* b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a) ->* b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a) ->* b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<T>>::value)
    constexpr decltype(auto) operator,(T&& a, const uni_auto<U>& b) noexcept(noexcept(std::forward<T>(a), b.operator typename uni_auto<U>::type()))
        requires requires { std::forward<T>(a), b.operator typename uni_auto<U>::type(); } {
        return std::forward<T>(a), b.operator typename uni_auto<U>::type();
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator+=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() += std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() += std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() += std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator-=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() -= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() -= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() -= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator*=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() *= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() *= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() *= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator/=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() /= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() /= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() /= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator%=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() %= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() %= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() %= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator&=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() &= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() &= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() &= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator|=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() |= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() |= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() |= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator^=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() ^= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() ^= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() ^= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator<<=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() <<= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() <<= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() <<= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator>>=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() >>= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() >>= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() >>= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator+(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() + std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() + std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() + std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator-(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() - std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() - std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() - std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator*(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() * std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() * std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() * std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator/(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() / std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() / std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() / std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator%(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() % std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() % std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() % std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator&(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() & std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() & std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() & std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator|(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() | std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() | std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() | std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator^(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() ^ std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() ^ std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() ^ std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator<<(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() << std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() << std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() << std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator>>(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() >> std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() >> std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() >> std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator&&(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() && std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() && std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() && std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator||(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() || std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() || std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() || std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator==(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() == std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() == std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() == std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator!=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() != std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() != std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() != std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator<(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() < std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() < std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() < std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator>(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() > std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() > std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() > std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator<=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() <= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() <= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() <= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator>=(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() >= std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() >= std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() >= std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator<=>(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() <=> std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() <=> std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() <=> std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator->*(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type() ->* std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type() ->* std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type() ->* std::forward<U>(b);
    }

    template <typename T, typename U>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<U>>::value)
    constexpr decltype(auto) operator,(const uni_auto<T>& a, U&& b) noexcept(noexcept(a.operator typename uni_auto<T>::type(), std::forward<U>(b)))
        requires requires { a.operator typename uni_auto<T>::type(), std::forward<U>(b); } {
        return a.operator typename uni_auto<T>::type(), std::forward<U>(b);
    }

    /**
     * @brief Fetches the type of the underlying value held by a `uni_auto` object.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    using uni_auto_t = std::remove_const_t<typename decltype(Value)::type>;

    /**
     * @brief Fetches the underlying value held by a `uni_auto` object.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    constexpr uni_auto_t<Value> uni_auto_v = Value;

    /**
     * @brief Similar to `uni_auto_t` but converts array types and function types to pointers and removes any references from the type returned.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    using uni_auto_simplify_t = std::decay_t<typename decltype(Value)::type>;

    /**
     * @brief Similar to `uni_auto_v` but reduces array types and function types to pointers and casts away any and all references.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    constexpr uni_auto_simplify_t<Value> uni_auto_simplify_v = Value;

    /**
     * @brief Pre-constructs a `uni_auto` object after binding an lvalue to a reference.
     * @tparam Value The value that the reference will bind to
     */
    template <auto& Value>
        requires (!uninttp_internals::is_uni_auto<std::remove_cvref_t<decltype(Value)>>::value)
    constexpr uni_auto<decltype(Value)> promote_to_ref = Value;

    /**
     * @brief Exchanges the values held by `a` and `b`.
     */
    template <typename T>
    constexpr auto swap(const uni_auto<T>& a, const uni_auto<T>& b) noexcept {
        using std::swap;
        swap(a.operator typename uni_auto<T>::type(), b.operator typename uni_auto<T>::type());
    }

    /**
     * @brief Exchanges the values held by `a` and `b`.
     */
    template <typename T1, typename T2>
    constexpr auto swap(const uni_auto<T1>& a, T2& b) noexcept {
        using std::swap;
        swap(a.operator typename uni_auto<T1>::type(), b);
    }

    /**
     * @brief Exchanges the values held by `a` and `b`.
     */
    template <typename T1, typename T2>
    constexpr auto swap(T1& a, const uni_auto<T2>& b) noexcept {
        using std::swap;
        swap(a, b.operator typename uni_auto<T2>::type());
    }

    /**
     * @brief Creates an `std::array` object using `a`.
     */
    template <typename T>
        requires std::is_array_v<std::remove_reference_t<T>>
    constexpr auto to_array(const uni_auto<T>& a) noexcept {
        return std::to_array(a.value);
    }
}

template <typename T>
struct std::formatter<uninttp::uni_auto<T>> : formatter<decay_t<T>> {
    template <typename FormatContext>
    auto format(const uninttp::uni_auto<T>& a, FormatContext& c) const {
        return formatter<decay_t<T>>::format(a.operator typename uninttp::uni_auto<T>::type(), c);
    }
};

#endif /* UNINTTP_UNI_AUTO_HPP */
