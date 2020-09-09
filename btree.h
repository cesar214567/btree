#include "node.cpp"
template<typename Index,typename Data>
class btree{
    string root;
    Node<Index,Data> find(Node<Index,Data> root, Index );
    public:
    void insert(Data data,Index index );
    void Delete(Index index);
    void update(Data data, Index index);


};
