#include <string>
#include <cassert>
#include <iostream>

using namespace std;

int bar ( int a )
{
    return 2 * a;
}
int foo ( const int & a )
{
    return bar ( a + 2 );
}
int main ( void )
{
    int x = foo ( 63 );
    cout << x;
    return 0;
}

