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
   
//#include "node.h" 
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>     
#include <time.h>   
#define DEGREE 5
#include <iostream>


using namespace std;
int ORDER;
enum state_t {OVERFLOW, UNDERFLOW, B_OK, BORROW, NOT_FOUND,FOUND ,FIRST_ITEM};


template<typename T>
class btree{
private:
    
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
        state_t delete_from(const T& value){
            for(int i=0;i<this->count;i++){
                cout<<data[i]<<" ";
            }cout<<endl;
            for(size_t i = 0; i < this->count; i++){
                if(this->data[i] == value){
                    for (int j=i+1;j<this->count;j++){
                        this->data[j-1]=this->data[j];
                    }
                    this->count--;
                    cout<<"i : "<<i<<endl; 
                    for(int i=0;i<this->count;i++){
                        cout<<data[i]<<" ";
                    }cout<<endl;
                    if(i == 0){

                        return state_t::FIRST_ITEM;

                    }
                    return state_t::FOUND;
                }
            }
            return state_t::NOT_FOUND;
        
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

        state_t delete_index(const T& value){
            size_t index = 0;
            state_t output;
            while (this->data[index] < value  && index < this->count) {
                index += 1; 
            }
            if (this->children[index] == nullptr) {
                // this is a leaf 
                output=this->delete_from(value);
            } else {
                cout<<"index: "<< index<<endl;
                output = this->children[index+1]->delete_index(value);
                if (output==state_t::FIRST_ITEM)cout<<"FIRST_ITEM"<<endl;
                if (output==state_t::FOUND)cout<<"FOUND"<<endl;
                if (output==state_t::NOT_FOUND)cout<<"NOT_FOUND"<<endl;

                 
                switch(output){
                    case state_t::FIRST_ITEM:{
                        cout<<"llego al first item"<<endl;
                        if(this->children[index+1]->count >= ORDER/2){
                            this->data[index] = this->children[index+1]->data[0];
                            output=FOUND;
                            break;
                        }else{
                            cout<<"llego al else"<<endl;
                            output=BORROW;
                        }
                    }
                    case state_t::BORROW:{
                        cout<<"llego al borrow"<<endl;
                        if (index==0 || index==this->count-1){
                            //wait
                        }
                        else{
                            if (lend(this->children[index-1],this->children[index])){
                                this->data[index-1] = this->children[index]->data[0];
                            }else if (lend(this->children[index+1],this->children[index])){
                                //ACA se updatea.
                                this->data[index]=this->children[index+1]->data[0];
                            }else{ 
                                //merge
                            }
                        }
                        break;
                    }
                    case state_t::NOT_FOUND:{
                        return state_t::NOT_FOUND;
                    }
                }
            }
            return this->count == ORDER/2 ? BORROW : output;

        }

        bool lend(node * child1, node * to){
            if(child1->count-1 <= ORDER/2)
                return false;
            
            if(child1->data[0] >= to->data[0]){
                //this is right child
                to->data[count]=child1->data[0];
                to->count++;
                child1->count--;
                for (int i=1;i<child1->count;i++ ){
                    child1->data[i-1]=child1->data[i];
                }
            }
            else{
                //borrow from left child
                to->insert_into(0,child1->data[child1->count-1]);
                child1->count--;
            } 

            return true;            
        }
        void split(size_t position) {
            // leaf nodes / index nodes
            node* parent = this; 
            node* ptr = this->children[position];

            // TODO: reuse ptr buffer 
            //node* child1 = new node();
            node* child2 = new node();
            /*for (; i < ptr->count / 2; i++) {
                child1->children[i] = ptr->children[i];
                child1->data[i] = ptr->data[i];
                child1->count++;
            }
            child1->children[i] = ptr->children[i];
            */
           
            size_t i = ptr->count/2;
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
            ptr->count/=2;
            
            parent->insert_into(position, ptr->data[mid]); 
            parent->children[position] = ptr; //child1;
            parent->children[position + 1] = child2;

            if(!ptr->children[0]){
                child2->next=ptr->next;
                ptr->next = child2;
            }


        }
    };

node root;


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
      while(temp!=nullptr){
          for(int i=0;i<temp->count;i++){
              cout<<temp->data[i]<<",";
          }
          temp=temp->next;
      }
      /*
      do {
          for(auto e: temp->data) std::cout<<e<<",";
          temp = temp->next;
      }while(temp);
    */ 
  }

    void printLeafs(){
        node * temp = &root;
        while(temp->children[0])
            temp = temp->children[0];
        printRecursive(temp);
        //std::cout << std::endl;
    }

    void delete_index(T index){
        auto state=root.delete_index(index);
    }

private: 
    //
   void printRecursive(node* temp){
        if (!temp)
            return;
        for(int i = 0;  i < temp->count; i++){
            std::cout << temp->data[i] << ", ";
        }
        temp = temp->next;
        printRecursive(temp);
    }
 
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
        child1->next=child2;

    }


};
