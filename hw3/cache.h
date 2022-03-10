#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <string.h>
#include <vector>
#include "set.h"


class cache {
    public: 
        cache(char* input_args[]);

    private:
        unsigned timer;   // holds value for timestamp
        unsigned num_sets;         // number of sets in the cache
        unsigned blocks;       // number of blocks per set
        unsigned bytes;        // number of bytes per block
        bool w_alloc;     // 0 if write-allocate, 1 if no-write-allocate
        bool w_scheme;    // 0 if write-back, 1 if write-through
        bool rem_scheme;  // 0 id LRU, 1 if FIFO
        std::vector<set> sets;

};


#endif