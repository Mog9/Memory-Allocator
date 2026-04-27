# Custom Memory Allocator in C++

A memory allocator built from scratch in C++ using `mmap` for pool-based allocation. Implements `mymalloc`, `myfree`, and block coalescing without any standard library allocation functions.

---

## How it works

A 1MB memory pool is created upfront via a single `mmap` call. All allocations carve blocks out of this contiguous pool. Each block stores metadata (size, free status, next pointer) immediately before the user data. The free list is kept sorted by memory address so adjacent free blocks can be merged correctly.

---

## Baseline -- standard malloc and free

Before testing the custom allocator, standard `malloc` and `free` behavior is verified. After freeing a pointer, reading from it returns garbage since the memory is no longer owned.

<img width="454" height="90" alt="Screenshot From 2026-04-27 16-43-07" src="https://github.com/user-attachments/assets/dc71f7d8-2b94-4883-926b-db05992d572b" />

---

## Step 1 -- basic allocation and free

`mymalloc` carves a block from the pool and returns a pointer past the metadata. `myfree` marks the block as free without releasing it back to the OS, keeping it available for reuse.

<img width="454" height="90" alt="Screenshot From 2026-04-27 16-43-38" src="https://github.com/user-attachments/assets/8be29dbd-c9a9-4fc4-a248-e3a806f45551" />



---

## Step 2 -- block reuse

After freeing a block, the next allocation of the same size reuses it instead of consuming new pool memory. Verified by comparing pointer addresses before and after.

<img width="932" height="330" alt="Screenshot From 2026-04-27 16-49-46" src="https://github.com/user-attachments/assets/c966220b-a346-414c-b926-84fb7a23209e" />
---
<img width="462" height="298" alt="image" src="https://github.com/user-attachments/assets/12c0b6fb-db0c-4e6f-a199-d4339fa8b207" />


---

## Step 3 -- coalescing and reuse of merged blocks

Two adjacent blocks are allocated and freed. The allocator merges them into a single larger block via coalescing. A subsequent allocation larger than either individual block reuses the merged space.

<img width="1212" height="306" alt="image" src="https://github.com/user-attachments/assets/ef331cf1-d36b-4327-9777-3493b851e5a9" />


---

## Build and run

```bash
g++ main.cpp -o mall 
```

---

## What is not implemented

- Block splitting for oversized free blocks
- Pool overflow handling
- Thread safety
- Alignment guarantees beyond size rounding