#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CSparseMatrix
{
public:

    CSparseMatrix(unsigned row, unsigned col){
        _row = row;
        _col = col;
    }
        // destructor, operator =, copy constructor if needed
    T_ & operator() ( unsigned row, unsigned col ){
        if(row >= _row || col >= _col){
            throw out_of_range("Index error");
        }
        pair<int,int> c = {row,col};
        auto it = std::lower_bound(coords.begin(), coords.end(),c);

        if(it != coords.end() && it->first == row && it->second == col){
            return data[it-coords.begin()];
        }

        int index = it-coords.begin();
        coords.insert(it, c);
        data.insert(data.begin() + index,T_());
        return data[index];
    }
    bool contains ( unsigned row, unsigned col ){
        pair<int,int> c = {row,col};
        auto it = std::lower_bound(coords.begin(), coords.end(),c);
        return it != coords.end();
    }
private:
    unsigned _row;
    unsigned _col;
    vector<T_> data;
    vector<pair<int,int>> coords;
};

#ifndef __PROGTEST__
int main ( void )
{
    CSparseMatrix<int> m1 ( 300, 700 );

    assert ( !m1 . contains ( 0, 0 ) );
    m1 ( 0, 0 ) = 123;
    m1 ( 1, 1 ) = 321;
    m1 ( 2, 2 ) = m1 ( 1, 1 ) + m1 ( 0, 0 );
    assert ( m1 . contains ( 0, 0 ) && m1 ( 0, 0 ) == 123 );
    assert ( m1 . contains ( 1, 1 ) && m1 ( 1, 1 ) == 321 );
    assert ( m1 . contains ( 2, 2 ) && m1 ( 2, 2 ) == 444 );

    for ( int i = 0; i < 300; ++i )
        for ( int j = 0; j < 300; ++j )
            m1 ( i, j ) = i * j;

    for ( int i = 0; i < 300; ++i )
        for ( int j = 0; j < 300; ++j )
            assert ( m1 . contains ( i, j ) && m1 ( i, j ) == i * j );

    try
    {
        m1 ( 300, 300 ) = 666;
        assert ( false );
    }
    catch ( out_of_range & o )
    {
        assert ( o . what() == "Index error"s );
    }
    catch ( ... )
    {
        assert ( false );
    }

    try
    {
        m1 ( 299, 300 ) = 322;
    }
    catch ( ... )
    {
        assert ( false );
    }

    CSparseMatrix m2 = m1;

    for ( int i = 0; i < 300; ++i )
        for ( int j = 0; j < 300; ++j )
            assert ( m2 . contains( i, j ) && m2 ( i, j ) == i * j );
    m1 ( 0, 0 ) = 15;

    assert ( m2 ( 0, 0 ) == 0 );
    assert ( m1 ( 0, 0 ) == 15 );

    CSparseMatrix <int> m3 ( 1000, 1000 );
    assert ( !m3 . contains ( 666, 666 ) );
    m3 ( 666, 666 ) = 666;
    assert ( m3 . contains ( 666, 666 ) && m3 ( 666, 666 ) == 666 );
    m3 = m1;
    assert ( !m3 . contains ( 666, 666 ) );
    assert ( m3 . contains ( 0, 0 ) && m3 ( 0, 0 ) == 15 );

    for ( int i = 1; i < 300; ++i )
        for ( int j = 1; j < 300; ++j )
            assert ( m3 . contains( i, j ) && m3 ( i, j ) == i * j );

    CSparseMatrix <string> m4 ( 1000000, 1000000 );
    assert ( !m4 . contains ( 0, 0 ) );
    assert ( !m4 . contains ( 999999, 999999 ) );
    m4 ( 0, 0 ) = "P";
    m4 ( 10, 40 ) = "r";
    m4 ( 55, 50000 ) = "o";
    m4 ( 400000, 11 ) = "g";
    m4 ( 999999, 1221 ) = "t";
    m4 ( 121, 169 ) = "e";
    m4 ( 66, 6 ) = "s";
    m4 ( 6, 66 ) = "t";
    assert ( m4 . contains ( 0, 0 ) && m4 ( 0, 0 ) == "P"s );
    assert ( m4 . contains ( 10, 40 ) && m4 ( 10, 40 ) == "r"s );
    assert ( m4 . contains ( 55, 50000 ) && m4 ( 55, 50000 ) == "o"s );
    assert ( m4 . contains ( 400000, 11 ) && m4 ( 400000, 11 ) == "g"s );
    assert ( m4 . contains ( 999999, 1221 ) && m4 ( 999999, 1221 ) == "t"s );
    assert ( m4 . contains ( 121, 169 ) && m4 ( 121, 169 ) == "e"s );
    assert ( m4 . contains ( 66, 6 ) && m4 ( 66, 6 ) == "s"s );
    assert ( m4 . contains ( 6, 66 ) && m4 ( 6, 66 ) == "t"s );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
