/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "slot.h"

slot::slot(unsigned new_tag, cache_stats stats){
    tag = new_tag;
    myStats = stats;
    load_ts = 0;
    access_ts = 0;
    valid = false;
    dirty = false;

}

bool slot::isValid() {
    return valid;
}

unsigned slot::getTag() {
    return tag;
}

unsigned slot::getAccessTime() {
    return access_ts;
}
unsigned slot::getLoadTime() {
    return load_ts;
}

void slot::update(unsigned time){
    access_ts = time;
}

void slot::place_value(unsigned new_tag, unsigned time){
    tag = new_tag;
    valid = true;
    load_ts = time;
    access_ts = time;
}