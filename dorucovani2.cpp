#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

class CDelivery
{
public:
	// ctor
	// dtor
	CDelivery & addConn ( const string & a, const string & b );
	vector<int> findCoverage ( const set<string> & depots ) const;

private:
    map<string, set<string>> graph;
};

CDelivery & CDelivery::addConn ( const string & a, const string & b )
{
    graph[a].insert(b);
    graph[b].insert(a);
    return *this;
}

vector<int> CDelivery::findCoverage ( const set<string> & depots ) const
{
    vector<int> result;


    int remaining= 0;
    int remainingNext = depots.size();

    queue<string> q;
    set<string> visited;


    for (const auto &item: depots){
        if(graph.count(item) == 0){
            throw invalid_argument("");
        }

        q.push(item);
        visited.insert(item);
    }


    while(!q.empty()){
        if(remaining == 0){
            if(result.empty()){
                result.push_back(remainingNext);
            }
            else{
                result.push_back(result.back() + remainingNext);
            }

            remaining = remainingNext;
            remainingNext =0;
        }


        for (const auto &item: graph.at(q.front())){
            if(visited.find(item) != visited.end()){
                continue;
            }

            q.push(item);
            visited.insert(item);

            remainingNext++;
        }
        remaining--;

        q.pop();


    }



    return result;
}

int main ()
{
	CDelivery t0;
	vector<int> r;
	t0
	.addConn("Austin", "Berlin")
	.addConn("Chicago", "Berlin")
	.addConn("Chicago", "Dallas")
	.addConn("Dallas", "Essen")
	.addConn("Essen", "Austin")
	.addConn("Frankfurt", "Essen")
	.addConn("Gyor", "Frankfurt")
	.addConn("Helsinki", "Istanbul")
	.addConn("Istanbul", "Jakarta");

	r = t0.findCoverage(set<string>{"Berlin"});
	assert ( r == (vector<int>{1, 3, 5, 6, 7}) );
	/* result:
	 * [0]: Berlin = 1
	 * [1]: Austin, Berlin, Chicago = 3
	 * [2]: Austin, Berlin, Chicago, Dallas, Essen = 5
	 * [3]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt = 6
	 * [4]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt, Gyor = 7
	 */

	r = t0.findCoverage(set<string>{"Berlin", "Essen"});
	assert ( r == (vector<int>{2, 6, 7}) );

	r = t0.findCoverage(set<string>{"Helsinki"});
	assert ( r == (vector<int>{1, 2, 3}) );

	r = t0.findCoverage(set<string>{"Istanbul"});
	assert ( r == (vector<int>{1, 3}) );

	r = t0.findCoverage(set<string>{"Austin", "Jakarta"});
	assert ( r == (vector<int>{2, 5, 9, 10}) );

	r = t0.findCoverage(set<string>{"Chicago", "Gyor", "Helsinki", "Jakarta"});
	assert ( r == (vector<int>{4, 8, 10}) );

	try
	{
		r = t0.findCoverage(set<string>{"Incorrect city"});
		assert ( "No invalid_argument exception caught!" == nullptr );
	}
	catch ( const invalid_argument & e ) {}

	return EXIT_SUCCESS;
}
