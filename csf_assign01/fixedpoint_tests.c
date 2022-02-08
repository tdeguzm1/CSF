/*
 * Code to test fixedpoint implementations
 * CSF Assignment 1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint minMagnitude;
  Fixedpoint max;
  Fixedpoint min;
  Fixedpoint negMinMagnitude;
  Fixedpoint halfMax;
  Fixedpoint negativeHalfMax;
  Fixedpoint ten;
  Fixedpoint nineAndThreeQuarters;
  Fixedpoint abcdef;

} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_double(TestObjs *objs);
void test_half(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
void test_is_overflow_neg(TestObjs *objs);
void test_is_underflow_neg(TestObjs *objs);
void test_is_underflow_pos(TestObjs *objs);
void test_compare(TestObjs *objs);


// main execution of the function
int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_double);
  TEST(test_half);
  TEST(test_is_overflow_pos);
  TEST(test_is_overflow_neg);
  TEST(test_is_underflow_pos);
  TEST(test_is_underflow_neg);
  TEST(test_compare);
  TEST(test_is_err);

  TEST_FINI();
}


//This function sets up test fixedpoints before each test
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->minMagnitude = fixedpoint_create2(0x0UL, 0x0000000000000001UL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);
  objs->min = fixedpoint_negate(objs->max);
  objs->negMinMagnitude = fixedpoint_negate(objs->minMagnitude);
  objs->halfMax = fixedpoint_create(0x8000000000000000UL);
  objs->negativeHalfMax = fixedpoint_negate(objs->halfMax);
  objs->ten = fixedpoint_create(0xaUL);
  objs->nineAndThreeQuarters = fixedpoint_create2(0x9UL, 0xc000000000000000UL);
  objs->abcdef = fixedpoint_create2(0xabcdefUL, 0xabcdef0000000000UL);

  return objs;
}

/* This function cleans up dynamically allocated memory after each step
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void cleanup(TestObjs *objs) {
  free(objs);
}

/* This function tests the fixedpoint_whole_part function
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));

  //added tests
  ASSERT((0x0UL) == fixedpoint_whole_part(objs->minMagnitude));
  ASSERT((0xffffffffffffffffUL) == fixedpoint_whole_part(objs->max));
  ASSERT((0xffffffffffffffffUL) == fixedpoint_whole_part(objs->min));
  ASSERT((0x8000000000000000UL) == fixedpoint_whole_part(objs->halfMax));
  ASSERT((0x8000000000000000UL) == fixedpoint_whole_part(objs->negativeHalfMax));
  ASSERT((0xaUL) == fixedpoint_whole_part(objs->ten));
  ASSERT((0x9UL) == fixedpoint_whole_part(objs->nineAndThreeQuarters));
  ASSERT((0xabcdefUL) == fixedpoint_whole_part(objs->abcdef));

}

/* This function tests the fixedpoint_frac_part function
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));

  //added tests
  ASSERT((0x1UL) == fixedpoint_frac_part(objs->minMagnitude));
  ASSERT((0xffffffffffffffffUL) == fixedpoint_frac_part(objs->max));
  ASSERT((0xffffffffffffffffUL) == fixedpoint_frac_part(objs->min));
  ASSERT((0x0UL) == fixedpoint_frac_part(objs->halfMax));
  ASSERT((0x0UL) == fixedpoint_frac_part(objs->negativeHalfMax));
  ASSERT((0x0UL) == fixedpoint_frac_part(objs->ten));
  ASSERT((0xc000000000000000UL) == fixedpoint_frac_part(objs->nineAndThreeQuarters));
  ASSERT((0xabcdef0000000000UL) == fixedpoint_frac_part(objs->abcdef));
}

/* This function tests the fixedpoint_create_from_hex function
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_create_from_hex(TestObjs *objs) {
  (void) objs;
  
  // Given test
  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
  ASSERT(!fixedpoint_is_neg(val1)); // added this test

  // Added test: negative number
  Fixedpoint val2 = fixedpoint_create_from_hex("-4cc.755e22c7208d5");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x4ccUL == fixedpoint_whole_part(val2));
  ASSERT(0x755e22c7208d5000UL == fixedpoint_frac_part(val2));
  ASSERT(fixedpoint_is_neg(val2));

  // Added test: positive max
  val2 = fixedpoint_create_from_hex("ffffffffffffffff.ffffffffffffffff");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xffffffffffffffffUL == fixedpoint_whole_part(val2));
  ASSERT(0xffffffffffffffffUL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: negative max
  val2 = fixedpoint_create_from_hex("-ffffffffffffffff.ffffffffffffffff");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xffffffffffffffffUL == fixedpoint_whole_part(val2));
  ASSERT(0xffffffffffffffffUL == fixedpoint_frac_part(val2));
  ASSERT(fixedpoint_is_neg(val2));

  // Added test: positive min
  val2 = fixedpoint_create_from_hex("0.0000000000000001");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x0UL == fixedpoint_whole_part(val2));
  ASSERT(0x0000000000000001UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: negative min
  val2 = fixedpoint_create_from_hex("-0.0000000000000001");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x0UL == fixedpoint_whole_part(val2));
  ASSERT(0x0000000000000001UL == fixedpoint_frac_part(val2));
  ASSERT(fixedpoint_is_neg(val2));

  // Added test: positive zero
  val2 = fixedpoint_create_from_hex("0.0");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x0UL == fixedpoint_whole_part(val2));
  ASSERT(0x0UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: negative zero (should still be positive)
  val2 = fixedpoint_create_from_hex("-0.0");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x0UL == fixedpoint_whole_part(val2));
  ASSERT(0x0UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: just a positive int
  val2 = fixedpoint_create_from_hex("abcdef");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xabcdefUL == fixedpoint_whole_part(val2));
  ASSERT(0x0UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: just a negative positive int
  val2 = fixedpoint_create_from_hex("-abcdef");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xabcdefUL == fixedpoint_whole_part(val2));
  ASSERT(0x0UL == fixedpoint_frac_part(val2));
  ASSERT(fixedpoint_is_neg(val2));

  // Added test: mixed case
  val2 = fixedpoint_create_from_hex("AbCdEf.AbCdEf");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xabcdefUL == fixedpoint_whole_part(val2));
  ASSERT(0xabcdef0000000000UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: trailing decimal
  val2 = fixedpoint_create_from_hex("AbCdEf.");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xabcdefUL == fixedpoint_whole_part(val2));
  ASSERT(0x0UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: negative trailing decimal
  val2 = fixedpoint_create_from_hex("AbCdEf.");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xabcdefUL == fixedpoint_whole_part(val2));
  ASSERT(0x0UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: leading decimal
  val2 = fixedpoint_create_from_hex(".AbCdEf");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x0UL == fixedpoint_whole_part(val2));
  ASSERT(0xabcdef0000000000UL == fixedpoint_frac_part(val2));
  ASSERT(!fixedpoint_is_neg(val2));

  // Added test: negative leading decimal
  val2 = fixedpoint_create_from_hex("-.AbCdEf");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x0UL == fixedpoint_whole_part(val2));
  ASSERT(0xabcdef0000000000UL == fixedpoint_frac_part(val2));
  ASSERT(fixedpoint_is_neg(val2));

}

/* This function tests the fixedpoint_create_from_hex function
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
  
  // added test: min positive magnitude
  s = fixedpoint_format_as_hex(objs->minMagnitude);
  ASSERT(0 == strcmp(s, "0.0000000000000001"));
  free(s);
  
  // added test: min
  s = fixedpoint_format_as_hex(objs->min);
  ASSERT(0 == strcmp(s, "-ffffffffffffffff.ffffffffffffffff"));
  free(s);
  
  // added test: max
  s = fixedpoint_format_as_hex(objs->max);
  ASSERT(0 == strcmp(s, "ffffffffffffffff.ffffffffffffffff"));
  free(s);

  // added test: halfMax
  s = fixedpoint_format_as_hex(objs->halfMax);
  ASSERT(0 == strcmp(s, "8000000000000000"));
  free(s);

  // added test: negativeHalfMax
  s = fixedpoint_format_as_hex(objs->negativeHalfMax);
  ASSERT(0 == strcmp(s, "-8000000000000000"));
  free(s);
  
  // added test: ten
  s = fixedpoint_format_as_hex(objs->ten);
  ASSERT(0 == strcmp(s, "a"));
  free(s);

  // added test: nineAndThreeQuarters
  s = fixedpoint_format_as_hex(objs->nineAndThreeQuarters);
  ASSERT(0 == strcmp(s, "9.c"));
  free(s);
  
  // added test: nineAndThreeQuarters
  s = fixedpoint_format_as_hex(objs->abcdef);
  ASSERT(0 == strcmp(s, "abcdef.abcdef"));
  free(s);

}


/* This function tests the fixedpoint_negate function
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));

  // added tests: negate them back and compare to original
  one_neg = fixedpoint_negate(one_neg);
  one_half_neg = fixedpoint_negate(one_half_neg);
  one_fourth_neg = fixedpoint_negate(one_fourth_neg);
  large1_neg = fixedpoint_negate(large1_neg);
  large2_neg = fixedpoint_negate(large2_neg);
  
  // changed back to positive
  ASSERT(!fixedpoint_is_neg(one_neg));
  ASSERT(!fixedpoint_is_neg(one_half_neg));
  ASSERT(!fixedpoint_is_neg(one_fourth_neg));
  ASSERT(!fixedpoint_is_neg(large1_neg));
  ASSERT(!fixedpoint_is_neg(large2_neg));
  
  // did not change magnitudes
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));


}

/* This function tests the fixedpoint_add
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);

  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));


  //tests added from assignment page
  lhs = fixedpoint_create_from_hex("934.ade8d38a");
  rhs = fixedpoint_create_from_hex("-edef814.21f023189");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xedeeedfUL == fixedpoint_whole_part(sum));
  ASSERT(0x74074f8e90000000UL == fixedpoint_frac_part(sum));
  
  //tests added from assignment page
  lhs = fixedpoint_create_from_hex("8bd.0e34492025065");
  rhs = fixedpoint_create_from_hex("5d7b061d6.034f5d");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0x5d7b06a93UL == fixedpoint_whole_part(sum));
  ASSERT(0x1183a62025065000UL == fixedpoint_frac_part(sum));

  //tests added from assignment page
  lhs = fixedpoint_create_from_hex("-8a6a9f92d72.82a9b99ad4e76");
  rhs = fixedpoint_create_from_hex("-8a93a62c25996.e09875");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x8b1e10cbb8709UL == fixedpoint_whole_part(sum));
  ASSERT(0x63422e9ad4e76000UL == fixedpoint_frac_part(sum));

  // tests added: two positive
  lhs = fixedpoint_create_from_hex("e86271c5bfad.38e");
  rhs = fixedpoint_create_from_hex("cde34789956.8f4cc8aa51");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0xf540a63e5903UL == fixedpoint_whole_part(sum));
  ASSERT(0xc82cc8aa51000000UL == fixedpoint_frac_part(sum));

  // tests added: two negative
  lhs = fixedpoint_create_from_hex("-bcc5c6af3.5e27fb9d3b4259");
  rhs = fixedpoint_create_from_hex("-a175a65369f0.318");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xa18172afd4e3UL == fixedpoint_whole_part(sum));
  ASSERT(0x8fa7fb9d3b425900UL == fixedpoint_frac_part(sum));

  // tests added: (-lhs) + (+rhs)
  lhs = fixedpoint_create_from_hex("-f.a");
  rhs = fixedpoint_create_from_hex("378655f53.c0a928d32c2775");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0x378655f44UL == fixedpoint_whole_part(sum));
  ASSERT(0x20a928d32c277500UL == fixedpoint_frac_part(sum));

  // tests added: (lhs) + (-rhs)
  lhs = fixedpoint_create_from_hex("14ae.350d860da8");
  rhs = fixedpoint_create_from_hex("-46f2ee55a.7f6750c27a");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x46f2ed0acUL == fixedpoint_whole_part(sum));
  ASSERT(0x4a59cab4d2000000UL == fixedpoint_frac_part(sum));

  // tests added: positive overflow -> moved to "test_is_overflow_pos"
  
  // tests added: negative overflow -> moved to "test_is_overflow_neg"

  //Note: addition cannot cause underflow
}

/* This function tests the fixedpoint_sub
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
  
  // added test: two positive
  lhs = fixedpoint_create_from_hex("a088.b");
  rhs = fixedpoint_create_from_hex("f2252f52.14e7");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xf2248ec9UL == fixedpoint_whole_part(diff));
  ASSERT(0x64e7000000000000UL == fixedpoint_frac_part(diff));

  // added test: two negatives
  lhs = fixedpoint_create_from_hex("-3.af0ce5688a");
  rhs = fixedpoint_create_from_hex("-032e8e019295b.5124");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(diff));
  ASSERT(0x32e8e0192957UL == fixedpoint_whole_part(diff));
  ASSERT(0xa2171a9776000000UL == fixedpoint_frac_part(diff));

  // added test: (+lhs) + (-rhs)
  lhs = fixedpoint_create_from_hex("1590b7607ee.8693545");
  rhs = fixedpoint_create_from_hex("-5ecacd558.855c3d1636");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(diff));
  ASSERT(0x15ef822dd47UL == fixedpoint_whole_part(diff));
  ASSERT(0x0bef916636000000UL == fixedpoint_frac_part(diff));

  // added test: (-lhs) + (+rhs)
  lhs = fixedpoint_create_from_hex("-6fe2dad3df.dea5b");
  rhs = fixedpoint_create_from_hex("16f5.b7");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x6fe2daead5UL == fixedpoint_whole_part(diff));
  ASSERT(0x95a5b00000000000UL == fixedpoint_frac_part(diff));

  //Note: that overflow caused by subtraction is handled in the addition section
  //       -> this got moved into seperate overflow sections

  //Note: subtraction cannot cause underflow
}

/* This function tests the fixedpoint_is_overflow_pos
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum, dub;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));


  //added tests (written originally as part of "test_add")
  sum = fixedpoint_add(objs->halfMax, objs->halfMax);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->max, objs->minMagnitude);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->max, objs->max);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_pos(sum));

  //added tests (written originally as part of "test_double")
  dub = fixedpoint_double(objs->max);
  ASSERT(!fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_overflow_pos(dub));

  dub = fixedpoint_double(objs->halfMax);
  ASSERT(!fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_overflow_pos(dub));

  dub = fixedpoint_double(fixedpoint_add(objs->halfMax, objs->minMagnitude));
  ASSERT(!fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_overflow_pos(dub));
}

/* This function tests the fixedpoint_is_overflow_neg
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_is_overflow_neg(TestObjs *objs) {
  Fixedpoint sum, dub;

  //added tests (written originally as part of "test_add")
  sum = fixedpoint_add(objs->negativeHalfMax, objs->negativeHalfMax);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_neg(sum));
  
  sum = fixedpoint_add(objs->min, fixedpoint_negate(objs->one));
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_neg(sum));

  sum = fixedpoint_add(objs->min, objs->negMinMagnitude);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_neg(sum));

  sum = fixedpoint_add(objs->min, objs->min);
  ASSERT(!fixedpoint_is_valid(sum));
  ASSERT(fixedpoint_is_overflow_neg(sum));

  //added tests (written originally as part of "test_double")
  dub = fixedpoint_double(objs->min);
  ASSERT(!fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_overflow_neg(dub));

  dub = fixedpoint_double(objs->negativeHalfMax);
  ASSERT(!fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_overflow_neg(dub));

  dub = fixedpoint_double(fixedpoint_add(objs->negativeHalfMax, objs->negMinMagnitude));
  ASSERT(!fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_overflow_neg(dub));


  
}


/* This function tests the fixedpoint_is_overflow_pos
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));


  // added test: double decimal (seperate)
  Fixedpoint err8 = fixedpoint_create_from_hex("7.0.4");
  ASSERT(fixedpoint_is_err(err8));

  // added test: double decimal (together)
  Fixedpoint err9 = fixedpoint_create_from_hex("7..04");
  ASSERT(fixedpoint_is_err(err9));

  // added test: error right after decimal
  Fixedpoint err10 = fixedpoint_create_from_hex("7.?04");
  ASSERT(fixedpoint_is_err(err10));

  // added test: gibberish
  Fixedpoint err11 = fixedpoint_create_from_hex("dflkhk;agbv;ouncrow;cn W;FNC /K");
  ASSERT(fixedpoint_is_err(err11));

  
}

/* This function tests the fixedpoint_double
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_double(TestObjs *objs) {
  Fixedpoint dub;
  
  // test added: one -> two
  dub = fixedpoint_double(objs->one);
  ASSERT(fixedpoint_is_valid(dub));
  ASSERT(!fixedpoint_is_neg(dub));
  ASSERT(0x2UL == fixedpoint_whole_part(dub));
  ASSERT(0x0UL == fixedpoint_frac_part(dub));

  // test added: 1/4 -> 1/2
  dub = fixedpoint_double(objs->one_fourth);
  ASSERT(fixedpoint_is_valid(dub));
  ASSERT(!fixedpoint_is_neg(dub));
  ASSERT(0 == fixedpoint_compare(dub, objs->one_half));

  // test added: double large1 four times
  dub = fixedpoint_double(fixedpoint_double(fixedpoint_double(fixedpoint_double(objs->large1))));
  ASSERT(fixedpoint_is_valid(dub));
  ASSERT(!fixedpoint_is_neg(dub));
  ASSERT(0x4b19efcea0UL == fixedpoint_whole_part(dub));
  ASSERT(0xec9a1e24180UL == fixedpoint_frac_part(dub));

  // test added: -one -> -two
  dub = fixedpoint_double(fixedpoint_negate(objs->one));
  ASSERT(fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_neg(dub));
  ASSERT(0x2UL == fixedpoint_whole_part(dub));
  ASSERT(0x0UL == fixedpoint_frac_part(dub));

  // test added: -1/4 -> -1/2
  dub = fixedpoint_double(fixedpoint_negate(objs->one_fourth));
  ASSERT(fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_neg(dub));
  ASSERT(0 == fixedpoint_compare(dub, fixedpoint_negate(objs->one_half)));

  // test added: double -large1 four times
  dub = fixedpoint_double(fixedpoint_double(fixedpoint_double(fixedpoint_double(fixedpoint_negate(objs->large1)))));
  ASSERT(fixedpoint_is_valid(dub));
  ASSERT(fixedpoint_is_neg(dub));
  ASSERT(0x4b19efcea0UL == fixedpoint_whole_part(dub));
  ASSERT(0xec9a1e24180UL == fixedpoint_frac_part(dub));

  //overflow handled in test_overflow_pos and test_overflow_neg

}

/* This function tests the fixedpoint_half
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_half(TestObjs *objs) {
  Fixedpoint half;
  
  // test added: two -> one
  half = fixedpoint_halve(fixedpoint_create(2));
  ASSERT(fixedpoint_is_valid(half));
  ASSERT(!fixedpoint_is_neg(half));
  ASSERT(0x1UL == fixedpoint_whole_part(half));
  ASSERT(0x0UL == fixedpoint_frac_part(half));

  // test added: 1/2 -> 1/4
  half = fixedpoint_halve(objs->one_half);
  ASSERT(fixedpoint_is_valid(half));
  ASSERT(!fixedpoint_is_neg(half));
  ASSERT(0 == fixedpoint_compare(half, objs->one_fourth));

  // test added: half large1 four times
  half = fixedpoint_halve(fixedpoint_halve(fixedpoint_halve(fixedpoint_halve(fixedpoint_create2(0x4b19efcea0UL, 0xec9a1e24180UL)))));
  ASSERT(fixedpoint_is_valid(half));
  ASSERT(!fixedpoint_is_neg(half));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(half));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(half));
  
  // test added: -two -> -one
  half = fixedpoint_halve(fixedpoint_negate(fixedpoint_create(2)));
  ASSERT(fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_neg(half));
  ASSERT(0x1UL == fixedpoint_whole_part(half));
  ASSERT(0x0UL == fixedpoint_frac_part(half));

  // test added: -1/2 -> -1/4
  half = fixedpoint_halve(fixedpoint_negate(objs->one_half));
  ASSERT(fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_neg(half));
  ASSERT(0 == fixedpoint_compare(half,fixedpoint_negate(objs->one_fourth)));

  // test added: half large1 four times
  half = fixedpoint_halve(fixedpoint_halve(fixedpoint_halve(fixedpoint_halve(fixedpoint_negate(fixedpoint_create2(0x4b19efcea0UL, 0xec9a1e24180UL))))));
  ASSERT(fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_neg(half));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(half));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(half));
  

  //underflow handled in test_underflow_pos and test_underflow_neg

}

/* This function tests the fixedpoint_is_underflow_pos
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_is_underflow_pos(TestObjs *objs) {
  Fixedpoint half;

  //added tests (written originally as part of "test_half")
  half = fixedpoint_halve(objs->max);
  ASSERT(!fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_underflow_pos(half));

  half = fixedpoint_halve(objs->minMagnitude);
  ASSERT(!fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_underflow_pos(half));

  half = fixedpoint_halve(fixedpoint_add(objs->halfMax, objs->minMagnitude));
  ASSERT(!fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_underflow_pos(half));
}

/* This function tests the fixedpoint_is_underflow_neg
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_is_underflow_neg(TestObjs *objs) {
  Fixedpoint half;

  //added tests (written originally as part of "test_half")
  half = fixedpoint_halve(objs->min);
  ASSERT(!fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_underflow_neg(half));

  half = fixedpoint_halve(objs->negMinMagnitude);
  ASSERT(!fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_underflow_neg(half));

  half = fixedpoint_halve(fixedpoint_add(objs->negativeHalfMax, objs->negMinMagnitude));
  ASSERT(!fixedpoint_is_valid(half));
  ASSERT(fixedpoint_is_underflow_neg(half));

}

/* This function tests the fixedpoint_compare
 * 
 * Parameters:
 *    objs - a struct full of objects to use for test cases
 */
void test_compare(TestObjs *objs) {

  // Test added: (+lhs) ? (+rhs) with integers
  ASSERT(1 == fixedpoint_compare(objs->ten, objs->one));
  ASSERT(-1 == fixedpoint_compare(objs->one, objs->ten));
  ASSERT(0 == fixedpoint_compare(objs->one, objs->one));

  // Test added: (+lhs) ? (+rhs) with fractions
  ASSERT(1 == fixedpoint_compare(objs->one_half, objs->one_fourth));
  ASSERT(-1 == fixedpoint_compare(objs->one_fourth, objs->one_half));
  ASSERT(0 == fixedpoint_compare(objs->one_fourth, objs->one_fourth));

  // Test added: (-lhs) ? (-rhs) with integers
  ASSERT(-1 == fixedpoint_compare(fixedpoint_negate(objs->ten), fixedpoint_negate(objs->one)));
  ASSERT(1 == fixedpoint_compare(fixedpoint_negate(objs->one), fixedpoint_negate(objs->ten)));
  ASSERT(0 == fixedpoint_compare(fixedpoint_negate(objs->one), fixedpoint_negate(objs->one)));

  // Test added: (-lhs) ? (-rhs) with fractions
  ASSERT(-1 == fixedpoint_compare(fixedpoint_negate(objs->one_half), fixedpoint_negate(objs->one_fourth)));
  ASSERT(1 == fixedpoint_compare(fixedpoint_negate(objs->one_fourth), fixedpoint_negate(objs->one_half)));
  ASSERT(0 == fixedpoint_compare(fixedpoint_negate(objs->one_fourth), fixedpoint_negate(objs->one_fourth)));

  // Test added: (+lhs) ? (+rhs) with mixed numbers
  ASSERT(1 == fixedpoint_compare(fixedpoint_add(objs->ten, objs->one_half), fixedpoint_add(objs->ten, objs->one_fourth)));
  ASSERT(-1 == fixedpoint_compare(fixedpoint_add(objs->ten, objs->one_fourth), fixedpoint_add(objs->ten, objs->one_half)));
  ASSERT(0 == fixedpoint_compare(fixedpoint_add(objs->ten, objs->one_half), fixedpoint_add(objs->ten, objs->one_half)));

  // Test added: sign-based comparison
  ASSERT(1 == fixedpoint_compare(objs->max, objs->min));
  ASSERT(-1 == fixedpoint_compare(objs->min, objs->max));

}
