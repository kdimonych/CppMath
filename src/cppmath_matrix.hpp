//
//  cppmath_matrix.hpp
//  CppMath
//
//  Created by Dmytro Krasnianskyi on 11.11.20.
//  Copyright © 2020 Dmytro Krasnianskyi. All rights reserved.
//

#ifndef cppmath_matrix_hpp
#define cppmath_matrix_hpp

#include <cstddef>
#include <type_traits>
#include <vector>
#include <initializer_list>

#include "cppmath_matrix_base.hpp"

namespace cppmath {

template <typename difference_type = std::ptrdiff_t>
class RowIterationStrategy: public MatrixBaseIterationStrategy
{
    std::size_t m_index = 0;
    
public:
    using ThisT = RowIterationStrategy<difference_type>;
    
    RowIterationStrategy() = default;
    RowIterationStrategy(const RowIterationStrategy&) = default;
    RowIterationStrategy(RowIterationStrategy&&) = default;
    
    RowIterationStrategy& operator = (const RowIterationStrategy& other) = default;
    RowIterationStrategy& operator = (RowIterationStrategy&& other) = default;
    
    constexpr static inline ThisT begin(const IMatrix* matrix){
        return ThisT(matrix, 0);
    };
    
    constexpr static inline ThisT end(const IMatrix* matrix){
        return ThisT(matrix, matrix->size());
    };
    
    constexpr static inline ThisT beginAt(const IMatrix* matrix, std::size_t row, std::size_t column){
        return ThisT(matrix, row * matrix->columns() + column);
    };
    
    inline void dec(difference_type index = 1){m_index -= index;};
    inline void inc(difference_type index = 1){m_index += index;};
    inline std::size_t index() const {return m_index;};
    inline std::size_t column() const {return m_index / m_matrix->rows();}
    inline std::size_t row() const {return m_index / m_matrix->columns();}
    inline difference_type diff(const RowIterationStrategy& other) const {return m_index - other.m_index; }
    
protected:
    RowIterationStrategy(const IMatrix* matrix, std::size_t index): MatrixBaseIterationStrategy(matrix), m_index(index){}
};

template <typename T>
class Matrix: public IMatrix{
public:
    typedef T           value_type;
    //typedef std::size_t   index_type;
    
    using RowIterator = MatrixBaseIterator<Matrix, RowIterationStrategy>;
    using RowConstIterator = MatrixBaseIterator<const Matrix, RowIterationStrategy>;
    
    Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    
    Matrix& operator = (const Matrix&) = default;
    Matrix& operator = (Matrix&&) = default;
    
    Matrix(std::size_t rows, std::size_t columns, const T& val = T()):
        m_data(rows * columns, val),
        m_columns(columns)
    {}
    
    Matrix(std::size_t rows, std::size_t columns, const std::initializer_list<T>& l, const T& val = T()):
        m_data(l),
        m_columns(columns)
    {
        m_data.resize(rows * columns, val);
    }
    
    void resize(std::size_t rows, std::size_t columns, const T& val = T())
    {
        m_data.resize(rows * columns, val);
        m_columns = columns;
    }
    
    inline T& operator [] (const MatrixPoint& point) {
        return m_data.at(point.row * m_columns + point.column);
    }
    
    inline const T& operator [] (const MatrixPoint& point) const{
        return m_data.at(point.row * m_columns + point.column);
    }
    
    inline T& operator [] (std::size_t index) {
        return m_data.at(index);
    }
    
    inline const T& operator [] (std::size_t index) const {
        return m_data.at(index);
    }
    
    constexpr inline RowIterator begin(){ return RowIterator::begin(this); }
    constexpr inline RowIterator beginAt(const MatrixPoint& point){ return RowIterator::beginAt(this, point); }
    constexpr inline RowIterator end(){ return RowIterator::end(this); }
    constexpr inline RowConstIterator begin() const { return RowConstIterator::begin(this); }
    constexpr inline RowConstIterator beginAt(const MatrixPoint& point) const { return RowConstIterator::beginAt(this, point); }
    constexpr inline RowConstIterator end() const { return RowConstIterator::end(this); }
    
    template <template <typename _DifferenceType> class IterationStrategy>
    constexpr inline MatrixBaseIterator<Matrix, IterationStrategy>
    begin(){
        return MatrixBaseIterator<Matrix, IterationStrategy>::begin(this);
    }
    template <template <typename _DifferenceType> class IterationStrategy>
    constexpr inline MatrixBaseIterator<Matrix, IterationStrategy>
    beginAt(const MatrixPoint& point){
        return MatrixBaseIterator<Matrix, IterationStrategy>::beginAt(this, point.row, point.column);
    }
    template <template <typename _DifferenceType> class IterationStrategy>
    constexpr inline MatrixBaseIterator<Matrix, IterationStrategy>
    end(){
        return MatrixBaseIterator<Matrix, IterationStrategy>::end(this);
    }
    
    template <template <typename _DifferenceType> class IterationStrategy>
    constexpr inline MatrixBaseIterator<const Matrix, IterationStrategy>
    begin() const {
        return MatrixBaseIterator<const Matrix, IterationStrategy>::begin(this);
    }
    template <template <typename _DifferenceType> class IterationStrategy>
    constexpr inline MatrixBaseIterator<const Matrix, IterationStrategy>
    beginAt(const MatrixPoint& point) const {
        return MatrixBaseIterator<const Matrix, IterationStrategy>::beginAt(this, point.row, point.column);
    }
    template <template <typename _DifferenceType> class IterationStrategy>
    constexpr inline MatrixBaseIterator<const Matrix, IterationStrategy>
    end() const {
        return MatrixBaseIterator<const Matrix, IterationStrategy>::end(this);
    }
    
    std::size_t size()  const override {return m_data.size();}
    std::size_t rows() const override {return m_data.size()/m_columns;}
    std::size_t columns() const override {return m_columns;}
    
private:
    std::vector<value_type> m_data;
    std::size_t m_columns = 1;
};

} //namespace ccpmath
#endif /* emath_matrix_hpp */
