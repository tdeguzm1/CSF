/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "set.h"

set::set(unsigned new_index, cache_stats stats){
    index = new_index;
    myStats = stats;

    for (unsigned i = 0; i < myStats.num_slots; i++){
        slots[i] = slot();
    }
}

bool set::contains(unsigned target_tag) {
    if (slots.find(target_tag) != slots.end()) {
        return true;
    }
    return false;
}

void set::insert(unsigned new_tag, unsigned time) {
   this->remove();
   slots[new_tag] = slot(new_tag, time, myStats);

}

void set::remove() {
   unsigned min_time = 4294967295; // corresponds to max unsigned value
   unsigned slot_address;
   for (std::map<unsigned, slot>::iterator it = slots.begin(); it != slots.end(); it++) {
        if (!it->second.isValid()) {
            slots.erase(it->first);
            //std::cout << "empty slot should be removed" << std::endl;
            return; // shortcut if there is a not-valid value
        }
        else if (myStats.rem_scheme && it->second.getLoadTime() < min_time) { // fifo - remove the earliest load time
            min_time = it->second.getLoadTime();
            slot_address = it->first;
        }
        else if (!myStats.rem_scheme && it->second.getAccessTime() < min_time) { // lru - remove the earliest access time
            min_time = it->second.getAccessTime();
            slot_address = it->first;
        }
    }


   if (!myStats.w_scheme && slots[slot_address].is_dirty()) { // write back and is dirty
       // TODO: write it back (count cycle time)
       std::cout << "this line should not execute on write-through" << std::endl;
       myStats.mySummary->total_count = myStats.mySummary->total_count + 25*myStats.num_bytes;
   }
   slots.erase(slot_address);
}

void set::update(unsigned target_tag, unsigned time){
    slots[target_tag].update(time);
}

void set::make_dirty(unsigned tag){
    slots[tag].make_dirty();
}

void set::print_set(){
    for(std::map<unsigned, slot>::iterator it = slots.begin(); it != slots.end(); it++) {
        std::cout << it->first << ", " << it->second.getTag() << ", " << it->second.isValid() << std::endl;
    }
}