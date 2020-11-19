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
#include <type_traits>
#include <iterator>
#include <cassert>

namespace cppmath {
namespace matrix{
struct MatrixPoint{
    std::size_t row = 0;
    std::size_t column = 0;
};

class IMatrix{
public:
    virtual ~IMatrix() = default;
    virtual std::size_t size() const = 0;
    virtual std::size_t rows() const  = 0;
    virtual std::size_t columns() const = 0;

    constexpr inline bool isSquareMatrix() const {return columns() == rows() && columns() != 0; }
    constexpr inline bool isColumnVector() const {return columns() == 1 && rows() > 0;}
    constexpr inline bool isRowVector() const {return rows() == 1 && columns() > 0;}
    constexpr inline bool isVector() const {return isColumnVector() || isRowVector();}
    constexpr inline bool isEmpty() const {return size() == 0;}
};

class BaseIterationStrategy{
public:
    BaseIterationStrategy() = default;
    BaseIterationStrategy(const BaseIterationStrategy&) = default;
    BaseIterationStrategy(BaseIterationStrategy&&) = default;
    
    BaseIterationStrategy& operator = (const BaseIterationStrategy& other) = default;
    BaseIterationStrategy& operator = (BaseIterationStrategy&& other) = default;
    
    inline const IMatrix* matrix() const {return m_matrix;};
    
    template<class MatrixT>
    constexpr inline MatrixT* matrix() const {
        return reinterpret_cast<MatrixT*>(const_cast<IMatrix*>(m_matrix));
    };
    
    constexpr inline bool isValid() const { return m_matrix == nullptr; }
    
protected:
    BaseIterationStrategy(const IMatrix* matrix): m_matrix(matrix) {};
    const IMatrix* m_matrix = nullptr;
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
    
    inline difference_type operator - (const MatrixBaseIterator& other) const
    {
        return m_strategy.diff(other.m_strategy);
    }
    
    inline bool operator == (const MatrixBaseIterator& other) const{return m_strategy.diff(other.m_strategy) == 0;}
    inline bool operator < (const MatrixBaseIterator& other) const {return m_strategy.diff(other.m_strategy) < 0;}
    inline bool operator > (const MatrixBaseIterator& other) const{return m_strategy.diff(other.m_strategy) > 0;}
    inline bool operator != (const MatrixBaseIterator& other) const {return !operator== (other);}
    inline bool operator >= (const MatrixBaseIterator& other) const {return operator > (other) || operator == (other);}
    inline bool operator <= (const MatrixBaseIterator& other) const {return operator < (other) || operator == (other);}
    
    pointer operator ->() const {return &data();}
    reference operator *() const {return data();}
    constexpr inline bool isValid() const { return m_strategy.isValid(); }
    
    inline size_t row() const { return m_strategy.row(); }
    inline size_t column() const { return m_strategy.column(); }
    
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

} //namespace matrix
} //namespace cppmath

#endif /* cppmath_matrix_base_hpp */
