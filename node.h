#define DEGREE 5
#define Max
#include <iostream>
using namespace std;


template<typename Index,typename Data>
class Node{
    int size=0;
    Index array[DEGREE-1];
    string pointers[DEGREE];
    public: 
    void add(Data data,Index index );
    void Delete(Index index);
    void update(Data data, Index index);


};

