
#include "cache.h"

cache::cache(char* input_args[]) {
    timer = 0;
    num_sets = (unsigned) atoi(input_args[1]);
    blocks = (unsigned) atoi(input_args[2]);
    bytes = (unsigned) atoi(input_args[3]);
    w_alloc = (strcmp(input_args[4], "write-allocate") == 0) ? 0 : 1;     // 0 if write-allocate, 1 if no-write-allocate
    w_scheme = (strcmp(input_args[5], "write-back") == 0) ? 0 : 1;       // 0 if write-back, 1 if write-through
    rem_scheme = (strcmp(input_args[6], "lru") == 0) ? 0 : 1;          // 0 id LRU, 1 if FIFO
    
    if (w_alloc != w_scheme) {
        std::cerr << "Invalid combination of write states" << std::endl;
        throw -2;
    }

    for (unsigned i = 0; i < num_sets; i++){
        sets.push_back(set(i, blocks, bytes));
    }
}





