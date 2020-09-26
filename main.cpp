#include "btree.h"

int main(){
    srand (time(NULL));
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    int n, temp;
    std::cin>>n>>ORDER;
    btree<int> bt;
    
    for (int i = 0; i < n; i++) {
        cin>>temp;
        bt.insert(temp);
    }

    
    
    bt.print();
    for (int i=0;i<2;i++){
        cin>>temp;
        bt.delete_index(temp);
        bt.print();
    }
    
    bt.printLeafs();
    return 0;
}
//10 3 
//1 4 3 7 9 5 8 2 10 6
//6 5 
