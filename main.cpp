#include <iostream>
#include "allocator.h"
#include <cstdlib>

int main()
{
    init_pool();
    int* ptr = (int*)malloc(sizeof(int));

    if (ptr == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr = 11;
    std::cout << "the value is: " << *ptr << '\n';
    
    free(ptr);
    std::cout << "the value is: " << *ptr << '\n'; //garbage
    ptr = nullptr;
    
    int* ptr1 = (int*)mymalloc(sizeof(int));
    
    if (ptr1 == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr1 = 22;
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
   
   // testing coalesce
   
    int* ptr3 = (int*)mymalloc(sizeof(int));
    *ptr3 = 30;
    std::cout << "\n\n value of ptr3: " << *ptr3 << '\n';
    
    int* ptr4 = (int*)mymalloc(sizeof(int));
    *ptr4 = 40;
    std::cout << "\n\n value of ptr4: " << *ptr4 << '\n';
    
    myfree(ptr3);
    std::cout << "address of ptr3: " << ptr3 << '\n';
    ptr3 = nullptr;

    myfree(ptr4);
    std::cout << "address of ptr4: " << ptr4 << '\n';
    std::cout << "sizeof of ptr4: " << sizeof(ptr4) << '\n';
    ptr4 = nullptr;
    std::cout << "sizeof of block: " << sizeof(block) << '\n';
    std::cout << "sizeof int: " << sizeof(int) << '\n';

    int* ptr5 = (int*)mymalloc(sizeof(int)*4);
    *ptr5 = 70;
    std::cout << "\n\n value of ptr5: " << *ptr5 << '\n';
    myfree(ptr5);
    std::cout << "address of ptr5: " << ptr5 << '\n';
    ptr5 = nullptr;
}