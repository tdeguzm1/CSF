/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include "set.h"
#include "cache_stats.h"


class cache {
    public: 
        cache(char* input_args[]);
        void load_store(std::string ls, std::string addr);
        void print_summary();

    private:
        unsigned timer;
        cache_stats myStats;
        std::vector<set> sets;

        void cache_load(unsigned address);
        void cache_store(unsigned address);
        void write_cache_on_hit(unsigned block_number, unsigned index);
        void write_cache_on_miss(unsigned block_number, unsigned index);

};


#endif