#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>

using namespace std;

class CLine{
public:
    CLine(unsigned cost, const vector<string>& stops){
        this->cost = cost;
        this->stops = stops;
    }
    unsigned cost;
    vector<string> stops;
};

class CPublicTransport{
public:

    CPublicTransport &addLine(unsigned cost, const vector<string> &stops){
        CLine c(cost, stops);
        lines.push_back(c);
        return *this;
    }

    CPublicTransport &optimize(){
        return *this;
    }


    unsigned findCheapest(const string &from, const string &to){
        createGraph();

        typedef pair<string, int> Pair;

        auto compare = [](const Pair& l , const Pair& r){
            return l.second > r.second;
        };

        priority_queue<Pair, std::vector<Pair>, decltype(compare)> pq(compare);

        map<string, pair<string, int>> prev;


        for (const auto &item: graph[from]){
            pq.emplace(item.first, item.second);
            prev[item.first] = {item.first, item.second};
        }

        while(!pq.empty()){
            auto curr = pq.top();
            pq.pop();

            if(curr.first == to){
                cout << prev[curr.first].second << endl;
                return prev[curr.first].second;
            }
            for (const auto &item: graph[curr.first]){
                if(prev.count(item.first) != 0){
                    continue;
                }
                pq.emplace(item.first, item.second+ curr.second);
                prev[item.first] = {item.first, item.second + curr.second};
            }


        }

        throw logic_error("chybicka se vbloudila");

    }

private:
    void createGraph(){
        for (const auto &line: lines){
            for (const auto &stop: line.stops){
                for (const auto &travel: line.stops){
                    if(travel == stop){
                        graph[stop].emplace(make_pair(travel, 0));
                    }
                    else{
                        graph[stop].emplace(make_pair(travel, line.cost));
                    }
                }
            }
        }
    }
    vector<CLine> lines;
    map<string, set<pair<string,unsigned >>> graph;
};

int main(){

    CPublicTransport t;

    t.addLine(3, {"Hlavni Nadrazi", "Orionka", "Baterie", "Petriny", "Jiriho z Podebrad"})
            .addLine(2, {"Kamenicka", "Letenske namesti", "Baterie", "Petriny", "Namesti Miru", "Malovanka"})
            .addLine(5, {"Dejvicka", "Muzeum"})
            .addLine(1, {"Staromestska", "Muzeum", "Mustek", "Namesti Miru", "Jiriho z Podebrad"})
            .optimize();

    assert(t.findCheapest("Orionka", "Kamenicka") == 5);
    assert(t.findCheapest("Staromestska", "Staromestska") == 0);
    assert(t.findCheapest("Staromestska", "Namesti Miru") == 1);
    assert(t.findCheapest("Staromestska", "Baterie") == 3);
    assert(t.findCheapest("Staromestska", "Hlavni Nadrazi") == 4);
    assert(t.findCheapest("Jiriho z Podebrad", "Namesti Miru") == 1);
    assert(t.findCheapest("Dejvicka", "Letenske namesti") == 8);


    t.addLine(6, {"Vitezne namesti", "I P Pavlova", "Andel"});
    t.addLine(9, {"Letenske namesti", "Vitezne namesti"});

    assert(t.findCheapest("Dejvicka", "Andel") == 23);
    assert(t.findCheapest("Andel", "Dejvicka") == 23);
    assert(t.findCheapest("Vitezne namesti", "Letenske namesti") == 9);
    assert(t.findCheapest("Orionka", "Andel") == 20);

    t.addLine(9, {"Terminal 1", "Terminal 2", "Terminal 3", "Nadrazi Veleslavin"});

    try{
        t.findCheapest("Namesti Miru", "Terminal 2");
    }
    catch (const logic_error &e){
        //assert(e.what() != "");
        cout << "chyceno" << endl;
    }
}
