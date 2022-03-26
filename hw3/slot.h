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

        /**
         *  Default constructor for the slot (called when constructing the cache)
         *  Cache's slots initally contain invalid data
         */
        slot();

        /**
         *  Constructor for a slot, used when inserting a new value into a set
         *
         *  Parameters:
         *  new_tag - the tag that is being inserted into the set
         *  new_time - the current time of the cache (for setting load/access time) 
         *  stats - struct of information about the overall cache for the slot to see
         */
        slot(unsigned new_tag, unsigned new_time, cache_stats stats);
        
        /**
         *  Returns true when slot is valid, false when it is not
         */
        bool isValid();
        
        /**
         *  Returns unsigned tag of slot
         */
        unsigned getTag();
        
        /**
         *  Returns unsigned most recent access timestamp
         */
        unsigned getAccessTime();
        
        /**
         *  Returns unsigned of original load timestamp
         */
        unsigned getLoadTime();
        
        /** 
         *  Updates slot's access time with current cache time
         */
        void update(unsigned time);
        
        /** 
         *  Updates slot's dirty tag to be dirty
         */
        void make_dirty();
        
        /**
         *  Returns true when slot is dirty, false when it is not
         */
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