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
        slots.push_back(slot(i, myStats));
    }
}

int set::find(unsigned target_tag) {
   for (unsigned i = 0; i < myStats.num_slots; i++) {
       if (slots[i].getTag() == target_tag) {
           return i;
       }
   }
   return -1;
}

void set::insert(unsigned new_tag, unsigned time) {
   for (unsigned i = 0; i < myStats.num_slots; i++) {
       if (!slots[i].isValid()) {
           slots[i].place_value(new_tag, time);
           return;
       }
   }
   
   unsigned my_index = remove();
   slots[my_index].place_value(new_tag, time);

}

unsigned set::remove(){
   unsigned min_time = (unsigned) -1; // corresponds to max unsigned value
   unsigned index;
   if (myStats.rem_scheme) { // fifo
      for (unsigned i = 0; i < myStats.num_slots; i++) {
          if (slots[i].getLoadTime() < min_time) {
              index = i;
          }
      }
   }
   else {  // lru
      for (unsigned i = 0; i < myStats.num_slots; i++) {
          if (slots[i].getAccessTime() < min_time) {
              index = i;
          }
      }
   }

   if (!myStats.w_scheme) { // write back
       // TODO: write it back (count cycle time)
   }
   return index;
}

void set::update(unsigned target_tag, unsigned time){
    for (unsigned i = 0; i < myStats.num_slots; i++) {
        if (slots[i].getTag() == target_tag) {
           slots[i].update(time);
           return;
        }
    }
}
