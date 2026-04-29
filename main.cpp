#include <iostream>
#include "allocator.h"
#include <cstdlib>

int main()
{
    init_pool();
    int* ptr1 = (int*)malloc(sizeof(int));

    if (ptr1 == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr1 = 11;
    std::cout << "\ndefault malloc ptr1 value is: " << *ptr1 << '\n';
    
    free(ptr1);
    std::cout << "the address of malloc is: " << ptr1 << "\n\n";
    ptr1 = nullptr;
    
    int* ptr2 = (int*)mymalloc(sizeof(int));
    
    if (ptr2 == nullptr){
        std::cerr << "mem allocation failed" << '\n';
    }
    *ptr2 = 25;
    std::cout << "my malloc ptr2 value is: " << *ptr2 << '\n';
    
    myfree(ptr2);
    std::cout << "address of my malloc: " << ptr2 << "\n\n"; //checking resuse by printing ptr address
    ptr2 = nullptr;
    
    //checking reuse
    int* ptr3 = (int*)mymalloc(sizeof(int));
    *ptr3 = 45;
    std::cout << "ptr2 is freed and ptr3 is created: " << "\n\n";
    std::cout << "value of ptr3: " << *ptr3 << "\n"; 
    std::cout << "address of ptr3: " << ptr3 << "\n\n"; //checking if it matches the ptr1 address
    std::cout << "ptr2 was freed and new ptr3 was created and instead of new address, it uses same address" << "\n\n";
    
    /* few more examples:
    address of ptr2: 0x7f253dfc6018
    address of ptr3: 0x7f253dfc6018
    
    address of ptr2: 0x7f1d12cee018
    address of ptr3: 0x7f1d12cee018
    
    address of ptr2: 0x7fae12d9d018
    address of ptr3: 0x7fae12d9d018
    */
   
    // testing coalesce
    std::cout << "testing colescing: " << "\n\n\n";
   
    int* ptr4 = (int*)mymalloc(sizeof(int));
    *ptr4 = 30;
    std::cout << "value of ptr4: " << *ptr4 << '\n';
    
    int* ptr5 = (int*)mymalloc(sizeof(int));
    *ptr5 = 40;
    std::cout << "value of ptr5: " << *ptr5 << '\n';
    
    myfree(ptr4);
    std::cout << "address of ptr4: " << ptr4 << " (creates new address because both are created first then freed)" << '\n';
    ptr4 = nullptr;

    myfree(ptr5);
    std::cout << "address of ptr5: " << ptr5 << " (creates new address because both are created first then freed)" <<'\n';
    ptr4 = nullptr;

    int* ptr6 = (int*)mymalloc(sizeof(int)*4);
    *ptr6 = 70;
    std::cout << "\n\nvalue of ptr6: " << *ptr6 << '\n';
    myfree(ptr6);
    std::cout << "address of ptr6: " << ptr6 << " -> using the address of the first block created (both blocks were freed and used as 1)" << "\n\n";
    ptr6 = nullptr;
    
    //test blocksplit
    int* ptr7 = (int*)mymalloc(sizeof(int) * 20);
    *ptr7 = 89;
    std::cout << "\n\nvalue of ptr7: " << *ptr7 << '\n';
    myfree(ptr7);
    std::cout << "address of ptr7: " << ptr7 <<"\n\n";
    ptr7 = nullptr;
    
    int* ptr8 = (int*)mymalloc(sizeof(int));
    *ptr8 = 99;
    std::cout << "\n\nvalue of ptr8: " << *ptr8 << '\n';
    std::cout << "address of ptr8: " << ptr8 <<"\n\n";
}