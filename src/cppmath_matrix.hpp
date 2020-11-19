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
#include <cassert>
#include <type_traits>
#include <vector>
#include <initializer_list>

#include "cppmath_matrix_base.hpp"

namespace cppmath {
namespace matrix{
template <typename difference_type = std::ptrdiff_t>
class RowIterationStrategy: public BaseIterationStrategy
{
    std::size_t m_index = 0;
    
public:
    RowIterationStrategy() = default;
    RowIterationStrategy(const RowIterationStrategy&) = default;
    RowIterationStrategy(RowIterationStrategy&&) = default;
    
    RowIterationStrategy& operator = (const RowIterationStrategy& other) = default;
    RowIterationStrategy& operator = (RowIterationStrategy&& other) = default;
    
    constexpr static inline RowIterationStrategy begin(const IMatrix* matrix){
        assert(matrix);
        return RowIterationStrategy(matrix, 0);
    };
    
    constexpr static inline RowIterationStrategy end(const IMatrix* matrix){
        assert(matrix);
        return RowIterationStrategy(matrix, matrix->size());
    };
    
    constexpr static inline RowIterationStrategy beginAt(const IMatrix* matrix, std::size_t row, std::size_t column){
        assert(matrix);
        return RowIterationStrategy(matrix, row * matrix->columns() + column);
    };
    
    inline void dec(difference_type index = 1){m_index -= index;};
    inline void inc(difference_type index = 1){m_index += index;};
    inline std::size_t index() const {return m_index;};
    inline std::size_t column() const {
        assert(m_matrix);
        return m_matrix->columns() != 0 ? m_index % m_matrix->columns() : 0;
    }
    inline std::size_t row() const {
        assert(m_matrix);
        return m_matrix->columns() != 0 ? m_index / m_matrix->columns() : 0;
    }
    inline difference_type diff(const RowIterationStrategy& other) const {return m_index - other.m_index; }
    
protected:
    RowIterationStrategy(const IMatrix* matrix, std::size_t index): BaseIterationStrategy(matrix), m_index(index){}
};

template <typename T>
class Matrix: public IMatrix{
public:
    typedef T           value_type;
    
    using Iterator = MatrixBaseIterator<Matrix, RowIterationStrategy>;
    using ConstIterator = MatrixBaseIterator<const Matrix, RowIterationStrategy>;
    
    Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    
    Matrix& operator = (const Matrix&) = default;
    Matrix& operator = (Matrix&&) = default;
    
    constexpr Matrix(std::size_t rows, std::size_t columns, const T& val = T()):
        m_data(rows * columns, val),
        m_rows(rows),
        m_columns(columns)
    {
    }
    
    constexpr Matrix(std::size_t rows, std::size_t columns, const std::initializer_list<T>& l, const T& val = T()):
        m_data(l),
        m_rows(rows),
        m_columns(columns)
    {
        m_data.resize(rows * columns, val);
    }
    
    void resize(std::size_t rows, std::size_t columns, const T& val = T()) {
        m_columns = columns;
        m_rows = rows;
        m_data.resize(rows * columns, val);
    }
    
    void reset() {
        m_columns = 0;
        m_rows = 0;
        m_data.clear();
    }
    
    inline void clear() {
        set();
    }
    
    inline void set(const T& val = T()) {
        m_data.assign(m_data.size(), val);
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
    
    constexpr inline Iterator begin(){ return Iterator::begin(this); }
    constexpr inline Iterator beginAt(const MatrixPoint& point){ return Iterator::beginAt(this, point); }
    constexpr inline Iterator end(){ return Iterator::end(this); }
    constexpr inline ConstIterator begin() const { return ConstIterator::begin(this); }
    constexpr inline ConstIterator beginAt(const MatrixPoint& point) const { return ConstIterator::beginAt(this, point); }
    constexpr inline ConstIterator end() const { return ConstIterator::end(this); }
    
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
    std::size_t rows() const override {return m_rows;}
    std::size_t columns() const override {return m_columns;}
    
private:
    std::vector<value_type> m_data;
    std::size_t m_rows = 0;
    std::size_t m_columns = 0;
};

} //namespace matrix
} //namespace cppmath
#endif /* emath_matrix_hpp */
