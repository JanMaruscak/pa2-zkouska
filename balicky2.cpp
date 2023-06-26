#include<iostream>
#include<cassert>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<unordered_set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>

using namespace std;

class CPkg{
public:
    //TODO constructor
    //TODO addDep(string)

    CPkg & addDep(const string& dep){
        dependencies.insert(dep);
        return *this;
    }

    CPkg(const string & name) : _name(name){

    }

    bool operator<(const CPkg& other) const{
        return _name < other._name;
    }


    string _name;
    set<string> dependencies;

};
class CPkgSys{
public:
    CPkgSys & addPkg(const CPkg & cPkg){
        packages[cPkg] = false;
        return *this;
    }

    set<string> install(list<string> names){
        set<string> newly;
        for (const auto &name: names){
            auto it = packages.find(name);
            if(it == packages.end() || it->first._name != name){
                for (const auto &item: newly){
                    packages[item] = false;
                }
                throw invalid_argument("Package not found.");
            }


            queue<string> q;
            q.emplace(name);
            for (const auto &dep: it->first.dependencies){
                q.emplace(dep);
            }


            while(!q.empty()){
                auto current = q.front();
                q.pop();

                auto it = packages.find(current);
                if(it == packages.end() || it->first._name != current){
                    for (const auto &item: newly){
                        packages[item] = false;
                    }
                    throw invalid_argument("Package not found.");
                }
                else if(it->second){
                    continue;
                }


                if(packages.find(current) != packages.end())
                    for (const auto &item: packages.find(current)->first.dependencies){
                        q.emplace(item);
                    }
                if(packages.find(current) == packages.end() || !packages[current]){
                    newly.emplace(current);
                }
                packages[current] = true;
            }
        }
        return newly;


    }
    //TODO addPkg
    //TODO install(list<string> )

    friend ostream& operator<<(ostream  & os,const CPkgSys & other);

    //TODO operator <<
private:
    map<CPkg, bool> packages;
};

ostream &operator<<(ostream &os, const CPkgSys &other) {

    bool first = true;
    for (const auto &item: other.packages){
        if(item.second){
            if(first){
                os << item.first._name;
                first = false;
            }
            else{
                os << ", " << item.first._name ;
            }
        }
    }
    return os;
}

int main(void){
    CPkgSys s;
    stringstream ss;
    s.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
     .addPkg(CPkg("sudo").addDep("git").addDep("c++"));
    s.addPkg(CPkg("apt"))
     .addPkg(CPkg("c++").addDep("c").addDep("asm").addDep("fortran"));
    s.addPkg(CPkg("git"))
     .addPkg(CPkg("c").addDep("kekw"))
     .addPkg(CPkg("kekw"))
     .addPkg(CPkg("asm"))
     .addPkg(CPkg("fortran"));
    s.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
     .addPkg(CPkg("karel").addDep("python"))
     .addPkg(CPkg("bash").addDep("sadge"))
     .addPkg(CPkg("sadge"))
     .addPkg(CPkg("cython").addDep("dev"));
    s.addPkg(CPkg("perl"));

    ss << s;
    assert(ss.str() == "");
    ss.clear();
    ss.str("");

    set<string> t1 = s.install(list<string> {"sudo"});
    assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));
    set<string> t2 = s.install(list<string> {"ssh", "c++"});
    assert(t2 == (set<string> {"apt", "ssh"}));

    ss << s;
    assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
    ss.clear();
    ss.str("");

    try{
        set<string> e = s.install(list<string> {"karel", "cython"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
    assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

    s.addPkg(CPkg("java").addDep("utils"))
            .addPkg(CPkg("utils").addDep("VB"))
            .addPkg(CPkg("VB").addDep("java"));

    set<string> t4 = s.install(list<string> {"java", "perl"});
    assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

    ss << s;
    assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
    ss.clear();
    ss.str("");

    CPkgSys k;
    k.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
     .addPkg(CPkg("sudo").addDep("git"));
    k.addPkg(CPkg("apt"));
    k.addPkg(CPkg("git"))
     .addPkg(CPkg("c").addDep("kekw"))
     .addPkg(CPkg("kekw"))
     .addPkg(CPkg("asm"))
     .addPkg(CPkg("fortran"));
    k.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
     .addPkg(CPkg("karel").addDep("python"))
     .addPkg(CPkg("bash").addDep("sadge"))
     .addPkg(CPkg("sadge"));
    k.addPkg(CPkg("perl").addDep("no"));

    set<string> t5 = k.install(list<string> {"asm"});
    assert(t5 == (set<string> {"asm"}));
    set<string> t6 = k.install(list<string> {"python", "ssh"});
    assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

    try{
        set<string> t7 = k.install(list<string> {"perl", "c"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});

    assert(t8 == (set<string> {"c", "karel", "kekw"}));

    ss << k;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
    ss.clear();
    ss.str("");

    cout << "DONE" << endl;

}
