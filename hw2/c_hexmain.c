// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO: implement the main function
  char buff[17];
  char offset_buff[9];
  char char_buff[4];
  char* one_space = " ";
  char* two_space = "  ";
  char* colon = ":";
  char* newline = "\n";
  unsigned num_buffs = 0;
  unsigned chars_read;
  chars_read = hex_read(buff);
  while (chars_read > 0) {
    //hex_write_string(newline);
    //chars_read = hex_read(buff);
    hex_format_offset(num_buffs*16, offset_buff);
    hex_write_string(offset_buff);
    hex_write_string(colon);
    hex_write_string(one_space);
    num_buffs++;
    unsigned i = 0;
    while(buff[i] != '\0'){
      hex_format_byte_as_hex(buff[i], char_buff);
      hex_write_string(char_buff);
      hex_write_string(one_space);
      i++;
    }
    while (i < 16) {
       hex_write_string(two_space);
       hex_write_string(one_space);
       i++;
    }
    hex_write_string(one_space);
    i = 0;
    char c[2];
    while(buff[i] != '\0'){
      c[0] = hex_to_printable(buff[i]);
      c[1] = '\0';
      hex_write_string(c);
      i++;
    }
    hex_write_string(newline);
    if (chars_read < 16){
      chars_read = 0;
    }
    else {
      chars_read = hex_read(buff);
    }

    
  }
  
}
