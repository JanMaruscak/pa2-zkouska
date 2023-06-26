

#ifndef __PROGTEST__
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CDiagonalMatrix
{
public:
    CDiagonalMatrix(int n, int k) : _n(n), _k(k){
        if(k < 1 || k > n){
            throw out_of_range("invalid matrix size");
        }
        for (int i = 0; i < n; ++i) {
            middle.push_back(T_());

        }
        for (int j = 0; j < (k-1); ++j) {
            left.push_back({});
            right.push_back({});
            for (int i = 1; i < (_n-j); ++i) {
                left[j].push_back(T_());
                right[j].push_back(T_());
            }
        }
    }
    // destructor, copy cons, operator = (if needed)
    T_ & operator () (int row,int col ){
        if(!exists(row,col)){
            throw out_of_range("invalid index ( "+to_string(row) + ", "+to_string(col)+" )");
        }

        if(row - col == 0){
            return middle[row];
        }
        if(row - col > 0){
            int outer = row - col - 1;
            int inner = col;
            return left[outer][inner];
        }

        int outer = col - row - 1;
        auto inner =row;
        return right[outer][inner];

    }
    bool exists (int row, int col ) const{
        int outer = abs(row-col);
        if(outer >= _k){
            return false;
        }
        if(row == col && row <= _k){
            return true;
        }

        outer -=1;

        if(col > row){

            int inner = right[outer].size() - (_k-col);
            if(inner > right[outer].size()){
                return false;
            }
        }
        else{
            int inner =right[outer].size() -  (_k-row);
            if(inner > left[outer].size()){
                return false;
            }
        }


        return true;
    }
    void reshape (int k ){
        if(k > _n || k < 1){
            throw out_of_range("invalid matrix size");
        }

        if(k > _k){
            for (int j = _k-1; j < (k-1); ++j) {
                left.push_back({});
                right.push_back({});
                for (int i = 1; i < (_n-j); ++i) {
                    left[j].push_back(T_());
                    right[j].push_back(T_());
                }
            }
        }
        else if( k< _k){
            right.erase(right.begin() + k - 1, right.end());
            left.erase(left.begin() + k - 1, left.end());
        }


        _k=k;


    }

private:
    vector<vector<T_>> left;
    vector<T_> middle;
    vector<vector<T_>> right;

    int _n;
    int _k;
};

#ifndef __PROGTEST__
class CTestCnt
{
public:
    CTestCnt                                ( int                                   val = 0 )
            : m_Val ( val )
    {
        c_Inst ++;
    }
    CTestCnt                                ( const CTestCnt                      & src ) noexcept
            : m_Val ( src . m_Val )
    {
        c_Inst ++;
    }
    CTestCnt                                ( CTestCnt                           && src ) noexcept
            : m_Val ( src . m_Val )
    {
        c_Inst ++;
    }
    ~CTestCnt                               ( void ) noexcept
    {
        c_Inst --;
    }
    CTestCnt                         & operator =                              ( CTestCnt                              src ) noexcept
    {
        swap ( src . m_Val, m_Val );
        return *this;
    }
    inline static size_t               c_Inst                                  = 0;
private:
    int                                m_Val;
};

int main ( void )
{
    CDiagonalMatrix<int> m1 ( 5, 4 );

    for ( int i = 0; i < 5; i ++ )
        m1 ( i, i ) = 10 + i;
    for ( int i = 0; i < 4; i ++ )
    {
        m1 ( i, i + 1 ) = 20 + i;
        m1 ( i + 1, i ) = 30 + i;
    }
    for ( int i = 0; i < 3; i ++ )
    {
        m1 ( i, i + 2 ) = 40 + i;
        m1 ( i + 2, i ) = 50 + i;
    }
    m1 ( 0, 3 ) = 60;
    m1 ( 1, 4 ) = 61;
    m1 ( 3, 0 ) = 70;
    m1 ( 4, 1 ) = 71;

    CDiagonalMatrix<int> m2 ( m1 );

    for ( int i = 0; i < 5; i ++ )
        assert ( m2 ( i, i ) == 10 + i );
    for ( int i = 0; i < 4; i ++ )
    {
        assert ( m2 ( i, i + 1 ) == 20 + i );
        assert ( m2 ( i + 1, i ) == 30 + i );
    }
    for ( int i = 0; i < 3; i ++ )
    {
        assert ( m2 ( i, i + 2 ) == 40 + i );
        assert ( m2 ( i + 2, i ) == 50 + i );
    }
    assert ( m2 ( 0, 3 ) == 60 );
    assert ( m2 ( 1, 4 ) == 61 );
    assert ( m2 ( 3, 0 ) == 70 );
    assert ( m2 ( 4, 1 ) == 71 );


    m2 ( 0, 0 ) = 100;
    assert ( m1 ( 0, 0 ) == 10 );
    assert ( m2 ( 0, 0 ) == 100 );


    assert ( m1 . exists ( 4, 1 ) && m1 ( 4, 1 ) == 71 );
    m1 . reshape ( 2 );
    assert ( m1 . exists ( 0, 1 ) && m1 ( 0, 1 ) == 20 );
    assert ( ! m1 . exists ( 0, 2 ) );
    try
    {
        m1 ( 0, 0 ) = m1 ( 0, 2 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
        assert ( e . what () == "invalid index ( 0, 2 )"s );
    }


    m1 . reshape ( 4 );
    assert ( m1 . exists ( 0, 1 ) && m1 ( 0, 1 ) == 20 );
    assert ( m1 . exists ( 0, 2 ) && m1 ( 0, 2 ) == 0 );

    try
    {
        m1 . reshape ( 6 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
        assert ( e . what () == "invalid matrix size"s );
    }

    try
    {
        CDiagonalMatrix<int> m3 ( 5, 6 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
        assert ( e . what () == "invalid matrix size"s );
    }


    CDiagonalMatrix<CTestCnt>  m10 ( 12, 1 );
    assert ( CTestCnt::c_Inst == 12 );
    m10 . reshape ( 4 );
    assert ( CTestCnt::c_Inst == 72 );
    m10 . reshape ( 2 );
    assert ( CTestCnt::c_Inst == 34 );
    m10 . reshape ( 11 );
    assert ( CTestCnt::c_Inst == 142 );
    m10 . reshape ( 12 );
    assert ( CTestCnt::c_Inst == 144 );
    m10 . reshape ( 8 );
    assert ( CTestCnt::c_Inst == 124 );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
