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

    // add connection from -> to, fluent interface
    CDelivery & addConn ( const string & from, const string & to );

    // returns a map containing shortest path to each customer from any of the depots
    // map key is the customer name, value is a list of nodes on the shortest path (depot -> customer)
    // if there is no path, the value is an empty list
    map<string, list<string>> serveCustomers ( const set<string> & customers, const set<string> & depots ) const;

private:
    map<string, set<string>> graph;
};

CDelivery & CDelivery::addConn ( const string & from, const string & to )
{
    graph[from].insert(to);
    return *this;
}

map<string, list<string>> CDelivery::serveCustomers ( const set<string> & customers, const set<string> & depots ) const
{
    map<string, list<string>> res;

    queue<string> q;

    map<string, string> prev;


    for (const auto &depot: depots){
        q.push(depot);
        prev[depot] = depot;
    }


    while(!q.empty()){
        auto current = q.front();
        q.pop();
        if(graph.count(current) == 0){
            continue;
        }
        for (const auto &neighbour: graph.at(current)){
            if(prev.count(neighbour) == 0){
                prev[neighbour] = current;
                q.push(neighbour);
            }
        }
    }


    for (const auto &customer: customers){

        if(prev.count(customer) != 0){
            string prevPath = prev[customer];

            if(prevPath != customer){
                res[customer].push_front(customer);
            }

            while(depots.count(prevPath) == 0){
                res[customer].push_front(prevPath);
                prevPath = prev[prevPath];
            }

            res[customer].push_front(prevPath);
        }
        else{
            res[customer];
        }
    }




    return res;
}

int main ()
{
    CDelivery t0;
    map<string, list<string>> r;
    t0.addConn("Austin", "Berlin");
    t0.addConn("Chicago", "Berlin");
    t0.addConn("Berlin", "Dallas");
    t0.addConn("Dallas", "Essen");
    t0.addConn("Essen", "Austin");
    t0.addConn("Frankfurt", "Gyor");
    t0.addConn("Gyor", "Helsinki");
    t0.addConn("Helsinki", "Frankfurt");

    r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});
    assert ( r == (map<string, list<string>>{ {"Berlin", {"Essen", "Austin", "Berlin"}}, {"Gyor", {"Helsinki", "Frankfurt", "Gyor"}} }) );

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
    t0.addConn("Chicago", "Helsinki");

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
    t0.addConn("Berlin", "Chicago");

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}} }) );
    t0.addConn("Essen", "Frankfurt");

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Dallas", "Essen", "Frankfurt", "Gyor"}} }) );

    return EXIT_SUCCESS;
}
