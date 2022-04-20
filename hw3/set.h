/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#ifndef SET_H
#define SET_H

#include "slot.h"
#include "cache_stats.h"
#include <map>
#include <iostream>

#define COUNTS_4_BYTE_MEMORY 100 // counts to access 4 bytes of memory
#define COUNTS_4_BYTE_CACHE 1 // counts to access 4 bytes of cache

class set {
    public: 

        /**
         *  Constuctor for sets
         *  
         *  Parameters:
         *  new_index - provides the set with information about its own index in the cache
         *  stats - a structure providing the set with information about the entire cache
         */
        set(unsigned index, cache_stats stats);
        
        /**
         *  Checks if this set contains the target tag
         *  
         *  Parameters:
         *  target_tag - the tag to check if its in the set
         */
        bool contains(unsigned tag);
        
        /**
         *  Inserts the target tag into the cache
         *  
         *  Parameters:
         *  target_tag - the tag to check if its in the set
         *  time - the current time state of the cache
         */
        void insert(unsigned tag, unsigned time);
        
        /**
         *  Updates the target tag's access timestamp with current cache time
         *  
         *  Parameters:
         *  target_tag - the tag to update
         *  time - the current time state of the cache
         */
        void update(unsigned target_tag, unsigned time);
        
        /**
         *  Updates the target tag's dirty field 
         *  
         *  Parameters:
         *  target_tag - indicates the slot to update
         */
        void make_dirty(unsigned tag);
        
        /** 
         *  Prints the current state of the set (for debugging)
         */
        void print_set();

    private:
        unsigned index;
        unsigned num_slots; // number of filled slots
        std::map<unsigned, slot> slots; // map of slots
        cache_stats myStats;

        /**
         *  Removes a value to make room (by fifo or lru as appropriate)
         */
        void remove();
};


#endif