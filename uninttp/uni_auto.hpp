#ifndef UNINTTP_UNI_AUTO_HPP
#define UNINTTP_UNI_AUTO_HPP

#include <type_traits>
#include <cstddef>
#include <iterator>

namespace uninttp {
    template <typename T, std::size_t N = 1, bool IsInitializedAsArray = false>
    struct uni_auto {
        using type = std::conditional_t<IsInitializedAsArray, const T(&)[N], const T&>;
        constexpr uni_auto(T const v) : value(v) {}
        constexpr uni_auto(const T* a) {
            for (std::size_t i = 0; i < N; i++)
                value_array[i] = a[i];
        }
        constexpr operator type() const
        requires (!IsInitializedAsArray) {
            return value;
        }
        constexpr operator type() const
        requires (IsInitializedAsArray) {
            return value_array;
        }
        constexpr const T* begin() const
        requires (IsInitializedAsArray) {
            return value_array;
        }
        constexpr const T* end() const
        requires (IsInitializedAsArray) {
            return value_array + N;
        }
        T value {};
        T value_array[N] {};
    };
    template <typename T, std::size_t N>
    uni_auto(T(&)[N]) -> uni_auto<std::remove_cv_t<T>, N, true>;
    template <uni_auto Value>
    using uni_auto_t = typename decltype(Value)::type;
    template <uni_auto Value>
    constexpr auto& uni_auto_v = static_cast<uni_auto_t<Value>>(Value);
}

#endif /* UNINTTP_UNI_AUTO_HPP*/
