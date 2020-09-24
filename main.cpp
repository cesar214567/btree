#include "btree.h"
#include "iostream"
using namespace std;

int main(){
    srand (time(NULL));
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    int n, temp;
    std::cin>>n>>ORDER;
    btree<int> bt;
    for (int i = 0; i < n; i++) {
        //cin>>temp;
        temp = rand() % 100 + 1;
        cout<<temp<<" ";
        bt.insert(temp);
    }
    cout<<endl;
    bt.print();
    bt.printLeafs();
    return 0;
}