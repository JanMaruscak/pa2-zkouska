#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <list>
#include <cassert>

using namespace std;
using point = pair<int,int>;
using mazeMap = vector<vector<int>>;

class Maze{
public:
    void loadMaze(const vector<vector<int>> &maze);

    list<point> findMaze(const pair<int,int> &start, const pair<int,int> &end)const;
private:

    vector<vector<int>> _maze;
};

void Maze::loadMaze(const vector<vector<int>> & maze) {
    _maze = maze;
}

list <point> Maze::findMaze(const pair<int,int> &start, const pair<int,int> &end)const {

    if(start.first < 0 || start.first >= _maze.size()){
        throw out_of_range("");
    }
    if(end.first < 0 || end.first >= _maze.size()){
        throw out_of_range("");
    }

    if(start.second < 0 || start.second >= _maze[start.first].size()){
        throw out_of_range("");
    }
    if(end.second < 0 || end.second >= _maze[end.first].size()){
        throw out_of_range("");
    }


    list<point> res;
    queue<pair<int,int>> q;
    map<pair<int,int>, pair<int,int>> prev;

    q.emplace(start);
    prev[start] = start;


    while(!q.empty()){
        auto current = q.front();
        q.pop();

        if(current == end){
            res.push_front(end);
            while(true){
                if(prev[current] == current){
                    return res;
                }
                res.push_front(prev[current]);
                current = prev[current];
            }
        }

        int xAdd[] = {-1,0,0,1};
        int yAdd[] = {0,1,-1,0};
        for (int i = 0; i <4 ; ++i) {
            int newX = current.first + xAdd[i];
            int newY = current.second + yAdd[i];
            if(newX >= 0 && newX < _maze.size()){
                if(newY >= 0 && newY <  _maze[newX].size()){
                    auto it = prev.find({newX,newY});
                    if(it != prev.end()){
                        continue;
                    }
                    if(_maze[newX][newY] == 0){
                        continue;
                    }
                   q.emplace(newX,newY);
                   prev[{newX,newY}] = current;
                }
            }
        }
    }
    throw logic_error("");
}

int main(){

    Maze m;

    mazeMap m1= {{1,1,1,1},
                 {1,0,1,0},
                 {1,1,1,1}};
    point start ={0,0};
    point end = {0,3};
    m.loadMaze(m1);
    auto res = m.findMaze(start,end);
    assert(res == list<point>({{0,0},{0,1},{0,2},{0,3}}));

    Maze m2;
    mazeMap m2m = {{1,1,0,0},
                   {0,1,1,1},
                   {0,1,0,1}};
    start ={0,0};
    end = {2,3};
    m2.loadMaze(m2m);
    res = m2.findMaze(start,end);
    assert(res == list<point>({{0,0},{0,1},{1,1},{1,2},{1,3},{2,3}}));

    Maze m3;

    mazeMap m3m ={
            {1,1,1,0,1,1,0,1,1,1,1,1,1,0},
            {1,0,1,0,1,0,0,0,0,1,0,1,0,1},
            {0,0,1,0,1,0,1,1,1,1,1,1,0,1},
            {1,0,1,0,1,0,0,0,0,0,0,0,0,0},
            {1,1,1,0,1,1,1,1,1,1,1,1,0,1},
            {1,0,1,0,0,0,0,0,0,0,0,0,0,1},
            {0,0,1,1,1,0,1,1,1,1,1,1,0,1},
            {1,0,0,0,1,0,1,0,1,0,0,0,0,1},
            {1,1,1,1,1,1,1,0,1,1,1,1,1,1}
    };

    start ={0,1};
    end = {6,13};

    m3.loadMaze(m3m);
    res = m3.findMaze(start,end);
    assert(res == list<point>({{0,1},{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{6,3},{6,4},{7,4},{8,4},{8,5},{8,6},
                               {7,6},{6,6},{6,7},{6,8},{7,8},{8,8},{8,9},{8,10},{8,11},{8,12},{8,13},{7,13},{6,13}}));

    start = {20,20};
    end = {0,0};
    try{
        res = m3.findMaze(start,end);
        assert(false);
    }catch (std::out_of_range &e){
        assert(true);
    }

    start = {0,0};
    end = {0,12};
    try{
        res = m3.findMaze(start, end);
        assert(false);
    }
    catch (std::logic_error &er){
        assert(true);
    }

    start = {0,0};
    end = {0,13};

    try{
        res = m3.findMaze(start, end);
        assert(false);
    }
    catch (std::logic_error &er){
        assert(true);
    }

    return 0;
}
