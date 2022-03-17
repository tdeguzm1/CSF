/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "slot.h"

slot::slot(){
    tag = 0;
    load_ts = 0;
    access_ts = 0;
    valid = false;
    dirty = false;

}

slot::slot(unsigned new_tag, unsigned new_time, cache_stats stats) {
    tag = new_tag;
    myStats = stats;
    load_ts = new_time;
    access_ts = new_time;
    valid = true;
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

void slot::make_dirty(){
    dirty = true;
}

bool slot::is_dirty(){
    return dirty;
}
