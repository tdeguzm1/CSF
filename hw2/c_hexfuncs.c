/*
 * Code to implement hexdump
 * CSF Assignment 2 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
//
// Parameters:
//     char[] data_buf - a string to put read bytes into
//              Pre: data_buf must be >= 17 bytes
// Returns:
//     number of bytes read into data_buf
unsigned hex_read(char data_buf[]) {
  return read(STDIN_FILENO, data_buf, 16);
}

// Write given nul-terminated string to standard output.
//
// Parameters:
//     char[] s - a string to be written
//              
void hex_write_string(const char s[]){
  size_t i = 0;
  while (s[i] != '\0') {
     if (write(STDOUT_FILENO, &s[i], 1) < 1); // if block handles compiler warning
     i++;
  }
}


// Convert a single integer (0-15) to a single hex char
//
// Parameters:
//     unsigned val - integer to convert (between 0-15)
// Returns:
//     char equivalent of integer value
//   
char to_hex(unsigned val) {
    if (val < 10) {
        return val + 48; // to ascii numeral
    }
    else {
        return val + 87; // to ascii letter (lowercase)
    }
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8 (assume given properly sized sbuf).
//
// Parameters:
//     unsigned offser - integer offset size
//     char sbuf[] - buffer to store offset string in
//   
void hex_format_offset(unsigned offset, char sbuf[]){
  sbuf[8] = '\0';
  for (int i = 7; i >= 0; i--) {
    sbuf[i] = to_hex(offset & 15);
    offset = offset >> 4;
  }
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
//
// Parameters:
//     unsigned char byteval - byte to convert to a hex string
//     char sbuf[] - buffer to store 2-byte hex equivalent string in
//                   Pre: must have >= 3 bytes
//   
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]){
  sbuf[0] = to_hex(byteval >> 4);  // same as / 16
  sbuf[1] = to_hex(byteval & 15);  // same as % 16
  sbuf[2] = '\0';
}

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
//
// Parameters:
//     unsigned char byteval - byte to convert to a printable
// Returns:
//     char that is printable
//   
char hex_to_printable(unsigned char byteval){
   if (byteval < 32 || byteval >= 127){
       return '.';
   }
   return byteval;
}







































////////////////////////////
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
}

void print_hex_offset(unsigned num_buffs) {
  char offset_buff[100];
  hex_format_offset(num_buffs*16, offset_buff);    
  hex_write_string(offset_buff);
  hex_write_string(": ");
}