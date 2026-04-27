#include <iostream>
#include "allocator.h"
#include <cstdlib>

int main()
{
    int* ptr = (int*)malloc(sizeof(int));

    if (ptr == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr = 98;
    std::cout << "the value is: " << *ptr << '\n';
    
    free(ptr);
    std::cout << "the value is: " << *ptr << '\n'; //garbage
    ptr = nullptr;
    
    int* ptr1 = (int*)mymalloc(sizeof(int));
    
    if (ptr1 == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr1 = 45;
    std::cout << "the value is: " << *ptr1 << '\n';
    
    myfree(ptr1);
    // std::cout << "the value is: " << *ptr1 << '\n'; //direct unmap, segmentation fault (no reuse)
    std::cout << "address of ptr1: " << ptr1 << '\n'; //checking resuse by printing ptr address
    ptr1 = nullptr;
    
    //checking reuse
    int* ptr2 = (int*)mymalloc(sizeof(int));
    std::cout << "address of ptr2: " << ptr2 << '\n'; //checking if it matches the ptr1 address
    
    /* address matches, its reusing properly
    address of ptr1: 0x7f253dfc6018
    address of ptr2: 0x7f253dfc6018
    
    address of ptr1: 0x7f1d12cee018
    address of ptr2: 0x7f1d12cee018

    address of ptr1: 0x7fae12d9d018
    address of ptr2: 0x7fae12d9d018
    */
}