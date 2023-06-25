#ifndef __PROGTEST__
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class CPos {
public:
    CPos(int y, int x, int hour)
            : m_hour(hour), m_posY(y), m_posX(x)
    {
    }

    int m_hour;
    int m_posY;
    int m_posX;
};

#endif /* __PROGTEST__ */

class CBattleRoyale {
public:
    CBattleRoyale(unsigned height, unsigned width, const std::list<CPos> &zones)
    {
        h=height;
        w=width;
        for(const auto & x : zones){
            zone[{x.m_posX,x.m_posY}]=x.m_hour;
        }
    }

    // des (if needed)

    unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const
    {
        auto it =zone.find({xSt,ySt});
        if(it!= zone.end() && it->second == 0){
            throw logic_error("Path does not exist.");
        }


        queue<pair<pair<int,int>,int>> q;
        set<pair<int,int>> visited;
        q.emplace(make_pair(xSt,ySt),0);
        visited.emplace(xSt,ySt);

        while(!q.empty()){
            auto curr = q.front();
            q.pop();

            if(curr.first.first == xEn && curr.first.second == yEn){
                return curr.second;
            }
            int xAdd [4] = {1,0,0,-1};
            int yAdd [4] = {0,1,-1,0};
            for (int i = 0; i < 4; i++) {
                int currX = curr.first.first + xAdd[i];
                int currY = curr.first.second + yAdd[i];

                if(currX >= 0 && currX < w && currY >= 0 && currY < h && visited.count({currX,currY}) == 0){
                    auto z = zone.find({currX,currY});
                    if(z == zone.end() || z->second > curr.second + 1) {
                        visited.emplace(currX, currY);
                        q.emplace(make_pair(currX, currY), curr.second + 1);
                    }
                }

            }
        }
        throw logic_error("Path does not exist.");


    }

private:
    map<pair<int,int>,int> zone;
    int h; //height
    int w; //width
};

#ifndef __PROGTEST__
int main() {
    CBattleRoyale r1(5, 5, {});
    assert(r1.findRoute(0, 0, 4, 0) == 4);
    assert(r1.findRoute(4, 4, 4, 4) == 0);

    CBattleRoyale r2(6, 7, {CPos(1, 0, 1), CPos(2, 1, 2), CPos(3, 2, 5)});
    assert(r2.findRoute(0, 0, 4, 0) ==  10);


    CBattleRoyale r3(8, 8, {CPos(0, 2, 1), CPos(3, 1, 2), CPos(2, 1, 0)});
    try {
        r3.findRoute(2, 1, 4, 7);
        assert("Exception missing!" == nullptr);
    } catch (const logic_error &e) {
    } catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    assert(r3.findRoute(0,2,3,0)==5);

    CBattleRoyale b(5,5,{CPos(0,1,1),CPos(1,1,0)});
    assert(b.findRoute(0,0,2,2)==4);
    assert(b.findRoute(0,0,0,2)==6);
    assert(b.findRoute(3,3,3,3)==0);
    try{
        assert(b.findRoute(1,1,2,1)==1);
        assert("Kde vyjimka?"==nullptr);
    } catch (logic_error & x){}
    try{
        assert(b.findRoute(1,1,1,1)==0);
        assert("Kde vyjimka? xd"==nullptr);
    } catch (logic_error & x){}

    CBattleRoyale b1(5,5,{CPos(2,0,2), CPos(2,1,1),CPos(2,2,1), CPos(2,3,3),CPos(2,4,4)});
    try{
        b1.findRoute(1,1,3,1);
        assert("Kde vyjimka?"==nullptr);
    } catch (logic_error & x){}

}
#endif /* __PROGTEST__ */
