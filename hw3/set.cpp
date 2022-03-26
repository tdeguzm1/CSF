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

    for (unsigned i = 0; i < myStats.num_slots; i++){
        slots[i] = slot(); // create a map of slots
    }
} // 4 lines


/**
 *  Checks if this set contains the target tag
 *  
 *  Parameters:
 *  target_tag - the tag to check if its in the set
 */
bool set::contains(unsigned target_tag) {
    // must be in the set's map and valid
    if (slots.find(target_tag) != slots.end() && slots[target_tag].isValid()) {
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
} // 2 lines


/**
 *  Removes a value to make room (by fifo or lru as appropriate)
 */
void set::remove() {
   unsigned min_time = 4294967295; // corresponds to the maximum unsigned value
   unsigned tag;
   for (std::map<unsigned, slot>::iterator it = slots.begin(); it != slots.end(); it++) {
        if (!it->second.isValid()) {
            slots.erase(it->first);
            return; // shortcut if there is a not-valid value (aka slot is empty)
        }

        // fifo - remove the earliest load time
        else if (myStats.rem_scheme && it->second.getLoadTime() < min_time) { 
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
       myStats.mySummary->total_count = myStats.mySummary->total_count + 25*myStats.num_bytes; // takes 100 cycles per 4 bytes to write to memory
   }

   // std::cout << "Removed tag with load time: " << slots[tag].getLoadTime() << std::endl;
   slots.erase(tag); // remove slot from cache
   
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