/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "cache.h"

cache::cache(char* input_args[]) {
    timer = 0;
    myStats.num_sets = (unsigned) atoi(input_args[1]);
    myStats.num_slots = (unsigned) atoi(input_args[2]);
    myStats.num_bytes = (unsigned) atoi(input_args[3]);
    myStats.w_alloc = (strcmp(input_args[4], "write-allocate") == 0) ? 0 : 1;     // 0 if write-allocate, 1 if no-write-allocate
    myStats.w_scheme = (strcmp(input_args[5], "write-back") == 0) ? 0 : 1;       // 0 if write-back, 1 if write-through
    myStats.rem_scheme = (strcmp(input_args[6], "lru") == 0) ? 0 : 1;          // 0 id LRU, 1 if FIFO
    myStats.mySummary = new cache_summary;
    
    if (myStats.w_alloc != myStats.w_scheme) {
        std::cerr << "Invalid combination of write states" << std::endl;
        throw -2;
    }

    for (unsigned i = 0; i < myStats.num_sets; i++){
        sets.push_back(set(i, myStats));
    }
}


void cache::load_store(std::string ls, std::string addr){
    unsigned address = std::stoul(addr, nullptr, 16);
    if ("l" == (ls)){
        cache_load(address);
    }
    else if ("s" == (ls)) {
        cache_store(address);
    }
    else {
        std::cerr << "Invalid load/store tag" << std::endl;
        throw -3;
    }
    return; 
}

void cache::cache_load(unsigned address) {
    myStats.mySummary->loads++;

    // convert to block number
    unsigned block_number = address - (address % myStats.num_bytes);

    // convert to index
    unsigned index = (block_number / myStats.num_bytes) % myStats.num_sets;

    // insert into set
    int set_position = sets[index].find(block_number);
    if (set_position >= 0) {
        myStats.mySummary->load_hits++;
        // cache hit
        // TODO: account for cycle time
        // update access timestamp
        sets[index].update(block_number, timer);
    } 
    else {
        myStats.mySummary->load_misses++;
        // chache miss
        sets[index].insert(block_number, timer); 
    }

    timer++;  

    // TODO
    return;
}

void cache::cache_store(unsigned address) {
    myStats.mySummary->stores++;

    // convert to block number
    unsigned block_number = address - (address % myStats.num_bytes);

    // convert to index
    unsigned index = (block_number / myStats.num_bytes)  % myStats.num_sets;

    // insert into set
    int set_position = sets[index].find(block_number);
    if (set_position >= 0) {
        myStats.mySummary->store_hits++;
        // cache hit
        write_cache_on_hit(block_number, index);
    } 
    else {
        myStats.mySummary->store_misses++;
        // chache miss
        write_cache_on_miss(block_number, index); 
    }

    timer++;  
    return;
}

void cache::write_cache_on_hit(unsigned block_number, unsigned index) {
    // update access timestamp
    sets[index].update(block_number, timer);
    if (myStats.w_scheme) {
        // write-through --> access cache and access memory
    }
    else { // write-back --> access chache only
        // update
    }
    return;
}

void cache::write_cache_on_miss(unsigned block_number, unsigned index) {
    if (myStats.w_alloc) {
        // no-write-allocate --> access memory only
    }
    else { // write-allocate --> access cache and memory
        sets[index].insert(block_number, timer); 

        timer++;
    }
    return;
}

void cache::print_summary(){
    std::cout << "Total loads: " << myStats.mySummary->loads << std::endl;
    std::cout << "Total stores: " << myStats.mySummary->stores << std::endl;
    std::cout << "Load hits: " << myStats.mySummary->load_hits << std::endl;
    std::cout << "Load misses: " << myStats.mySummary->load_misses << std::endl;
    std::cout << "Store hits: " << myStats.mySummary->store_hits << std::endl;
    std::cout << "Store misses: " << myStats.mySummary->store_misses << std::endl;
    std::cout << "Total cycles: " << myStats.mySummary->total_count << std::endl;

    delete myStats.mySummary;
    return;
}





