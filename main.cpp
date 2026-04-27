#include <iostream>
#include "allocator.h"
#include <cstdlib>

int main()
{
    int* ptr = (int*)malloc(sizeof(int));

    if (ptr == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr = 66;
    std::cout << "the value is: " << *ptr << '\n';
    
    free(ptr);
    std::cout << "the value is: " << *ptr << '\n'; //garbage
    ptr = nullptr;
    
    int* ptr1 = (int*)mymalloc(sizeof(int));
    
    if (ptr1 == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr1 = 33;
    std::cout << "the value is: " << *ptr1 << '\n';
    
    myfree(ptr1);
    std::cout << "the value is: " << *ptr1 << '\n'; //direct unmap, segmentation fault (no reuse)
    ptr1 = nullptr;
        
}