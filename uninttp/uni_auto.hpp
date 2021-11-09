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
 * Version: v2.4
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
#include <array>

namespace uninttp {
    /**
     * @brief The `uni_auto` class type implementation
     */
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    struct uni_auto;

    template <typename T, std::size_t N, bool IsClassType>
    struct uni_auto<T, N, true, IsClassType> {
        using type = T(&)[N];
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

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

        constexpr auto rbegin() const noexcept {
            return std::rbegin(value);
        }

        constexpr auto rend() const noexcept {
            return std::rend(value);
        }
    };

    template <typename T, std::size_t N, bool IsClassType>
    struct uni_auto<const T, N, true, IsClassType> {
        using type = const T(&)[N];
        std::remove_reference_t<type> value;

        template <std::size_t ...Indices>
        constexpr uni_auto(type v, std::index_sequence<Indices...>) noexcept(noexcept(T{std::declval<T>()}))
            : value{v[Indices]...} {}

        constexpr uni_auto(type v) noexcept : uni_auto{v, std::make_index_sequence<N>()} {}

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

        constexpr auto rbegin() const noexcept {
            return std::rbegin(value);
        }

        constexpr auto rend() const noexcept {
            return std::rend(value);
        }
    };

    template <typename T>
    struct uni_auto<T, 0, false, true> : T {
        using type = T;

        constexpr uni_auto(type v) noexcept(noexcept(type{v})) : type{v} {}

        constexpr operator type() const noexcept {
            return *this;
        }
    };

    template <typename T>
    struct uni_auto<T*, 0, false, true> {
        using type = T*&;
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

        constexpr operator type() const noexcept {
            return value;
        }

        constexpr type operator->() const noexcept {
            return value;
        }
    };

    template <typename R, typename ...Args>
    struct uni_auto<R (Args ...), 0, false, false> {
        using type = R (*)(Args ...);
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

        constexpr operator type() const noexcept {
            return value;
        }

        constexpr auto operator&() const noexcept = delete;

        constexpr R operator()(Args&&... args) const noexcept(noexcept(value(std::forward<Args>(args)...))) {
            return value(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    struct uni_auto<T, 0, false, false> {
        using type = T;
        type value;

        constexpr uni_auto(type v) noexcept(noexcept(type{v})) : value{v} {}

        constexpr operator type() const noexcept {
            return value;
        }
    };

    template <typename T>
    struct uni_auto<T&, 0, false, false> {
        using type = T&;
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

        template <typename U>
        constexpr decltype(auto) operator=(U&& b) const noexcept(noexcept(value = std::forward<U>(b))) {
            return value = std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator+=(U&& b) const noexcept(noexcept(value += std::forward<U>(b))) {
            return value += std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator-=(U&& b) const noexcept(noexcept(value -= std::forward<U>(b))) {
            return value -= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator*=(U&& b) const noexcept(noexcept(value *= std::forward<U>(b))) {
            return value *= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator/=(U&& b) const noexcept(noexcept(value /= std::forward<U>(b))) {
            return value /= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator%=(U&& b) const noexcept(noexcept(value %= std::forward<U>(b))) {
            return value %= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator&=(U&& b) const noexcept(noexcept(value &= std::forward<U>(b))) {
            return value &= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator|=(U&& b) const noexcept(noexcept(value |= std::forward<U>(b))) {
            return value |= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator^=(U&& b) const noexcept(noexcept(value ^= std::forward<U>(b))) {
            return value ^= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator<<=(U&& b) const noexcept(noexcept(value <<= std::forward<U>(b))) {
            return value <<= std::forward<U>(b);
        }

        template <typename U>
        constexpr decltype(auto) operator>>=(U&& b) const noexcept(noexcept(value >>= std::forward<U>(b))) {
            return value >>= std::forward<U>(b);
        }

        constexpr decltype(auto) operator++() const noexcept(noexcept(++value)) {
            return ++value;
        }

        constexpr decltype(auto) operator--() const noexcept(noexcept(--value)) {
            return --value;
        }

        constexpr decltype(auto) operator++(int) const noexcept(noexcept(value++)) {
            return value++;
        }

        constexpr decltype(auto) operator--(int) const noexcept(noexcept(value--)) {
            return value--;
        }

        constexpr operator type() const noexcept {
            return value;
        }
    };

    template <typename T>
    struct uni_auto<T&, 0, false, true> {
        using type = T&;
        type value;

        constexpr uni_auto(type v) noexcept : value{v} {}

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

        template <typename U>
        constexpr decltype(auto) operator[](U&& b) const noexcept(noexcept(value[std::forward<U>(b)]))
            requires requires { value[std::forward<U>(b)]; } {
            return value[std::forward<U>(b)];
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

        constexpr operator type() const noexcept {
            return value;
        }
    };

    /* Deals with C-style arrays */
    template <typename T, std::size_t N>
    uni_auto(T (&)[N]) -> uni_auto<T, N, true, false>;

    /* Deals with function pointers */
    template <typename R, typename ...Args>
    uni_auto(R (Args ...)) -> uni_auto<R (Args ...), 0, false, false>;

    /* Deals with integral and enumeration types, pointers to objects, pointers to member functions and objects, nullptr */
    template <typename T>
        requires std::is_class_v<T>
    uni_auto(const T&) -> uni_auto<const T, 0, false, true>;
    template <typename T>
        requires (!(std::is_class_v<std::remove_pointer_t<T>> || std::is_array_v<T>))
    uni_auto(const T&) -> uni_auto<const T, 0, false, false>;

    /* Deals with lvalue references */
    template <typename T>
        requires std::is_class_v<T>
    uni_auto(T&) -> uni_auto<T&, 0, false, true>;
    template <typename T>
        requires (!(std::is_class_v<std::remove_pointer_t<T>> || std::is_array_v<T>))
    uni_auto(T&) -> uni_auto<T&, 0, false, false>;

    /* Deals with pointer to class types */
    template <typename T>
        requires (std::is_pointer_v<T> && std::is_class_v<std::remove_pointer_t<T>>)
    uni_auto(T) -> uni_auto<T, 0, false, true>;

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(T1&& a, const uni_auto<T2, 0, false, false>& b) noexcept {
        return [&] <typename ...Args>(Args&&... args) constexpr
        noexcept(noexcept((std::forward<T1>(a).*static_cast<T2>(b))(std::forward<Args>(args)...))) {
            return (std::forward<T1>(a).*static_cast<T2>(b))(std::forward<Args>(args)...);
        };
    }

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(const uni_auto<T1, 0, false, true>& a, T2&& b) noexcept {
        return [&] <typename ...Args>(Args&&... args) constexpr
        noexcept(noexcept((static_cast<T1>(a).*std::forward<T2>(b))(std::forward<Args>(args)...))) {
            return (static_cast<T1>(a).*std::forward<T2>(b))(std::forward<Args>(args)...);
        };
    }

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(T1* a, const uni_auto<T2, 0, false, false>& b) noexcept {
        return [a, &b] <typename ...Args>(Args&&... args) constexpr
        noexcept(noexcept((a->*static_cast<T2>(b))(std::forward<Args>(args)...))) {
            return (a->*static_cast<T2>(b))(std::forward<Args>(args)...);
        };
    }

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(const uni_auto<T1*, 0, false, true>& a,
                               const uni_auto<T2, 0, false, false>& b) noexcept {
        return [&] <typename ...Args>(Args&&... args) constexpr
        noexcept(noexcept((static_cast<T1*>(a)->*static_cast<T2>(b))(std::forward<Args>(args)...))) {
            return (static_cast<T1*>(a)->*static_cast<T2>(b))(std::forward<Args>(args)...);
        };
    }

    /**
     * @brief Member access `operator->*()` overload for convenience working with 'uni_auto' and pointers to member functions.
     */
    template <typename T1, typename T2>
    constexpr auto operator->*(const uni_auto<T1*, 0, false, true>& a, T2&& b) noexcept {
        return [&] <typename ...Args>(Args&&... args) constexpr
        noexcept(noexcept((static_cast<T1*>(a)->*std::forward<T2>(b))(std::forward<Args>(args)...))) {
            return (static_cast<T1*>(a)->*std::forward<T2>(b))(std::forward<Args>(args)...);
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
        static_cast<std::conditional_t<std::is_array_v<std::remove_reference_t<uni_auto_t<Value>>>,
                                       std::remove_extent_t<std::remove_reference_t<uni_auto_t<Value>>>*,
                                       uni_auto_t<Value>>>(Value);

    /**
     * @brief Fetches the type after "simplification" of a `uni_auto` object.
     * @tparam Value The `uni_auto` object
     */
    template <uni_auto Value>
    using uni_auto_simplify_t = std::remove_const_t<decltype(uni_auto_simplify_v<Value>)>;
}

namespace std {
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    constexpr auto swap(const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& a, const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& b) noexcept {
        return std::swap(a.value, b.value);
    }
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    constexpr auto cbegin(const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& c) noexcept {
        return std::cbegin(c.value);
    }
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    constexpr auto cend(const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& c) noexcept {
        return std::cend(c.value);
    }
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    constexpr auto crbegin(const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& c) {
        return std::crbegin(c.value);
    }
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    constexpr auto crend(const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& c) {
        return std::crend(c.value);
    }
    template <typename T, std::size_t N, bool IsArrayType, bool IsClassType>
    constexpr auto to_array(const uninttp::uni_auto<T, N, IsArrayType, IsClassType>& c) {
        return std::to_array(c.value);
    }
}

#endif /* UNINTTP_UNI_AUTO_HPP */
