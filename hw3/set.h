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

class set {
    public: 
        set(unsigned index, cache_stats stats);
        bool contains(unsigned tag);
        void insert(unsigned tag, unsigned time);
        void update(unsigned target_tag, unsigned time);
        void make_dirty(unsigned tag);
        void print_set();

    private:
        unsigned index;
        std::map<unsigned, slot> slots;
        cache_stats myStats;

        void remove();
};


#endif