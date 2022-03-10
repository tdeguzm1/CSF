#include "slot.h"

slot::slot(unsigned new_tag, unsigned num_bytes){
    tag = new_tag;
    size = num_bytes;
    load_ts = 0;
    access_ts = 0;
    valid = 0;
    dirty = 0;

}