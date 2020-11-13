//
//  cppmath_matrix_base.hpp
//  CppMath
//
//  Created by Dimonych on 13.11.20.
//  Copyright © 2020  Dmytro Krasnianskyi. All rights reserved.
//

#ifndef cppmath_matrix_base_hpp
#define cppmath_matrix_base_hpp

#include <stdio.h>
#include <cstddef>

namespace cppmath {

struct MatrixPoint{
    std::size_t row = 0;
    std::size_t column = 0;
};

class IMatrix{
public:
    virtual ~IMatrix() = default;
    virtual std::size_t size() const = 0;
    virtual std::size_t rows() const {return size()/columns();}
    virtual std::size_t columns() const = 0;
    constexpr inline bool isSquareMatrix() const {return columns() == rows();}
    constexpr inline bool isColumnVector() const {return columns() == 1;}
    constexpr inline bool isRowVector() const {return rows() == 1;}
    constexpr inline bool isVector() const {return isColumnVector() || isRowVector();}
};

}//namespace cppmath

#endif /* cppmath_matrix_base_hpp */
