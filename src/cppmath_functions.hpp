//
//  cppmath_functions.hpp
//  CppMath
//
//  Created by Dimonych on 20.11.20.
//

#ifndef cppmath_functions_hpp
#define cppmath_functions_hpp

#include <stdio.h>
#include <cstddef>

namespace cppmath {
namespace functions {
    
    template<typename T, typename Result = size_t>
    constexpr Result factorial(T n, Result res = 1) noexcept
    {
        if(n >= 0)
        {
            for(;n != 0; --n) res *= n;
            return res;
        }
        else
            return 1;
    }
    
    template<typename T, typename R = size_t>
    constexpr R matrixCountPaths(T m, T n) noexcept
    {
        return factorial<T, R>(m + n)/(factorial<T, R>(m) + factorial<T, R>(n));
    }
} // namespace functions
} // namespace cppmath
#endif /* cppmath_functions_hpp */
