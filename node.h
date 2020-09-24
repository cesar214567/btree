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
enum state_t {OVERFLOW, UNDERFLOW, B_OK};
     
template<typename T>
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
                ptr->next = child2;
                child2->next=parent->children[position+2];
            }


        }
    };

