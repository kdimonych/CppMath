#include <iostream>
#include <algorithm>

#include "src/cppmath_matrix.hpp"

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
    
    m.assign(val);
    
    for(i = 0; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], val);
    }
    
    m.clear();
    for(i = 0; i < m.size(); ++i) {
        ASSERT_EQUAL(m[i], 0);
    }
}

void testMatrixInit(){
    {
        int i = 0;
        Matrix<int> m(2,3, {0,1,2,3,4,5});
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 2);
        ASSERT_EQUAL(m.columns(), 3);
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], i);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {5,4,3}
        });
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 2);
        ASSERT_EQUAL(m.columns(), 3);

        int expect[6] = {0,1,2,5,4,3};
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
}

void testMatrixRemoveColumns(){
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeColumn(1);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 2);

        Matrix<int> expect({
            {0,2},
            {3,5},
            {6,8}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeColumn(1, 2);
        
        ASSERT_EQUAL(m.size(), 3);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 1);

        Matrix<int> expect({
            {0},
            {3},
            {6}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeColumn(2);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 2);

        Matrix<int> expect({
            {0,1},
            {3,4},
            {6,7}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeColumn(0);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 2);

        Matrix<int> expect({
            {1,2},
            {4,5},
            {7,8}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2, 3},
            {4,5,6, 7},
            {8,9,10,11}
        });
        
        ASSERT_EQUAL(m.size(), 12);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 4);
        
        m.removeColumn(1,2);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 2);

        Matrix<int> expect({
            {0,3},
            {4,7},
            {8,11}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
        
        m.removeColumn(0,2);
        
        ASSERT_EQUAL(m.size(), 0);
        ASSERT_EQUAL(m.rows(), 0);
        ASSERT_EQUAL(m.columns(), 0);
    }
}

void testMatrixRemoveRows(){
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeRow(1);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 2);
        ASSERT_EQUAL(m.columns(), 3);
        
        Matrix<int> expect({
            {0,1,2},
            {6,7,8}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeRow(1, 2);
        
        ASSERT_EQUAL(m.size(), 3);
        ASSERT_EQUAL(m.rows(), 1);
        ASSERT_EQUAL(m.columns(), 3);
        
        Matrix<int> expect({
            {0,1,2}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeRow(2);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 2);
        ASSERT_EQUAL(m.columns(), 3);
        
        Matrix<int> expect({
            {0,1,2},
            {3,4,5}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1,2},
            {3,4,5},
            {6,7,8}
        });
        
        ASSERT_EQUAL(m.size(), 9);
        ASSERT_EQUAL(m.rows(), 3);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeRow(0);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 2);
        ASSERT_EQUAL(m.columns(), 3);
        
        Matrix<int> expect({
            {3,4,5},
            {6,7,8}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
    }
    
    {
        int i = 0;
        Matrix<int> m({
            {0,1, 2},
            {3,4, 5},
            {6,7, 8},
            {9,10,11}
        });
        
        ASSERT_EQUAL(m.size(), 12);
        ASSERT_EQUAL(m.rows(), 4);
        ASSERT_EQUAL(m.columns(), 3);
        
        m.removeRow(1,2);
        
        ASSERT_EQUAL(m.size(), 6);
        ASSERT_EQUAL(m.rows(), 2);
        ASSERT_EQUAL(m.columns(), 3);
        
        Matrix<int> expect({
            {0,1, 2},
            {9,10,11}
        });
        
        for(i = 0; i < m.size(); ++i) {
            ASSERT_EQUAL(m[i], expect[i]);
        }
        
        m.removeRow(0,2);
        
        ASSERT_EQUAL(m.size(), 0);
        ASSERT_EQUAL(m.rows(), 0);
        ASSERT_EQUAL(m.columns(), 0);
    }
}

void testMatrix()
{
    testEmptyMatrix();
    testMatrixInit();
    testMatrixSetFunc();
    testMatrixResize();
    testZeroMatrix();
    testMatrixIndexes();
    testRaowIterator();
    testMatrixRemoveColumns();
    testMatrixRemoveRows();
}

int main(int a, char**)
{
    testMatrix();
    return 0;
}
