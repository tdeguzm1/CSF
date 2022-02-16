// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
//#include <stdio.h>
#include "hexfuncs.h"

// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[]){
  unsigned count = 0;
  char c;
  while (count < 16 && read(STDIN_FILENO, &c, 1) > 0) {
      data_buf[count] = c;
      //printf("read %c ", data_buf[count]);
      count++;
  }
  data_buf[count] = '\0';
  //printf("numread %d ", count);
  return count;

}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]){
  //printf("attempt to write %s \n", s);
  size_t i = 0;
  while (s[i] != '\0') {
     if (write(STDOUT_FILENO, &s[i], 1) <= 0); // to get rif of warning
     i++;
  }
}

char to_hex(unsigned val) {
    if (val < 10) {
        return val + 48;
    }
    else {
        return val + 87;
    }
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8 (assume given properly sized sbuf).
void hex_format_offset(unsigned offset, char sbuf[]){
  sbuf[8] = '\0';
  for (int i = 7; i >= 0; i--) {
    sbuf[i] = to_hex(offset % 16);
    offset /= 16;
  }
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]){
  sbuf[0] = to_hex(byteval / 16);
  sbuf[1] = to_hex(byteval % 16);
  sbuf[2] = '\0';
}

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval){
   if (byteval < 32 || byteval == 127 || byteval > 254){
       return '.';
   }
   return byteval;
}
