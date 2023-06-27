#ifndef __PROGTEST__
#include <algorithm>
#include <assert.h>
#include <deque>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stdexcept>
#include <unordered_map>
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

    bool operator <(const CPos& other) const{
        return tie(m_posX, m_posY, m_hour) <  tie(other.m_posX, other.m_posY, other.m_hour);
    }
};
#endif /* __PROGTEST__ */

class CBattleRoyale {
public:
    CBattleRoyale(unsigned height, unsigned width, const std::list<CPos> &zones)
    {
        _height = height;
        _width = width;
        for (const auto &item: zones){
            _zones[{item.m_posX, item.m_posY}] = item.m_hour;
        }
    }

    // des (if needed)


    [[nodiscard]] vector<pair<int,int >> getNeighbours(int x, int y) const{
        vector<pair<int,int>> res;

        if(x + 1 < _width){
            res.emplace_back(x+1,y);
        }
        if(x - 1 >= 0 ){
            res.emplace_back(x-1,y);
        }
        if(y - 1 >= 0){
            res.emplace_back(x, y -1);
        }
        if(y + 1 < _height){
            res.emplace_back(x, y +1);
        }

        return res;
    }

    [[nodiscard]] unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const
    {
        if(ySt < 0 || xSt <0 || yEn < 0 || xEn < 0){
            throw logic_error("Path does not exist :(");
        }

        if(ySt >= _height || xSt >= _width || yEn >= _height || xEn >= _width){
            throw logic_error("Path does not exist :(");
        }

        if(_zones.count({xSt,ySt}) != 0 && _zones.find({xSt,ySt})->second == 0){
            throw logic_error("Path does not exist :(");
        }


        queue<pair<pair<int,int>,int >>q;
        set<pair<int,int>> visited;

        q.emplace(make_pair(xSt,ySt), 0);
        visited.emplace(xSt,ySt);

        while(!q.empty()){
            auto current = q.front();
            q.pop();

            if(current.first.first == xEn && current.first.second == yEn){
                return current.second;
            }

            auto neighbours = getNeighbours(current.first.first, current.first.second);

            for (const auto &n: neighbours){
                if(visited.count({n.first,n.second}) == 0){
                    if(_zones.count({n.first,n.second}) == 0 || _zones.find({n.first,n.second})->second > current.second+1)
                    {
                        q.emplace(make_pair(n.first,n.second), current.second+1);
                        visited.emplace(n.first,n.second);
                    }
                }
            }

        }

        throw logic_error("");
    }

private:
    map<pair<int,int>, int> _zones;
    unsigned _height;
    unsigned _width;

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
