#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


template < typename T >
class Array {
public:
    explicit Array ( size_t size )
    {
        m_Size = size;
        m_Data = new T [ size ];
    }
    Array ( )
            : m_Data ( nullptr ), m_Size ( 0 )
    {
    }
    Array ( const Array<T> & src )
    {
        m_Data = new T [ m_Size = src . size() ];
        for ( size_t i = 0; i < m_Size; i ++ )
            m_Data [ i ] = src . m_Data [ i ];
    }
    ~Array ( )
    {
        delete [] m_Data;
    }
    size_t size ( ) const
    {
        return m_Size;
    }
    T & operator [] ( int idx )
    {
        if ( idx >= m_Size )
            throw out_of_range ( "" );
        return m_Data [ idx ];
    }
    Array<T> & operator = ( const Array < T > & src )
    {
        if ( & src == this )
            return * this;
        delete [] m_Data;
        m_Data = new T [ m_Size = src . m_Size ];
        for ( size_t i = 0; i < m_Size; i ++ )
            m_Data [ i ] = src . m_Data [ i ];
        return * this;
    }
private:
    T * m_Data;
    size_t m_Size;
};

template <typename T>
class CMatrix3
{
public:

    // constructor ( unsigned n1, unsigned n2, unsigned n3 )
    CMatrix3 ( unsigned n1, unsigned n2, unsigned n3 )
            : mData3D ( n1 )
    {
        for ( size_t i = 0; i < n1; ++ i )
        {
            mData3D [ i ] = Array < Array < T > > ( n2 );
            for ( size_t j = 0; j < n2; ++j  )
                mData3D [ i ] [ j ] = Array < T > ( n3 );
        }
    }
    // destructor
    ~ CMatrix3() = default;

    // operator [][][]
    Array < Array < T > > & operator [] ( unsigned id )
    {
        return mData3D [ id ];
    }
private:
    Array < Array < Array < T > > > mData3D;
};

#ifndef __PROGTEST__
int main ( void )
{

    CMatrix3<int> m ( 3, 4, 5 );
    m[0][0][0] = 128;
    m[2][3][1] = 7;
    assert ( m[0][0][0] == 128 );
    assert ( m[2][3][1] == 7 );

    m[1][1][1] = m[0][0][0] + m[2][3][1];
    m[2][3][4] = 135;
    assert ( m[1][1][1] == m[2][3][4] );

    CMatrix3<int> copy ( m );
    try { copy[4][1][2]; assert ( "Missing exception for access i0 = 4, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[1][4][2]; assert ( "Missing exception for access i1 = 4, limit = 3" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[2][1][6]; assert ( "Missing exception for access i2 = 6, limit = 4" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }

    copy[2][3][1] = 42;
    assert ( m[2][3][1] == 7 );
    assert ( copy[2][3][1] == 42 );

    CMatrix3<int> n ( 2, 2, 2 );
    n[0][1][0] = 56;
    n[1][0][1] = 42;
    m = n;
    n[0][1][0] = 0;
    assert ( m[0][1][0] == 56 );
    try { m[2][1][0]; assert ( "Missing exception for access i0 = 2, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }

    CMatrix3<string> s ( 2, 1, 1 );
    s[0][0][0] = "Hello ";
    s[1][0][0] = "World!";
    assert ( ( s[0][0][0] + s[1][0][0] ) == string("Hello World!") );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
