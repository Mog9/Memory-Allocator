# Custom Memory Allocator in C++
A memory allocator built from scratch in C++ using `mmap` for pool-based allocation. Implements `mymalloc`, `myfree`, block coalescing, and block splitting without any standard library allocation functions.

---

## How it works
A 1MB memory pool is created upfront via a single `mmap` call. All allocations carve blocks out of this contiguous pool. Each block stores metadata (size, free status, next pointer) immediately before the user data. The free list is kept sorted by memory address so adjacent free blocks can be merged correctly. When a reused block is larger than needed, block splitting carves it into an allocated chunk and a smaller free leftover, reducing wasted space.

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

## Benchmark vs standard malloc

Benchmarked over 1,000,000 iterations on Linux. Lower is better.

| Test | mymalloc | malloc |
|---|---|---|
| Single size (4 bytes) | 7.82 ms | 16.61 ms |
| Mixed sizes (8-512 bytes) | 16.49 ms | 7.89 ms |
| Bulk allocate 1000 then free all | 1.55 ms | 0.026 ms |
| Reverse order free | 1.88 ms | 0.011 ms |

Single size allocation is competitive with and occasionally beats standard malloc due to cache-friendly free list reuse. Mixed sizes and bulk free patterns are slower because sorted insertion into the free list, required for coalescing correctness, makes each free an O(n) operation.

### perf analysis

| Metric | Value |
|---|---|
| Total time | 47.27 ms |
| Instructions | 542,714,619 |
| CPU cycles | 192,681,943 |
| Instructions per cycle | 2.8 |
| Branch misses | 19,919 / 107,406,979 (0.018%) |
| Backend bound | 27.6% |
| Retiring | 54.4% |

2.8 instructions per cycle indicates the CPU is kept busy. 54.4% retiring means the majority of cycles are doing real useful work. The 27.6% backend bound points to memory latency stalls from walking the linked list during sorted insertion, which confirms why bulk free is slow. Branch prediction is nearly perfect at 0.018% miss rate, meaning the list traversal logic is highly predictable. The bottleneck is memory access patterns, not computation.

---

## Build and run
```bash
g++ main.cpp -o mall
g++ -O2 -o benchmark benchmark.cpp -std=c++17
```