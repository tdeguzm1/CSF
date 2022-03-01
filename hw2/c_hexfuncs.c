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
unsigned hex_read(char data_buf[]){
  unsigned count = 0;
  char c;
  while (count < 16 && read(STDIN_FILENO, &c, 1) > 0) {
      data_buf[count] = c;
      count++;
  }
  data_buf[count] = '\0';
  return count;
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
    sbuf[i] = to_hex(offset % 16);
    offset /= 16;
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
  sbuf[0] = to_hex(byteval / 16);
  sbuf[1] = to_hex(byteval % 16);
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
