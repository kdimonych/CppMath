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
#include <iterator>
#include <algorithm>
#include <initializer_list>

#include "cppmath_matrix_base.hpp"
#include "cppmath_functions.hpp"

/** Naming convention:
    "Idx" or "idx" - index
 */

namespace cppmath {
namespace matrix{
    
template <class MatrixT>
class RowIterator final: public MatrixBaseIterator<MatrixT, RowIterator<MatrixT>>
{
    std::size_t m_index = 0;
    
public:
    using BaseT = MatrixBaseIterator<MatrixT, RowIterator<MatrixT>>;
    using MatrixBaseIterator<MatrixT, RowIterator<MatrixT>>::MatrixBaseIterator;
    
    constexpr static inline RowIterator begin(MatrixT* matrix){
        assert(matrix);
        return RowIterator(matrix, 0);
    };
    
    constexpr static inline RowIterator end(MatrixT* matrix){
        assert(matrix);
        return RowIterator(matrix, matrix->size());
    };
    
    constexpr static inline RowIterator beginAt(MatrixT* matrix, std::size_t row, std::size_t column){
        assert(matrix);
        return RowIterator(matrix, row * matrix->columns() + column);
    };
    
    inline std::size_t index() const noexcept {return m_index;};
    inline std::size_t column() const {
        assert(this->m_matrix);
        return this->m_matrix->columns() != 0 ? m_index % this->m_matrix->columns() : 0;
    }
    inline std::size_t row() const {
        assert(this->m_matrix);
        return this->m_matrix->columns() != 0 ? m_index / this->m_matrix->columns() : 0;
    }
    
    
protected:
    friend BaseT;
    inline void dec(typename BaseT::difference_type index) {m_index -= index;};
    inline void inc(typename BaseT::difference_type index) {m_index += index;};
    inline typename BaseT::difference_type diff(const RowIterator& other) const {return m_index - other.m_index; }
    
    RowIterator(MatrixT* matrix, std::size_t index): BaseT(matrix), m_index(index){}
};

struct MatrixPoint{
    std::size_t row = 0;
    std::size_t column = 0;
};

template <typename T>
class Matrix {
public:
    typedef T           value_type;
    typedef T&          reference;
    typedef const T&    const_reference;
    
    using Iterator = RowIterator<Matrix>;
    using ConstIterator = RowIterator<const Matrix>;
    
    /**
            Constructors
     */
    Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    
    Matrix& operator = (const Matrix&) = default;
    Matrix& operator = (Matrix&&) = default;
    
    constexpr Matrix(std::size_t rows, std::size_t columns, const T& val = T()):
        m_data(rows * columns, val),
        m_rows(rows),
        m_columns(columns)
    {}
    
    template <int R, int C>
    constexpr Matrix(T const (& arr) [R][C]):
        m_data(std::begin(arr[0]), std::end(arr[R - 1])),
        m_rows(R),
        m_columns(C)
    {}

    constexpr Matrix(std::size_t rows, std::size_t columns, const std::initializer_list<T>& l, const T& val = T()):
        m_data(l),
        m_rows(rows),
        m_columns(columns)
    {
        m_data.resize(rows * columns, val);
    }
        
    inline void assign(const T& val = T()) { m_data.assign(m_data.size(), val); }
    
    /**
            Element access
     */
    inline reference at(const MatrixPoint& point) { return m_data.at(point.row * m_columns + point.column); };
    constexpr inline const_reference at(const MatrixPoint& point) const { return m_data.at(point.row * m_columns + point.column); };
    inline reference at(std::size_t index) { return m_data.at(index); };
    constexpr inline const_reference at(std::size_t index) const { return m_data.at(index); };
    inline reference operator [] (const MatrixPoint& point) { return at(point); }
    constexpr inline const_reference operator [] (const MatrixPoint& point) const { return at(point); }
    inline reference operator [] (std::size_t index) { return at(index); }
    constexpr inline const_reference operator [] (std::size_t index) const { return at(index); }
    
    /**
            Iterator
     */
    constexpr inline Iterator begin() { return Iterator::begin(this); }
    constexpr inline Iterator beginAt(const MatrixPoint& point) { return Iterator::beginAt(this, point.row, point.column); }
    constexpr inline Iterator beginAt(std::size_t row, std::size_t column) { return Iterator::beginAt(this, row, column); }
    constexpr inline Iterator end() { return Iterator::end(this); }
    constexpr inline ConstIterator begin() const { return ConstIterator::begin(this); }
    constexpr inline ConstIterator beginAt(const MatrixPoint& point) const { return ConstIterator::beginAt(this, point.row, point.column); }
    constexpr inline ConstIterator beginAt(std::size_t row, std::size_t column) const { return ConstIterator::beginAt(this, row, column); }
    constexpr inline ConstIterator end() const { return ConstIterator::end(this); }
    
    /**
            Capacity
     */
    constexpr inline std::size_t size()  const noexcept { return m_data.size(); }
    constexpr inline std::size_t rows() const noexcept { return m_rows; }
    constexpr inline std::size_t columns() const noexcept { return m_columns; }
    inline std::size_t capacity() const noexcept { return m_data.capacity(); }
    constexpr inline void shrink_to_fit() { m_data.shrink_to_fit(); }
    constexpr inline bool isEmpty() const { return size() == 0; }
    
    /**
            Helpers
     */
    constexpr inline bool isSquareMatrix() const { return columns() == rows() && columns() != 0; }
    constexpr inline bool isColumnVector() const { return columns() == 1 && rows() > 0;}
    constexpr inline bool isRowVector() const { return rows() == 1 && columns() > 0; }
    constexpr inline bool isVector() const { return isColumnVector() || isRowVector(); }
    
    /**
            Modifiers
     */
    void resize(std::size_t rows, std::size_t columns, const T& val = T());
    void reset();
    inline void clear() { assign(); }
    void removeColumn(size_t column, size_t count = 1);
    void removeRow(size_t row, size_t count = 1);
    
private:
    std::vector<value_type> m_data;
    std::size_t m_rows = 0;
    std::size_t m_columns = 0;
};

template <typename T>
void Matrix<T>::resize(std::size_t rows, std::size_t columns, const T& val)
{
    m_columns = columns;
    m_rows = rows;
    m_data.resize(rows * columns, val);
}

template <typename T>
void Matrix<T>::reset()
{
    m_columns = 0;
    m_rows = 0;
    m_data.clear();
}

template <typename T>
void Matrix<T>::removeColumn(size_t column, size_t count)
{
    assert(column + count <= m_columns);
    
    if(count >= m_columns)
    {
        reset();
        return;
    }
    
    m_columns -= count;
    const size_t newSize = m_columns * m_rows;
    const size_t end = newSize - column;
    
    auto dstIt = std::next(m_data.begin(), column);
    auto srcIt = dstIt;
    
    for(size_t i = 0 ; i < end; ++i, ++dstIt)
    {
        const size_t shift = count + (i)/m_columns * count;
        srcIt = std::next(dstIt, shift);
        
        *dstIt = std::move(*srcIt);
    }
    
    m_data.resize(newSize);
}

template <typename T>
void Matrix<T>::removeRow(size_t row, size_t count)
{
    assert(row + count <= m_rows);
    
    const size_t size = m_data.size();
    if(count >= m_rows)
    {
        reset();
        return;
    }
    
    auto start = m_data.begin() + row * m_columns;
    auto end = std::next(start, count * m_columns);
    
    m_rows -= count;
    m_data.erase(start, end);
}
    
constexpr size_t factorial(size_t n, size_t res = 1)
{
    for(;n != 0; --n) res *= n;
    return res;
}
    
} //namespace matrix
} //namespace cppmath
#endif /* emath_matrix_hpp */
