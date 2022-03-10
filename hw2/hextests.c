/*
 * Code to implement hexdump
 * CSF Assignment 2 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
void testHexRead(TestObjs *objs);
void testHexWrite(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);
  //TEST(testHexRead);
  //TEST(testHexWrite);

  TEST_FINI();

  return 0;
}

// these were used in development to test I/O operations
void testHexRead(TestObjs *objs) {
  (void) objs;
  char buf[17];

  int i = hex_read(buf);
  buf[i] = 0;
  printf("%s\n", buf);

}

// these were used in development to test I/O operations
void testHexWrite(TestObjs *objs) {
  (void) objs;
  char* buf = "Hello there!\n";

  hex_write_string(buf);

}

// These functions test the hex_format_offset
void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0, buf);
  ASSERT(0 == strcmp(buf, "00000000"));
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
  hex_format_offset(2L, buf);
  ASSERT(0 == strcmp(buf, "00000002"));
  hex_format_offset(0xaL, buf);
  ASSERT(0 == strcmp(buf, "0000000a"));
  hex_format_offset(0x10L, buf);
  ASSERT(0 == strcmp(buf, "00000010"));
  hex_format_offset(0x30L, buf);
  ASSERT(0 == strcmp(buf, "00000030"));
  hex_format_offset(0x1000aL, buf);
  ASSERT(0 == strcmp(buf, "0001000a"));
  hex_format_offset(0xabcdef01UL, buf);
  ASSERT(0 == strcmp(buf, "abcdef01"));
  hex_format_offset(0xffffffffUL, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
}

// These functions test the hex_format_byte_as_hex
void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  
  //Hello,
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65"));
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));

  // world!
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20"));
  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77")); // w
  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f")); // o
  hex_format_byte_as_hex(objs->test_data_1[9], buf);
  ASSERT(0 == strcmp(buf, "72")); // r
  hex_format_byte_as_hex(objs->test_data_1[10], buf);
  ASSERT(0 == strcmp(buf, "6c")); // l
  hex_format_byte_as_hex(objs->test_data_1[11], buf);
  ASSERT(0 == strcmp(buf, "64")); // d
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21")); // !
  
  hex_format_byte_as_hex(objs->test_data_1[13], buf);
  ASSERT(0 == strcmp(buf, "0a")); // \n
  
  hex_format_byte_as_hex(objs->test_data_1[14], buf);
  ASSERT(0 == strcmp(buf, "00")); // \0


  // now test passing non-char values
  hex_format_byte_as_hex(1, buf);
  ASSERT(0 == strcmp(buf, "01"));
  hex_format_byte_as_hex(12, buf);
  ASSERT(0 == strcmp(buf, "0c"));
  hex_format_byte_as_hex(94, buf);
  ASSERT(0 == strcmp(buf, "5e"));
  hex_format_byte_as_hex(255, buf);
  ASSERT(0 == strcmp(buf, "ff"));

}


void testHexToPrintable(TestObjs *objs) {
  
  // alphabetic characters
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[3]));
  ASSERT(',' == hex_to_printable(objs->test_data_1[5]));
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
  
  // numeric characters
  ASSERT('1' == hex_to_printable('1'));
  ASSERT('0' == hex_to_printable('0'));
  ASSERT('9' == hex_to_printable('9'));

  // non alpha-numeric charcters
  ASSERT('.' == hex_to_printable('\t'));
  ASSERT('.' == hex_to_printable('\0'));
  ASSERT('.' == hex_to_printable('\n'));
  ASSERT('@' == hex_to_printable('@'));
  ASSERT('*' == hex_to_printable('*'));

  // pass non-char values
  ASSERT('.' == hex_to_printable(0));
  ASSERT('.' == hex_to_printable(1));
  ASSERT('a' == hex_to_printable(97));
  ASSERT('.' == hex_to_printable(255));
  ASSERT('.' == hex_to_printable(128));
  ASSERT('.' == hex_to_printable(31));

}
