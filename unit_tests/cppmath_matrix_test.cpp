#include <iostream>
#include <algorithm>

#include "cppmath_matrix.hpp"

#define ASSERT_THROW( condition )                                   \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define ASSERT_EQUAL( x, y )                                        \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( x ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( y ) )             \
                              + std::string( " in expr.: " )        \
                              + std::string( #x )                   \
                              + std::string( " != " )               \
                              + std::string( #y )                   \
    );                                                              \
  }                                                                 \
}

#define ASSERT_EQUAL_EXP( x, y )                                    \
{                                                                   \
if( ( x ) != ( y ) )                                                \
{                                                                   \
throw std::runtime_error(   std::string( __FILE__ )                 \
                            + std::string( ":" )                    \
                            + std::to_string( __LINE__ )            \
                            + std::string( " in " )                 \
                            + std::string( __PRETTY_FUNCTION__ )    \
                            + std::string( ": " )                   \
                            + std::string( #x )                     \
                            + std::string( " != " )                 \
                            + std::string( #y )                     \
);                                                                  \
}                                                                   \
}
using namespace cppmath::matrix;

void testRaowIterator(){
    
}

void testEmptyMatrix(){
    Matrix<int> m;
    //Basic test
    ASSERT_EQUAL(m.size(), 0);
    ASSERT_EQUAL(m.rows(), 0);
    ASSERT_EQUAL(m.columns(), 0);
    ASSERT_EQUAL(m.isSquareMatrix(), false);
    ASSERT_EQUAL(m.isColumnVector(), false);
    ASSERT_EQUAL(m.isRowVector(), false);
    ASSERT_EQUAL(m.isVector(), false);
    ASSERT_EQUAL(m.isEmpty(), true);
    ASSERT_EQUAL_EXP(m.begin(), m.end());
    
    ASSERT_EQUAL(m.begin().column(), 0);
    ASSERT_EQUAL(m.begin().row(), 0);
    ASSERT_EQUAL(m.end().column(), 0);
    ASSERT_EQUAL(m.end().row(), 0);
}

void testMatrixResize(){
    int i = 0;

    Matrix<int> m;
    
    m.resize(1, 3);
    
    ASSERT_EQUAL(m.size(), 3);
    ASSERT_EQUAL(m.rows(), 1);
    ASSERT_EQUAL(m.columns(), 3);
    ASSERT_EQUAL(m.isSquareMatrix(), false);
    ASSERT_EQUAL(m.isColumnVector(), false);
    ASSERT_EQUAL(m.isRowVector(), true);
    ASSERT_EQUAL(m.isVector(), true);
    ASSERT_EQUAL(m.isEmpty(), false);
    
    for(int i = 0; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], 0);
    }
    
    const auto oldSize = m.size();
    auto defaultValue = 6;
    
    m.resize(4, 1, defaultValue);
    
    ASSERT_EQUAL(m.size(), 4);
    ASSERT_EQUAL(m.rows(), 4);
    ASSERT_EQUAL(m.columns(), 1);
    ASSERT_EQUAL(m.isSquareMatrix(), false);
    ASSERT_EQUAL(m.isColumnVector(), true);
    ASSERT_EQUAL(m.isRowVector(), false);
    ASSERT_EQUAL(m.isVector(), true);
    ASSERT_EQUAL(m.isEmpty(), false);

    for(i = 0; i < oldSize; ++i) {
        ASSERT_EQUAL(m[i], 0);
    }
    for(; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], defaultValue);
    }
    
    const auto oldSize2 = m.size();
    auto defaultValue2 = 7;
    m.resize(4, 4, defaultValue2);
    
    ASSERT_EQUAL(m.size(), 16);
    ASSERT_EQUAL(m.rows(), 4);
    ASSERT_EQUAL(m.columns(), 4);
    ASSERT_EQUAL(m.isSquareMatrix(), true);
    ASSERT_EQUAL(m.isColumnVector(), false);
    ASSERT_EQUAL(m.isRowVector(), false);
    ASSERT_EQUAL(m.isVector(), false);
    for(i = 0; i < oldSize; ++i) {
        ASSERT_EQUAL(m[i], 0);
    }
    for(; i < oldSize2; ++i) {
        ASSERT_EQUAL(m[i], defaultValue);
    }
    for(; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], defaultValue2);
    }
}

void testZeroMatrix(){
    int i = 0;
    Matrix<int> m(4, 4);
    
    ASSERT_EQUAL(m.size(), 16);
    ASSERT_EQUAL(m.rows(), 4);
    ASSERT_EQUAL(m.columns(), 4);
    
    for(i = 0; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], 0);
    }
}

void testMatrixIndexes(){
    int i = 0;
    Matrix<int> m(4, 4);
    
    ASSERT_EQUAL(m.size(), 16);
    ASSERT_EQUAL(m.rows(), 4);
    ASSERT_EQUAL(m.columns(), 4);
    
    auto it = m.begin();
    
    for(i = 0; i < m.size(); ++i, ++it) {
        const auto rowIndex = i / m.columns();
        const auto columnIndex = i % m.columns();
        
        MatrixPoint mp = {rowIndex, columnIndex};
        ASSERT_EQUAL(mp.row, rowIndex);
        ASSERT_EQUAL(mp.column, columnIndex);
        
        m[i] = i;
        
        ASSERT_EQUAL(m[i], m[mp]);
        ASSERT_EQUAL(i, m[mp]);
        
        ASSERT_EQUAL(std::distance(m.begin(), it), i);
        ASSERT_EQUAL(*it, i);
        ASSERT_EQUAL(it.row(), rowIndex);
        ASSERT_EQUAL(it.column(), columnIndex);
    }
}

void testMatrixSetFunc(){
    int i = 0;
    Matrix<int> m(4, 4);
    
    ASSERT_EQUAL(m.size(), 16);
    ASSERT_EQUAL(m.rows(), 4);
    ASSERT_EQUAL(m.columns(), 4);
    
    const auto val = 13;
    
    m.set(val);
    
    for(i = 0; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], val);
    }
    
    m.clear();
    for(i = 0; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], 0);
    }
}

void testMatrix()
{
    testEmptyMatrix();
    testMatrixSetFunc();
    testMatrixResize();
    testZeroMatrix();
    testMatrixIndexes();
    testRaowIterator();
}

int main(int a, char**)
{
    testMatrix();
    return 0;
}
