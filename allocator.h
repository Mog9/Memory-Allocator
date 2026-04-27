#include <iostream>
#include <sys/mman.h>

struct block {
    size_t size;
    block* next;
    bool is_free;
};

/*

mmap(
    void *address,
    
    size_t length/size,
    
    int prot, = access permisssions, read/write etc
    
    int flags, = map_shared (changes shared with other processes)
    map_private (creates a private copy on write mapping)
    map_anonomus (mapping is not backed by any file, the fd argument should be -1)
    
    int fd, = file descriptor
    off_t offset = starting point in the file from which the mapping begins
    
    )
    
*/





void* mymalloc(size_t size) {
    const size_t METADATA = sizeof(block);
    size_t total_size = METADATA + size; //to get user size + metadata space
    void* addr = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return nullptr;
    } else {
        block* meta = (block*) addr;
        meta -> size = size;
        meta -> is_free = false;
        meta -> next = nullptr;

        return (block*)addr + 1;
        // returning address only means ptr at metadata, doing + 1 puts it to next pointer where user enteres value
    }

}


void myfree(void* ptr) {
    if (!ptr){
        return;
    }

    block* meta = (block*)ptr - 1; //puts it back to where metadata is (actual start)
    
    munmap(meta, meta->size + sizeof(block));
    //give back to OS, from the actual start + its total size, which is (actual size + metadata)
}



