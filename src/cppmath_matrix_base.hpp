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
#include <utility>

namespace cppmath {
namespace matrix{
    
template <class MatrixT> struct MatrixTrait {
    typedef typename std::decay<typename MatrixT::value_type>::type value_type;
};

template <class MatrixT> struct MatrixTrait<const MatrixT> {
    typedef const typename std::decay<typename MatrixT::value_type>::type value_type;
};

template <class MatrixT, class Derived>
class MatrixBaseIterator{
public:
    
    typedef typename MatrixTrait<MatrixT>::value_type                   value_type;
    typedef value_type&                                                 reference;
    typedef value_type*                                                 pointer;
    typedef std::ptrdiff_t                                              difference_type;
    typedef std::random_access_iterator_tag                             iterator_category;
    
    MatrixBaseIterator() = default;
    MatrixBaseIterator(const MatrixBaseIterator&) =  default;
    MatrixBaseIterator(MatrixBaseIterator&&) =  default;
    
    MatrixBaseIterator& operator = (const MatrixBaseIterator& other) = default;
    MatrixBaseIterator& operator = (MatrixBaseIterator&& other) = default;
    
    inline Derived& operator ++() noexcept {
        _inc();
        return static_cast<Derived&>(*this);
    }
    
    inline Derived operator ++(int) noexcept {
        Derived result(static_cast<const Derived&>(*this));
        _inc();
        return result;
    }
    
    inline Derived& operator --() noexcept {
        _dec();
        return static_cast<Derived&>(*this);
    }
    
    inline Derived operator --(int) noexcept {
        Derived result(static_cast<const Derived&>(*this));
        _dec();
        return result;
    }
    
    Derived& operator +=(difference_type di) noexcept {
        _inc(di);
        return static_cast<Derived&>(*this);
    }
    
    Derived& operator -=(difference_type di) noexcept {
        _dec(di);
        return static_cast<Derived&>(*this);
    }
    
    inline Derived operator - (difference_type index) const noexcept {
        Derived result(static_cast<const Derived&>(*this));
        result -= index;
        return result;
    }
    
    inline Derived operator + (difference_type index) const noexcept {
        Derived result(static_cast<const Derived&>(*this));
        result += index;
        return result;
    }
    
    inline difference_type operator - (const Derived& other) const noexcept {
        return _diff(other);
    }
    
    inline bool operator == (const Derived& other) const{ return _diff(other) == 0; }
    inline bool operator < (const Derived& other) const { return _diff(other) < 0; }
    inline bool operator > (const Derived& other) const{ return _diff(other) > 0; }
    inline bool operator != (const Derived& other) const { return ! operator== (other); }
    inline bool operator >= (const Derived& other) const { return operator > (other) || operator == (other); }
    inline bool operator <= (const Derived& other) const { return operator < (other) || operator == (other); }
    
    pointer operator ->() const noexcept {return &data();}
    reference operator *() const {return data();}
    constexpr inline bool isValid() const { return m_matrix != nullptr; }
    
protected:
//    template <class _Iter1, class _Iter2>
//    _LIBCPP_CONSTEXPR_IF_NODEBUG friend
//    bool
//    operator<(const MatrixBaseIterator<_Iter1>&, const MatrixBaseIterator<_Iter2>&) _NOEXCEPT_DEBUG;
    
    MatrixBaseIterator(MatrixT* matrix)
    : m_matrix(matrix)
    {}

    constexpr inline value_type& data() const {
        assert(m_matrix != nullptr);
        assert(_index() < m_matrix->size());
        
        return m_matrix->at(_index());
    };
    
    /**
     Interface that must be implemented in derived classes:
     
     void inc(difference_type index) noexcept;
     void dec(difference_type index) noexcept;
     std::size_t index() const noexcept;
     std::size_t column() const noexcept;
     std::size_t row() const noexcept;
     sdifference_type diff(const Derived& other) const noexcept;
     */
    inline constexpr void _inc(difference_type index = 1) noexcept {
        return static_cast<Derived&>(*this).inc(index);
    }
    
    inline constexpr void _dec(difference_type index = 1) noexcept {
        return static_cast<Derived&>(*this).dec(index);
    }
    
    inline constexpr std::size_t _index() const noexcept {
        return static_cast<const Derived&>(*this).index();
    }
    
    inline constexpr std::size_t _column() const noexcept {
        return static_cast<const Derived&>(*this).column();
    }
    inline constexpr std::size_t _row() const noexcept {
        return static_cast<const Derived&>(*this).row();
    }
    
    inline constexpr difference_type _diff(const Derived& other) const noexcept {
        return static_cast<const Derived&>(*this).diff(other);
    }
    
    MatrixT* m_matrix = nullptr;
};

    template <class Iterator, class Derived>
    class IteratorBase: public Iterator
    {
    public:
        inline Derived& operator ++() noexcept {
            this->inc();
            return static_cast<Derived&>(*this);
        }
        
        inline Derived operator ++(int) noexcept {
            Derived result(static_cast<const Derived&>(*this));
            this->inc();
            return result;
        }
        
        typename Iterator::reference operator *() const { return this->data(); }
    };
    
    template <class Iterator, class Derived, typename IiteratorCategory>
    class IteratorAccessSelector: public IteratorBase < Iterator,  Derived >
    {};
    
    template <class Iterator, class Derived>
    class IteratorAccessSelector <Iterator, Derived, std::input_iterator_tag>:
    public IteratorBase <Iterator, Derived>
    {

    };
    
    template <class Iterator, class Derived>
    class IteratorAccessSelector <Iterator, Derived, std::forward_iterator_tag>:
        public IteratorAccessSelector <Iterator, Derived, std::input_iterator_tag>
    {
        
    };
    
    template <class Iterator, class Derived>
    class IteratorAccessSelector <Iterator, Derived, std::bidirectional_iterator_tag>:
        public IteratorAccessSelector <Iterator, Derived, std::forward_iterator_tag>
    {

    };
    
    template <class Iterator, class Derived>
    class IteratorAccessSelector <Iterator, Derived, std::random_access_iterator_tag>:
        public IteratorAccessSelector <Iterator, Derived, std::bidirectional_iterator_tag>
    {

    };
    
    template <class Iterator, class Derived>
    class IteratorAccessSelector <Iterator, Derived, std::output_iterator_tag>:
    public IteratorBase <Iterator, Derived>
    {

    };
    
    template <class Iterator>
    class MatrixIteratorWrap:
        public IteratorAccessSelector<
            Iterator,
            MatrixIteratorWrap < Iterator >,
            typename Iterator::iterator_category >
    {
    public:
//        typedef typename std::iterator_traits<Iterator>::value_type             value_type;
//        typedef typename std::iterator_traits<Iterator>::reference              reference;
//        typedef typename std::iterator_traits<Iterator>::pointer                pointer;
        //typedef typename std::iterator_traits<Iterator>::iterator_category      iterator_category;
        //typedef std::random_access_iterator_tag                                 iterator_category;
        typedef std::ptrdiff_t                                                  difference_type;
        
    protected:
//        inline constexpr void _inc(difference_type index = 1) noexcept {
//            return static_cast<Iterator&>(*this).inc(index);
//        }
//
//        inline constexpr void _dec(difference_type index = 1) noexcept {
//            return static_cast<Iterator&>(*this).dec(index);
//        }
//
//        inline constexpr std::size_t _index() const noexcept {
//            return static_cast<const Iterator&>(*this).index();
//        }
//
//        inline constexpr std::size_t _column() const noexcept {
//            return static_cast<const Iterator&>(*this).column();
//        }
//        inline constexpr std::size_t _row() const noexcept {
//            return static_cast<const Iterator&>(*this).row();
//        }
//
//        inline constexpr difference_type _diff(const Derived& other) const noexcept {
//            return static_cast<const Iterator&>(*this).diff(other);
//        }
    };
} //namespace matrix
} //namespace cppmath

#endif /* cppmath_matrix_base_hpp */
