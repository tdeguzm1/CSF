/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include "cache.h"


/**
 *  Constructor for cache object.
 *
 *  Parameters:
 *  input_args - the values input to create a cache
 *                 Pre: input arguments have already been validated
 */
cache::cache(char* input_args[]) {
    timer = 0;
    myStats.num_sets = (unsigned) atoi(input_args[1]);
    myStats.num_slots = (unsigned) atoi(input_args[2]);
    myStats.num_bytes = (unsigned) atoi(input_args[3]);
    myStats.w_alloc = (strcmp(input_args[4], "write-allocate") == 0) ? false : true;     // 0 if write-allocate, 1 if no-write-allocate
    myStats.w_scheme = (strcmp(input_args[5], "write-back") == 0) ? false : true;       // 0 if write-back, 1 if write-through
    myStats.rem_scheme = (strcmp(input_args[6], "lru") == 0) ? false : true;          // 0 id LRU, 1 if FIFO
    myStats.mySummary = new cache_summary;
    
    if (myStats.w_alloc && !myStats.w_scheme) {  // cannot no-write-allocate with write-back
        std::cerr << "Invalid combination of write states" << std::endl;
        throw -2;
    }

    for (unsigned i = 0; i < myStats.num_sets; i++){
        sets.push_back(set(i, myStats));
    }
}  // 13 lines

/**
 *  Destructor for cache object
 *  Cleans up dynamic memory
 */
cache::~cache(){
    delete myStats.mySummary; // will move to destructor for ms3
}


/**
 *  Calls load or store based on input condition
 *
 *  Parameters:
 *  ls - string indicating load or store
 *  addr - string indicating the hexidecimal address
 */
void cache::load_store(std::string ls, std::string addr){
    unsigned address = std::stoul(addr, nullptr, 16);  // parse address from string
   
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
    
    timer++; // to update time - note that this overflows if program has more than 4.2 billion memory calls
    return; 
} // 10 lines


/**
 *  Checks for cache hit/miss and loads appropriately
 *
 *  Parameters:
 *  addr - unsigned indicating the address
 */
void cache::cache_load(unsigned address) {
    myStats.mySummary->loads++; // increase loads counter

    // convert to tag
    unsigned tag = address - (address % myStats.num_bytes);

    // convert to index
    unsigned index = (tag / myStats.num_bytes) % myStats.num_sets;

    if (sets[index].contains(tag)) {       // load hit
        sets[index].update(tag, timer);    // update acess timestamp
        myStats.mySummary->load_hits++;
        myStats.mySummary->total_count++;  // one load from cache takes one count
    } 
    else {  // load miss
        sets[index].insert(tag, timer);    // load value into cache
        myStats.mySummary->load_misses++; 
        myStats.mySummary->total_count = myStats.mySummary->total_count + 25*myStats.num_bytes; // load from memory takes 100 counts per 4 bytes
    }  
    return;
}


/**
 *  Checks for cache hit/miss and stores appropriately
 *
 *  Parameters:
 *  addr - unsigned indicating the address
 */
void cache::cache_store(unsigned address) {
    myStats.mySummary->stores++;  // increase stores counter

    // convert to block number
    unsigned tag = address - (address % myStats.num_bytes);

    // convert to index
    unsigned index = (tag / myStats.num_bytes)  % myStats.num_sets;

    if (sets[index].contains(tag)) {  // store hit
        myStats.mySummary->store_hits++;
        write_cache_on_hit(tag, index);
    } 
    else {  // store miss
        myStats.mySummary->store_misses++;
        write_cache_on_miss(tag, index); 
    }
  
    return;
}  // 10 lines


/**
 *  Handles writes when cache hits occur
 *
 *  Parameters:
 *  tag - unsigned indicating the tag
 *  index - unsidned indicating the index
 */
void cache::write_cache_on_hit(unsigned tag, unsigned index) {
    // update access timestamp and mark block as dirty
    sets[index].update(tag, timer);
    sets[index].make_dirty(tag);
    
    if (myStats.w_scheme) { // write-through --> access cache (4 bytes takes 1 count) & access memory (4 bytes takes 100 counts)
        myStats.mySummary->total_count = myStats.mySummary->total_count + 100 + 1;
    }
    else { // write-back --> access chache (4 bytes takes 1 count)
        myStats.mySummary->total_count++;
    }
    return;
} // 7 lines


/**
 *  Handles writes when cache misses occur
 *
 *  Parameters:
 *  tag - unsigned indicating the tag
 *  index - unsidned indicating the index
 */
void cache::write_cache_on_miss(unsigned tag, unsigned index) {
    if (myStats.w_alloc) { // no-write-allocate --> access memory only
        myStats.mySummary->total_count = myStats.mySummary->total_count + 100; // a 4 byte store takes 100 counts
    }
    else { // write-allocate --> access cache and memory
        sets[index].insert(tag, timer); 
        sets[index].make_dirty(tag);
        myStats.mySummary->total_count = myStats.mySummary->total_count + 25 * myStats.num_bytes + 1;
    }
    return;
} // 7 lines


/**
 *  Prints the current cache state
 */
void cache::print_current(){
    unsigned i = 0;
    for(std::vector<set>::iterator it = sets.begin(); it != sets.end(); it++) {
        std::cout << "Index: " << i << std::endl;
        it->print_set();
        i++;
    }
} // 5 lines


/**
 *  Print the summary of the cache state at the end of the program
 */
void cache::print_summary(){
    std::cout << "Total loads: " << myStats.mySummary->loads << std::endl;
    std::cout << "Total stores: " << myStats.mySummary->stores << std::endl;
    std::cout << "Load hits: " << myStats.mySummary->load_hits << std::endl;
    std::cout << "Load misses: " << myStats.mySummary->load_misses << std::endl;
    std::cout << "Store hits: " << myStats.mySummary->store_hits << std::endl;
    std::cout << "Store misses: " << myStats.mySummary->store_misses << std::endl;
    std::cout << "Total cycles: " << myStats.mySummary->total_count << std::endl;

    return;
} // 9 lines





