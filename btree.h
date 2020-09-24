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
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>     
#include <time.h>       
using namespace std;

int ORDER;

template<typename T>
class btree{
private:

    enum state_t {OVERFLOW, UNDERFLOW, B_OK};

    struct node {
        std::vector<T> data;
        std::vector<node*> children;
        size_t count{0};
        node* next;
        
        node() 
            : data(ORDER+1), children(ORDER+2, nullptr)
        {
            next = nullptr;
        }

        void insert_into(size_t index, const T& value) {
            size_t j = this->count;
            while (j > index) {
                children[j+1] = children[j];
                data[j] = data[j-1];
                j--;
            }
            children[j+1] = children[j];
            data[j] = value;
            this->count++;
        }

        void push_back(const T& value) {
            insert_into(this->count, value);
        }

        state_t insert(const T& value) {
            // binary_search
            size_t index = 0;
            while (this->data[index] < value  && index < this->count) {
                index += 1; 
            }
            if (this->children[index] == nullptr) {
                // this is a leaf node
                this->insert_into(index, value);
            } else {
                auto state = this->children[index]->insert(value);
                if (state == state_t::OVERFLOW) {
                    // split 
                    this->split(index);
                }
            }
            return this->count > ORDER ? OVERFLOW : B_OK;
        }

        void split(size_t position) {
            // leaf nodes / index nodes
            node* parent = this; 
            node* ptr = this->children[position];

            // TODO: reuse ptr buffer 
            node* child1 = new node();
            node* child2 = new node();
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
            
            parent->insert_into(position, ptr->data[mid]); 
            parent->children[position] = child1;
            parent->children[position + 1] = child2;

        }
    };

    

public:


    btree(){

    }

    btree(int i) {
        ORDER = i;
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

  void print(node *ptr, int level) {
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
      node* temp = &root;
      while(temp->children[0]) {
          temp = temp->children[0];
      }
      do {
          for(auto e: temp->data) std::cout<<e<<",";
          temp = temp->next;
      }while(temp->next);
  }

private: 
    // 
    void split_root() {
        node* ptr = &root;
        node* child1 = new node();
        node* child2 = new node();
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

private:
    node root;
    //int ORDER;
};
