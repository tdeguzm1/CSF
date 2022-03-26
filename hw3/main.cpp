/*
 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */


#include <iostream> // C++ standard library for input/outpit
#include <string.h> // C standard library for parsing inputs

// custom created objects
#include "slot.h"
#include "cache.h"


void check_valid_inputs(int num_args, char* input_args[]);
// passsed with arguments ([1] num_sets, [2] block_per_set,  [3] bytes_per_block, [4] write_allocation, [5] write_scheme, [6] removal_scheme)


// main driving methods
int main(int argc, char* argv[]) {
  check_valid_inputs(argc, argv);
  cache myCache = cache(argv);

  std::string ls;
  std::string address;
  std::string offset;
  while (std::cin >> ls && std::cin >> address) {
    std::cin >> offset; // remove "random" other value from end of input
    myCache.load_store(ls, address);
    //myCache.print_current();
  }
  myCache.print_summary();
  
} // 9 lines



/*
 *  Checks inputs to see if they are valid.
 *  Note that (x & (x-1)) checks if a value is a power of 2
 *
 *  Parameter:
 *  num_args - number of arguments input
 *  input_args - array of strings containing input arguments
 */
void check_valid_inputs(int num_args, char* input_args[]){
  if (num_args != 7) {
      std::cerr << "Invalid number of parameters given" << std::endl;
      throw -1;
  }
  else if(!(atoi(input_args[1]) > 0 && (atoi(input_args[1]) & (atoi(input_args[1]) - 1)) == 0)){
      std::cerr << "Invalid number of sets" << std::endl;
      throw 1;
  }
  else if(!(atoi(input_args[2]) > 0 && (atoi(input_args[2]) & (atoi(input_args[2]) - 1)) == 0)){
      std::cerr << "Invalid number of blocks per set" << std::endl;
      throw 2;
  }
  else if(!(atoi(input_args[3]) >= 4 && (atoi(input_args[3]) & (atoi(input_args[3]) - 1)) == 0)){
      std::cerr << "Invalid number of bytes per block" << std::endl;
      throw 3;
  }
  else if(!(strcmp(input_args[4], "write-allocate") == 0 || strcmp(input_args[4], "no-write-allocate") == 0)){
      std::cerr << "Invalid write allocation parameter" << std::endl;
      throw 4;
  }
  else if(!(strcmp(input_args[5], "write-through") == 0 || strcmp(input_args[5], "write-back") == 0)){
      std::cerr << "Invalid write scheme parameter" << std::endl;
      throw 5;
  }
  else if(!(strcmp(input_args[6], "lru") == 0 || strcmp(input_args[6], "fifo") == 0)){
      std::cerr << "Invalid data removal scheme parameter" << std::endl;
      throw 6;
  }
  return;

}  // 19 lines