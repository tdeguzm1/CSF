/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#ifndef SLOT_H
#define SLOT_H

#include "cache_stats.h"


class slot {
    public: 
        slot();
        slot(unsigned new_tag, unsigned new_time, cache_stats stats);
        bool isValid();
        unsigned getTag();
        unsigned getAccessTime();
        unsigned getLoadTime();
        void update(unsigned time);
        void make_dirty();
        bool is_dirty();

    private:
        cache_stats myStats;  // information about cache
        unsigned tag;         // tag
        unsigned load_ts;     //  loaded timestamp
        unsigned access_ts;   // accessed timestamp
        bool valid;           // true if valid, false if not
        bool dirty;           // true if "dirty" (if write back and has been modified), false if unmodified
};


#endif