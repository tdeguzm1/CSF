/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#ifndef SLOT_H
#define SLOT_H

#include <vector>
#include "cache_stats.h"


class slot {
    public: 
        slot(unsigned new_tag, cache_stats stats);
        void place_value(unsigned new_tag, unsigned time);
        bool isValid();
        unsigned getTag();
        unsigned getAccessTime();
        unsigned getLoadTime();
        void update(unsigned time);

    private:
        cache_stats myStats;  // information about cache
        unsigned tag;         // tag
        unsigned load_ts;     //  loaded timestamp
        unsigned access_ts;   // accessed timestamp
        bool valid;           // true if valid, false if not
        bool dirty;           // true if "dirty" (if write back and has been modified), false if unmodified
};


#endif