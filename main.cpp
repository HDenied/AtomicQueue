#include<mqueue.h>
#include<iostream>

int main(){
    Mqueue<int,3> val; 
    std::cout<<"Is lock free? "<<val.is_supported()<<"\n";
    return 0;
}