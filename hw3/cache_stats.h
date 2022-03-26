/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#ifndef CACHE_STATS_H
#define CACHE_STATS_H

struct cache_summary {
    unsigned loads = 0;
    unsigned stores = 0;
    unsigned load_hits = 0;
    unsigned load_misses = 0;
    unsigned store_hits = 0;
    unsigned store_misses = 0;
    unsigned total_count = 0;
};

struct cache_stats {
    unsigned num_sets;         // number of sets in the cache
    unsigned num_slots;       // number of blocks per set
    unsigned num_bytes;        // number of bytes per block
    bool w_alloc;              // 0 if write-allocate, 1 if no-write-allocate
    bool w_scheme;             // 0 if write-back, 1 if write-through
    bool rem_scheme;           // 0 id LRU, 1 if FIFO
    cache_summary* mySummary;  // pointer to a summary struct that can be accessed by any subset of the cache
};


#endif