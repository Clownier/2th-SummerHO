#include <iostream>
#include <cstdlib>
#include <climits>
using namespace std;

struct Btreenodebase{
    typedef btreenodebase* Baseptr;
    vector<Baseptr*> childrens;
};

template<class Key, class Value>
struct Btreenode : public btreenodebase{
    typedef Btreenode<Key, Value>* Linktype;
    Value Mvaluefield;
};

int main(){
    BTree T1;
    BTree T2 = T1;
    return 0;
}