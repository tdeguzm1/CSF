#include "set.h"

set::set(unsigned new_index, unsigned num_slots, unsigned num_bytes){
    index = new_index;
    size = num_slots;

    for (unsigned i = 0; i < size; i++){
        slots.push_back(slot(i, num_bytes));
    }
}