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

class MatrixBaseIterationStrategy{
public:
    MatrixBaseIterationStrategy() = default;
    MatrixBaseIterationStrategy(const MatrixBaseIterationStrategy&) = default;
    MatrixBaseIterationStrategy(MatrixBaseIterationStrategy&&) = default;
    
    MatrixBaseIterationStrategy& operator = (const MatrixBaseIterationStrategy& other) = default;
    MatrixBaseIterationStrategy& operator = (MatrixBaseIterationStrategy&& other) = default;
    
    inline const IMatrix* matrix() const {return m_matrix;};
    
    template<class MatrixT>
    constexpr inline MatrixT* matrix() const {return reinterpret_cast<MatrixT*>(const_cast<IMatrix*>(m_matrix));};
    
protected:
    MatrixBaseIterationStrategy(const IMatrix* matrix): m_matrix(matrix) {};
    const IMatrix* m_matrix = nullptr;
};

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

template <class MatrixT> struct MatrixTrait {
    typedef typename std::decay<typename MatrixT::value_type>::type value_type;
};

template <class MatrixT> struct MatrixTrait<const MatrixT> {
    typedef const typename std::decay<typename MatrixT::value_type>::type value_type;
};

template <
    class MatrixT,
    template <typename DifferenceType> class Strategy >
class MatrixBaseIterator{
public:
    
    typedef typename MatrixTrait<MatrixT>::value_type                   value_type;
    typedef value_type&                                                 reference;
    typedef value_type*                                                 pointer;
    typedef std::ptrdiff_t                                              difference_type;
    typedef std::random_access_iterator_tag                             iterator_category;
    
    using StrategyT = Strategy<difference_type>;
    //template <class _MatrixT> friend class MatrixBaseIterator;
    
    MatrixBaseIterator() = default;
    MatrixBaseIterator(const MatrixBaseIterator&) =  default;
    MatrixBaseIterator(MatrixBaseIterator&&) =  default;
    
    template <class _MatrixT>
    constexpr MatrixBaseIterator(const MatrixBaseIterator<_MatrixT, Strategy>& other,
        typename std::enable_if<std::is_convertible<_MatrixT, MatrixT>::value>::type* = 0)
    : m_strategy(other.m_strategy){}
    
    MatrixBaseIterator& operator = (const MatrixBaseIterator& other) = default;
    MatrixBaseIterator& operator = (MatrixBaseIterator&& other) = default;
    
//    template <class _MatrixT>
//    MatrixBaseIterator& operator = (const MatrixBaseIterator<_MatrixT, Strategy>& other,
//                                 typename std::enable_if<std::is_convertible<_MatrixT, MatrixT>::value>::type* = 0)
//    : m_strategy(other.m_strategy){}
    
    constexpr static inline MatrixBaseIterator begin(MatrixT* matrix){
        return MatrixBaseIterator(StrategyT::begin(matrix));
    }
    
    constexpr static inline MatrixBaseIterator beginAt(MatrixT* matrix, std::size_t row, std::size_t column){
        return MatrixBaseIterator(StrategyT::begin(matrix, row, column));
    }
    
    constexpr static inline MatrixBaseIterator end(MatrixT* matrix){
        return MatrixBaseIterator(StrategyT::end(matrix));
    }
    
    inline MatrixBaseIterator& operator ++()
    {
        m_strategy.inc();
        return *this;
    }
    
    inline MatrixBaseIterator operator ++(int) {
        MatrixBaseIterator result(*this);
        m_strategy.inc();
        return result;
    }
    
    inline MatrixBaseIterator& operator --(){
        m_strategy.dec();
        return *this;
    }
    
    inline MatrixBaseIterator operator --(int)
    {
        MatrixBaseIterator result(*this);
        m_strategy.dec();
        return result;
    }
    
    MatrixBaseIterator& operator +=(difference_type di){
        m_strategy.inc(di);
        return *this;
    }
    
    MatrixBaseIterator& operator -=(difference_type di){
        m_strategy.dec(di);
        return *this;
    }
    
    inline MatrixBaseIterator operator - (difference_type index) const
    {
        MatrixBaseIterator result(*this);
        result -= index;
        return result;
    }
    
    inline MatrixBaseIterator operator + (difference_type index) const
    {
        MatrixBaseIterator result(*this);
        result += index;
        return result;
    }
    
    inline bool operator == (const MatrixBaseIterator& other) const{return m_strategy.diff(other.m_strategy) == 0;}
    inline bool operator < (const MatrixBaseIterator& other) const {return m_strategy.diff(other.m_strategy) < 0;}
    inline bool operator > (const MatrixBaseIterator& other) const{return m_strategy.diff(other.m_strategy) > 0;}
    inline bool operator != (const MatrixBaseIterator& other) const {return !operator== (other);}
    inline bool operator >= (const MatrixBaseIterator& other) const {return operator > (other) || operator == (other);}
    inline bool operator <= (const MatrixBaseIterator& other) const {return operator < (other) || operator == (other);}
    
    pointer operator ->() const {return &data();}
    reference operator *() const {return data();}
    
protected:
    
    MatrixBaseIterator(StrategyT&& strategy)
    : m_strategy(std::move(strategy)){}
    
    MatrixBaseIterator(const StrategyT& strategy)
    :m_strategy(strategy){}
    
    //constexpr inline void dec(difference_type index = 1){ m_strategy.dec(index); };
    //constexpr inline void inc(difference_type index = 1){ m_strategy.inc(index); };
    constexpr inline value_type& data() const {
        return (*m_strategy.template matrix<MatrixT>())[m_strategy.index()];
    };
    //constexpr inline difference_type diff(const MatrixBaseIterator& other) const {return m_strategy.diff(other.m_strategy); }
    
    StrategyT m_strategy;
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
