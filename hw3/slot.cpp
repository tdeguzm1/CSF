/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "slot.h"

/**
 *  Default constructor for the slot (called when constructing the cache)
 *  Cache's slots initally contain invalid data
 */
slot::slot(){
    tag = 0;
    load_ts = 0;
    access_ts = 0;
    valid = false;
    dirty = false;
}

/**
 *  Constructor for a slot, used when inserting a new value into a set
 *
 *  Parameters:
 *  new_tag - the tag that is being inserted into the set
 *  new_time - the current time of the cache (for setting load/access time) 
 *  stats - struct of information about the overall cache for the slot to see
 */
slot::slot(unsigned new_tag, unsigned new_time, cache_stats stats) {
    tag = new_tag;
    myStats = stats;
    load_ts = new_time;
    access_ts = new_time;
    valid = true;
    dirty = false;

}

/**
 *  Returns true when slot is valid, false when it is not
 */
bool slot::isValid() {
    return valid;
}

/**
 *  Returns unsigned tag of slot
 */
unsigned slot::getTag() {
    return tag;
}


/**
 *  Returns unsigned most recent access timestamp
 */
unsigned slot::getAccessTime() {
    return access_ts;
}

/**
 *  Returns unsigned of original load timestamp
 */
unsigned slot::getLoadTime() {
    return load_ts;
}

/** 
 *  Updates slot's access time with current cache time
 */
void slot::update(unsigned time){
    access_ts = time;
}

/** 
 *  Updates slot's dirty tag to be dirty
 */
void slot::make_dirty(){
    dirty = true;
}


/**
 *  Returns true when slot is dirty, false when it is not
 */
bool slot::is_dirty(){
    return dirty;
}
