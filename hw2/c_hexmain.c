/*
 * Code to implement hexdump
 * CSF Assignment 2 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

// C implementation of hexdump main function
// Takes input from the user or from file, and prints out
// a hexadecimal equivalent, 16 bytes at a time


#include "hexfuncs.h"  // this is the only header file which may be included!
#include <stdio.h>

void print_hex_equivalent(char* buff, const unsigned chars_read); // main helper function
void reprint_ascii_form(char* buff, const unsigned chars_read); // main helper function

int main(void) {
  char buff[100];
  //char offset_buff[100];
  unsigned num_buffs = 0;
  unsigned chars_read = hex_read(buff);
  
  // main implementation loop
  while (chars_read > 0) {
    
    // format and write offset
    print_hex_offset(num_buffs);
    //hex_format_offset(num_buffs*16, offset_buff);    
    //hex_write_string(offset_buff);
    //hex_write_string(": ");
    num_buffs++;
    
    // print the hex conversion and ascii equivalent
    print_hex_equivalent(buff, chars_read);
    reprint_ascii_form(buff, chars_read);
    
    // detect end of input
    if (chars_read < 16){
      break;
    }
    chars_read = hex_read(buff);
  }
}
/*
// Prints the hex equivalent of a 16 byte input buffer
//
// Parameters:
//   *char buff - pointer to a ascii string
//   unsigned chars_read - number of items in the buffer
//
void print_hex_equivalent(char* buff, const unsigned chars_read) {
  unsigned i = 0;
  char char_buff[100];
  while(i < chars_read){
    hex_format_byte_as_hex(buff[i], char_buff);
    hex_write_string(char_buff);
    hex_write_string(" ");
    i++;
  }
  while (i < 16) {
    hex_write_string("   ");
    i++;
  }
  hex_write_string(" ");
}


// Prints the ascii form of a 16 byte input buffer
//
// Parameters:
//   *char buff - pointer to a ascii string
//   unsigned chars_read - number of items in the buffer
//
void reprint_ascii_form(char* buff, const unsigned chars_read) {
  char c[100];
  for (unsigned i = 0; i < chars_read; i++){
    c[0] = hex_to_printable(buff[i]);
    c[1] = '\0';
    hex_write_string(c);
  }
  hex_write_string("\n");
}*/