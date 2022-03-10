#ifndef SET_H
#define SET_H

#include "slot.h"

class set {
    public: 
        set(unsigned index, unsigned num_slots, unsigned num_bytes);

    private:
        unsigned index;
        unsigned size; // number of slots
        std::vector<slot> slots;
};


#endif