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
        
        /**
         *  Constructor for cache object.
         *
         *  Parameters:
         *  input_args - the values input to create a cache
         *                 Pre: input arguments have already been validated
         */
        cache(char* input_args[]);


        /**
         *  Destructor for cache object
         *  Cleans up dynamic memory
         */
        ~cache();
        
        /**
         *  Calls load or store based on input condition
         *
         *  Parameters:
         *  ls - string indicating load or store
         *  addr - string indicating the hexidecimal address
         */
        void load_store(std::string ls, std::string addr);
        
        /**
         *  Print the summary of the cache state at the end of the program
         */
        void print_summary();
        
        /**
         *  Prints the current cache state
         */
        void print_current();

        /**
         *  Returns a struct of the cache summary for the experiment
         */
        cache_summary getSummary() { return *(myStats.mySummary); }

    private:
        unsigned timer;            // keep track of time in simulation (for fifo/lru)
        cache_stats myStats;       // a struct containting info about the cache
        std::vector<set> sets;     // stores all of the sets within the cache

        /**
         *  Checks for cache hit/miss and loads appropriately
         *
         *  Parameters:
         *  addr - unsigned indicating the address
         */
        void cache_load(unsigned address);

        /**
         *  Checks for cache hit/miss and stores appropriately
         *
         *  Parameters:
         *  addr - unsigned indicating the address
         */
        void cache_store(unsigned address);

        /**
         *  Handles writes when cache hits occur
         *
         *  Parameters:
         *  tag - unsigned indicating the tag
         *  index - unsidned indicating the index
         */
        void write_cache_on_hit(unsigned block_number, unsigned index);

        /**
         *  Handles writes when cache misses occur
         *
         *  Parameters:
         *  tag - unsigned indicating the tag
         *  index - unsidned indicating the index
         */
        void write_cache_on_miss(unsigned block_number, unsigned index);

        
};


#endif