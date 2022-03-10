#ifndef SLOT_H
#define SLOT_H

#include <vector>


class slot {
    public: 
        slot(unsigned new_tag, unsigned num_bytes);

    private:
        unsigned size; // number of bytes
        unsigned tag;
        unsigned load_ts;
        unsigned access_ts;
        bool valid;
        bool dirty;
};


#endif