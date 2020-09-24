// #include "node.cpp"
// template<typename Index,typename Data>
// class btree{
//     string root;
//     Node<Index,Data> find(Node<Index,Data> root, Index );
//     public:
//     void insert(Data data,Index index );
//     void Delete(Index index);
//     void update(Data data, Index index);


// };
   
#include "node.h" 
using namespace std;


template<typename T>
class btree{
private:
    node<T> root;
public:


    btree(){

    }

    btree(int order) {
        ORDER = order;
    }
    
    void insert(const T& value) {
        auto state = root.insert(value);
        if (state == state_t::OVERFLOW) {
            // split root node
            split_root();
        }
    }

  void print() {
    print(&root, 0);
    std::cout << "________________________\n";
  }

  void print(node<T> *ptr, int level) {
    if (ptr) {
      int i;
      for (i = ptr->count - 1; i >= 0; i--) {
        print(ptr->children[i + 1], level + 1);

        for (int k = 0; k < level; k++) {
          std::cout << "    ";
        }
        std::cout << ptr->data[i] << "\n";
      }
      print(ptr->children[i + 1], level + 1);
    }
  }

  void inOrder() {
      node<T>* temp = &root;
      while(temp->children[0]) {
          temp = temp->children[0];
      }
      while(temp!=nullptr){
          for(int i=0;i<temp->count;i++){
              cout<<temp->data[i]<<endl;
          }
          temp=temp->next;
          cout<<"salto"<<endl; 
      }
      /*
      do {
          for(auto e: temp->data) std::cout<<e<<",";
          temp = temp->next;
      }while(temp);
    */ 
  }

    void printLeafs(){
        node<T> * temp = &root;
        while(temp->children[0])
            temp = temp->children[0];
        printRecursive(temp);
        std::cout << std::endl;
    }

private: 
    //
   void printRecursive(node<T>* temp){
        if (!temp)
            return;
        for(int i = 0;  i < temp->count; i++){
            std::cout << temp->data[i] << " ";
        }
        temp = temp->next;
        printRecursive(temp);
    }
 
    void split_root() {
        node<T>* ptr = &root;
        node<T>* child1 = new node<T>();
        node<T>* child2 = new node<T>();
        size_t i = 0;
        for (; i < ptr->count / 2; i++) {
            child1->children[i] = ptr->children[i];
            child1->data[i] = ptr->data[i];
            child1->count++;
        }
        child1->children[i] = ptr->children[i];
        auto mid = i;
        if(ptr->children[0]) i += 1;
        size_t j = 0;
        for (; i < ptr->count; i++) {
            child2->children[j] = ptr->children[i];
            child2->data[j] = ptr->data[i];
            child2->count++;
            j++;
        }
        child2->children[j] = ptr->children[i];

        ptr->data[0] = ptr->data[mid];
        ptr->children[0] = child1;
        ptr->children[1] = child2;
        ptr->count = 1;

    }


};
