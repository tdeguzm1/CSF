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

class set {
    public: 
        set(unsigned index, cache_stats stats);
        int find(unsigned tag);
        void insert(unsigned tag, unsigned time);
        void update(unsigned target_tag, unsigned time);

    private:
        unsigned index;
        std::vector<slot> slots;
        cache_stats myStats;

        unsigned remove();
};


#endif