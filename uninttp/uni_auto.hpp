/*
              _       _   _         
             (_)     | | | |        
  _   _ _ __  _ _ __ | |_| |_ _ __  
 | | | | '_ \| | '_ \| __| __| '_ \ 
 | |_| | | | | | | | | |_| |_| |_) |
  \__,_|_| |_|_|_| |_|\__|\__| .__/ 
                             | |    
                             |_|    

uninttp (Universal Non-Type Template Parameters)

Copyright (c) 2021-... reacfen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef UNINTTP_UNI_AUTO_HPP
#define UNINTTP_UNI_AUTO_HPP

#include <type_traits>
#include <cstddef>
#include <iterator>

namespace uninttp {
    template <typename T, std::size_t N = 1, bool IsInitializedAsArray = false>
    struct uni_auto {
        using type = std::conditional_t<IsInitializedAsArray, const T(&)[N], const T&>;
        constexpr uni_auto(T const v) : values{v} {}
        constexpr uni_auto(const T* v) {
            for (std::size_t i = 0; i < N; i++)
                values[i] = v[i];
        }
        constexpr operator type() const
        requires (!IsInitializedAsArray) {
            return *values;
        }
        constexpr operator type() const
        requires (IsInitializedAsArray) {
            return values;
        }
        constexpr const T* begin() const
        requires (IsInitializedAsArray) {
            return values;
        }
        constexpr const T* end() const
        requires (IsInitializedAsArray) {
            return values + N;
        }
        T values[N] {};
    };
    template <typename T, std::size_t N>
    uni_auto(T(&)[N]) -> uni_auto<std::remove_cv_t<T>, N, true>;
    template <uni_auto Value>
    using uni_auto_t = typename decltype(Value)::type;
    template <uni_auto Value>
    constexpr auto& uni_auto_v = static_cast<uni_auto_t<Value>>(Value);
}

#endif /* UNINTTP_UNI_AUTO_HPP */
