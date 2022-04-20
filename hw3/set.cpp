/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "set.h"


/**
 *  Constuctor for sets
 *  
 *  Parameters:
 *  new_index - provides the set with information about its own index in the cache
 *  stats - a structure providing the set with information about the entire cache
 */
set::set(unsigned new_index, cache_stats stats){
    index = new_index;
    myStats = stats;
    num_slots = 0;
} // 4 lines


/**
 *  Checks if this set contains the target tag
 *  
 *  Parameters:
 *  target_tag - the tag to check if its in the set
 */
bool set::contains(unsigned target_tag) {
    // must be in the set's map and valid
    if (slots.find(target_tag) != slots.end()) { //} && slots[target_tag].isValid()) {
        return true;
    }
    return false;
} // 3 lines


/**
 *  Inserts the target tag into the cache
 *  
 *  Parameters:
 *  target_tag - the tag to check if its in the set
 *  time - the current time state of the cache
 */
void set::insert(unsigned new_tag, unsigned time) {
   this->remove();
   slots[new_tag] = slot(new_tag, time, myStats);
   num_slots++;
} // 2 lines


/**
 *  Removes a value to make room (by fifo or lru as appropriate)
 */
void set::remove() {
   unsigned min_time = 4294967295; // corresponds to the maximum unsigned value
   unsigned tag;
   if (num_slots < myStats.num_slots) {
       return;
   }
   for (std::map<unsigned, slot>::iterator it = slots.begin(); it != slots.end(); it++) {

        // fifo - remove the earliest load time
        if (myStats.rem_scheme && it->second.getLoadTime() < min_time) { 
            min_time = it->second.getLoadTime();
            tag = it->first;
        }

        // lru - remove the earliest access time
        else if (!myStats.rem_scheme && it->second.getAccessTime() < min_time) { 
            min_time = it->second.getAccessTime();
            tag = it->first;
        }
    }


   if (!myStats.w_scheme && slots[tag].is_dirty()) { // if write back and is dirty
       myStats.mySummary->total_count = myStats.mySummary->total_count + COUNTS_4_BYTE_MEMORY/4*myStats.num_bytes; // write back to memory
   }

   // std::cout << "Removed tag with load time: " << slots[tag].getLoadTime() << std::endl;
   slots.erase(tag); // remove slot from cache
   num_slots--;
} // 15 lines


/**
 *  Updates the target tag's access timestamp with current cache time
 *  
 *  Parameters:
 *  target_tag - the tag to update
 *  time - the current time state of the cache
 */
void set::update(unsigned target_tag, unsigned time){
    slots[target_tag].update(time);
}

/**
 *  Updates the target tag's dirty field 
 *  
 *  Parameters:
 *  target_tag - indicates the slot to update
 */
void set::make_dirty(unsigned tag){
    slots[tag].make_dirty();
}

/** 
 *  Prints the current state of the set (for debugging)
 */
void set::print_set(){
    for(std::map<unsigned, slot>::iterator it = slots.begin(); it != slots.end(); it++) {
        std::cout << it->first << ", " << it->second.getTag() << ", " << it->second.isValid() << std::endl;
    }
}