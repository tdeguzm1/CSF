/*
 * Fixedpoint library implementation 
 * CSF Assignment 1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"


/*
 * Create a fixedpoint value representing an integer
 *
 * Parameters:
 *   whole - the value of the whole paer of the representation
 *
 * Returns:
 *   the Fixedpoint Value
 */
Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint num;
  num.w = whole;
  num.f = 0;
  num.tag = '+';
  return num;
} // 5 lines



// Create a Fixedpoint value from specified whole and fractional values
//
// Parametrs:
//   whole - the value of the whole part of the representation
//   frac - the value of the fractional part of the representation, where
//          the highest bit is the halves (2^-1) place, the second highest
//          bit is the fourths (2^-2) place, etc.
//
// Returns:
//   the Fixedpoint value
Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint num;
  num.w = whole;
  num.f = frac;
  num.tag = '+';
  return num;
} // 5 lines



// Updates final four bits of number, shifting the others left
// based on given hexidecimal char - called by create_from_hex
//
// Parameters:
//   *num - pointer to uint64_t value
//   letter - letter being added
//
void setLast4bits(uint64_t* num, char letter){
  *num = *num << 4; // shift 4 bits left
  
  if (isalpha(letter)){
    letter = tolower(letter);
    *num += (letter - 87); // add hex value if letter 
  }
  else {
    *num += (letter - 48); // add hex value if number
  }
} // 6 lines



// Check if the char being read is valid- called by create_from_hex
//
// Parameters:
//   *val - pointer to fixedpoint being created
//   ch - char being considered for validity
//   count - the number of values aready checked for validity
//
// Returns:
//   1 if valid
//   0 if invalid
int valid_char(Fixedpoint *val, char ch, int count){
  
  if (count > 16); // more than 16 values read-> jump to error
  
  else if (ch == '.' && val->w == 0){
    return 1; // valid, if the char is the first occurence of a decimal point
  }
  else if (isdigit(ch) || (isalpha(ch) && (tolower(ch) >= 'a' && tolower(ch) <= 'f'))){
    return 1; // valid, if the char is a digit or a letter (a-f) or (A-F)
  }
  
  val->tag = '/'; // if in error, set fixedpoint to error state
  return 0;
} // 7 lines
    


// Create a Fixedpoint value from a string representation.
// The string will have one of the following forms:
//
//    X
//    -X
//    X.Y
//    -X.Y
//
// In all value strings, X and Y are sequences of 0 to 16 hex digits
// (chosen from 0-9, a-f, A-F).
//
// Returns:
//   if the string is valid, the Fixedpoint value;
//   if the string is invalid, a Fixedpoint value for which
//   fixedpoint_is_err returns true
Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint num;
  num.w = 0;
  num.f = 0;
  
  // sets +/- designation
  size_t start = 0;
  if (hex[0] == '-'){
    num.tag = '-';
    start = 1;
  }
  else if (valid_char(&num, hex[0], 1)){
    num.tag = '+';
  }
  
  // sets num.w value until decimal or end of string is reached
  size_t i;
  for (i = 0; i+start < strlen(hex) && hex[i+start] != '.'; i++){
    valid_char(&num, hex[i+start], i+1);
    setLast4bits(&num.w, hex[i+start]);
  }
  
  // sets num.f value until decimal or end of string is reached
  size_t j;
  for (j = 1; i+j+start < strlen(hex); j++){
    valid_char(&num, hex[i+j+start], j);
    setLast4bits(&num.f, hex[j+i+start]);
  }
  num.f = num.f << ((16-(j-1))*4); // shift to account for trailing zeroes
  
  num = positive_zero(num); // checks for negative zero
  return num;

} // 20 lines



// Get the whole part of the given Fixedpoint value.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   a uint64_t value which is the whole part of the Fixedpoint value
uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.w;
} // 1 line



// Get the fractional part of the given Fixedpoint value.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   a uint64_t value which is the fractional part of the Fixedpoint value 
uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.f;
} // 1 line



// Compute the sum of two valid Fixedpoint values.
//
// Parameters: (assume valid fixedpoints are passed)
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//   if the sum left + right is in the range of values that can be represented
//   exactly, the sum is returned;
//   if the sum left + right is not in the range of values that can be
//   represented, then a value for which either fixedpoint_is_overflow_pos or
//   fixedpoint_is_overflow_neg returns true is returned (depending on whether
//   the overflow was positive or negative)
Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left) && fixedpoint_is_valid(right)); //check validity

  Fixedpoint sum;

  // if the signs are the same
  if (fixedpoint_is_neg(left) == fixedpoint_is_neg(right)){ 

    // Set sum to + or -
    if (fixedpoint_is_neg(left)){
      sum.tag = '-';
    }
    else {
      sum.tag = '+';
    }

    // add parts together
    sum.w = left.w + right.w;
    sum.f = left.f + right.f;

    // check if fractional part overflows
    if (is_in_overflow(sum.f, left.f, right.f)) {
      sum.w += 1;
    }

    // check if whole fixedpoint overflowed
    if (fixedpoint_is_in_overflow(sum, left, right)){
      sum = to_overflow(sum);
    }
  }
  
  // if (-lhs) + (+rhs), call (+rhs) - (+lhs)
  else if (fixedpoint_is_neg(left)){
    left = fixedpoint_negate(left);
    sum = fixedpoint_sub(right, left);
  }

  // if (lhs) + (-rhs), call (+lhs) - (+rhs)
  else {
    right = fixedpoint_negate(right);
    sum = fixedpoint_sub(left, right);
  }

  sum = positive_zero(sum); // check for negative zero
  return sum;
} // 21 lines



// Check if two added uint64_t values are in overflow. Called by fixedpoint_add
//
// Parameters:
//   sum - a final uint64_t value
//   num1 & num2 - two original values
//
// Returns:
//   1 if in overflow
//   0 if not in overflow
char is_in_overflow(uint64_t sum, uint64_t num1, uint64_t num2){
  if (sum < num1 || sum < num2){
    return 1;
  }
  return 0;
} // 3 lines



// Check if two added fixedpoint values are in overflow. Called by fixedpoint_add
//
// Parameters:
//   sum - a final fixedpoint value
//   num1 & num2 - two original values
//
// Returns:
//   1 if in overflow
//   0 if not in overflow
char fixedpoint_is_in_overflow(Fixedpoint sum, Fixedpoint num1, Fixedpoint num2){
  if (is_in_overflow(sum.w, num1.w, num2.w)){
    return 1; // if whole has overflowed
  }
  else if ((sum.w == num1.w && sum.f < num1.f) || (sum.w == num2.w && sum.f < num2.f)){
    return 1; // if one whole is equal, but overflowed fractionally
  }
  return 0;
} // 5 lines



// Check if magnitude of one fixedpoint is larger than another.
// Used in fixedpoint_subtract and fixedpoint_compare
//
// Parameters:
//   left - main fixedpoint
//   right - fixedpoint for comparison
//
// Returns:
//   1 if left > right
//   0 if left <= right
int fixedpoint_mag_greater_than(Fixedpoint left, Fixedpoint right) {
  if ((left.w > right.w)  || ((left.w == right.w) && (left.f > right.f))){
    return 1;
  }
  return 0;
} // 3 lines



// Compute the difference of two valid Fixedpoint values.
//
// Parameters: (assume valid fixedpoint is passed)
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//   if the difference left - right is in the range of values that can be represented
//   exactly, the difference is returned;
//   if the difference left - right is not in the range of values that can be
//   represented, then a value for which either fixedpoint_is_overflow_pos or
//   fixedpoint_is_overflow_neg returns true is returned (depending on whether
//   the overflow was positive or negative)
Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left) && fixedpoint_is_valid(right)); // checks fixedpoints are valid

  Fixedpoint diff;
  
  // if the fixedpoints' signs are the same
  if (fixedpoint_is_neg(left)  == fixedpoint_is_neg(right)){
    
    // if |right| > |left|, change (left - right) to -(right - left)
    if (fixedpoint_mag_greater_than(right, left)){
      diff = fixedpoint_sub(right, left);
      diff = fixedpoint_negate(diff);
    }

    // if |left| < |right|, subtract normally
    else {

      // set +/-
      if (fixedpoint_is_neg(left)){
        diff.tag = '-';
      }
      else {
        diff.tag = '+';
      }

      // subtract parts
      diff.w = left.w - right.w;
      diff.f = left.f - right.f;
      
      // check if fractional part requires borrowing
      if (right.f > left.f) {
        diff.w -= 1;
      }
    }
  }
  
  // if (-lhs) - (+rhs), change to (-lhs) + (-rhs)
  else if (fixedpoint_is_neg(left)){
    right = fixedpoint_negate(right);
    diff = fixedpoint_add(right, left);
  }

  // if (+lhs) - (-rhs), change to (+lhs) + (+rhs)
  else {
    right = fixedpoint_negate(right);
    diff = fixedpoint_add(left, right);
  }
    
  return diff;
} // 22 lines



// Negate a valid Fixedpoint value.  (I.e. a value with the same magnitude but
// the opposite sign is returned.)  As a special case, the zero value is considered
// to be its own negation.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   the negation of val
Fixedpoint fixedpoint_negate(Fixedpoint val) {
  assert(fixedpoint_is_valid(val)); // checks fixedpoint is valid
  switch (val.tag){
    case '+':
      val.tag = '-';
      break;
    case '-':
      val.tag = '+';
      break;
    default:
      val.tag = '/';
      break;
  }
  val = positive_zero(val); // handles negative zero
  return val;
} // 13 lines



// Return a Fixedpoint value that is exactly 1/2 the value of the given one.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Return:
//   a Fixedpoint value exactly 1/2 of the given one, if it can be represented exactly;
//   otherwise, a Fixedpoint value for which either fixedpoint_is_underflow_pos
//   or fixedpoint_is_underflow_neg returns true (depending on whether the
//   computed value would have been positive or negative)
Fixedpoint fixedpoint_halve(Fixedpoint val) {
  assert(fixedpoint_is_valid(val)); // checks fixedpoint is valid
  
  // if lowest bit of fraction is 1, underflow is triggered
  if (val.f % 2 == 1){
    return to_underflow(val);
  }

  val.f = val.f >> 1; // shift fraction right
  if (val.w % 2 == 1){
    val.f += (((uint64_t)1) << 63); // add from whole part if necessary
  }
  val.w = val.w >> 1; // shift whole right
  return val;
} // 8 lines



// Update a valid, recently-underflowed fixedpoint with appropriate signage.
//
// Parameters:
//   val - an underflowed Fixedpoint value
//
// Return:
//   an updated fixpoint value with +/- underflow tag
Fixedpoint to_underflow(Fixedpoint val){
  if (fixedpoint_is_neg(val)){
    val.tag = 'u';
  }
  else {
    val.tag = 'U';
  }
  return val;
} // 5 lines



// Return a Fixedpoint value that is exactly twice the value of the given one.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Return:
//   a Fixedpoint value exactly twice the given one, if it can be represented exactly;
//   otherwise, a Fixedpoint value for which either fixedpoint_is_overflow_pos
//   or fixedpoint_is_overflow_neg returns true (depending on whether the
//   computed value would have been positive or negative)
Fixedpoint fixedpoint_double(Fixedpoint val) {
  assert(fixedpoint_is_valid(val)); // checks fixedpoint is valid
  
  // ckecks if the fixedpoint will overflow 
  if (val.w >= (((uint64_t)1) << 63)){
    return to_overflow(val);
  }

  val.w = val.w << 1; // shift whole left
  if (val.f >= (((uint64_t)1) << 63)){
    val.w += 1; // add from fraction part if necessary
  }
  val.f = val.f << 1; // shift fraction left
  return val;
} // 8 lines



// Update a valid, recently-overflowed fixedpoint with appropriate signage.
//
// Parameters:
//   val - an overflowed Fixedpoint value
//
// Return:
//   an updated fixpoint value with +/- overflow tag
Fixedpoint to_overflow(Fixedpoint val){
  if (fixedpoint_is_neg(val)){
    val.tag = 'o';
  }
  else {
    val.tag = 'O';
  }
  return val;
} // 5 lines



// Compare two valid Fixedpoint values.
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//    -1 if left < right;
//     0 if left == right;
//     1 if left > right
int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left) && fixedpoint_is_valid(right)); // checks fixedpoint is valid
  
  if (fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)){
    return -1; // -1 because (-lhs) < (+rhs)
  }
  else if (fixedpoint_is_neg(right) && !fixedpoint_is_neg(left)){
    return 1; // 1 because (+lhs) > (-rhs)
  }
  else if (right.w == left.w && left.f == right.f){
    return 0; // 1 because (+lhs) = (+rhs) or (-lhs) = (-rhs)
  }

  // handles when lhs and rhs are both positive
  else if (!fixedpoint_is_neg(right) && !fixedpoint_is_neg(left)){
    if (fixedpoint_mag_greater_than(left, right)){
      return 1; 
    }
    else {
      return -1;
    }
  }

  // handles when lhs and rhs are both negative
  else if (fixedpoint_is_neg(right) && fixedpoint_is_neg(left)){
    if (fixedpoint_mag_greater_than(right, left)){
      return 1;
    }
    else {
      return -1;
    }
  }
  return -2; // should never get here
} // 18 lines



// Determine whether a Fixedpoint value is equal to 0.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is a valid Fixedpoint value equal to 0;
//   0 is val is not a valid Fixedpoint value equal to 0
int fixedpoint_is_zero(Fixedpoint val) {
  if ((val.tag == '+' || val.tag == '-') && val.f == 0 && val.w == 0){
    return 1;
  }
  return 0;
} // 3 lines



// Determine whether a Fixedpoint value is an "error" value resulting
// from a call to fixedpoint_create_from_hex for which the argument
// string was invalid.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of a call to fixedpoint_create_from_hex with
//   an invalid argument string;
//   0 otherwise
int fixedpoint_is_err(Fixedpoint val) {
  if (val.tag == '/'){
    return 1;
  }
  return 0;
}



// Determine whether a Fixedpoint value is negative (less than 0).
//
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is a valid value less than 0;
//   0 otherwise
int fixedpoint_is_neg(Fixedpoint val) {
  if (val.tag == '-'){
    return 1;
  }
  return 0;
} // 3 lines



// Determine whether a Fixedpoint value is the result of negative overflow.
// Negative overflow results when a sum, difference, or product is negative
// and has a magnitude that is too large to represent.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where negative overflow occurred;
//   0 otherwise
int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if (val.tag == 'o'){
    return 1;
  }
  return 0;
} // 3 lines



// Determine whether a Fixedpoint value is the result of positive overflow.
// Positive overflow results when a sum, difference, or product is positive
// and has a magnitude that is too large to represent.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where positive overflow occurred;
//   0 otherwise
int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if (val.tag == 'O'){
    return 1;
  }
  return 0;
} // 3 lines



// Determine whether a Fixedpoint value is the result of negative underflow.
// Negative underflow occurs when a division (i.e., fixedpoint_halve)
// produces a value that is negative, and can't be exactly represented because
// the fractional part of the representation doesn't have enough bits.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where negative underflow occurred;
//   0 otherwise
int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if (val.tag == 'u'){
    return 1;
  }
  return 0;
} // 3 lines



// Determine whether a Fixedpoint value is the result of positive underflow.
// Positive underflow occurs when a division (i.e., fixedpoint_halve)
// produces a value that is positive, and can't be exactly represented because
// the fractional part of the representation doesn't have enough bits.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where positive underflow occurred;
//   0 otherwise
int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if (val.tag == 'U'){
    return 1;
  }
  return 0;
} // 3 lines



// Determine whether a Fixedpoint value represents a valid negative or non-negative number.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val represents a valid negative or non-negative number;
//   0 otherwise
int fixedpoint_is_valid(Fixedpoint val) {
  if (val.tag == '+' || val.tag == '-'){
    return 1;
  }
  return 0;
} // 3 lines



// Return a dynamically allocated C character string with the representation of
// the given valid Fixedpoint value.  The string should start with "-" if the
// value is negative, and should use the characters 0-9 and a-f to represent
// each hex digit of the whole and fractional parts. As a special case, if the
// Fixedpoint value represents an integer (i.e., the fractional part is 0),
// then no "decimal point" ('.') should be included.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   dynamically allocated character string containing the representation
//   of the Fixedpoint value
char *fixedpoint_format_as_hex(Fixedpoint val) {
  
  assert(fixedpoint_is_valid(val)); // check fixedpoint is valid
  char *s = malloc(35); // dynamically allocate string of maximum possible length

  char num[34]; // locally stored string for formatting
  
  // handles if no fractional part, stores in num
  if (val.f == 0){
    sprintf(num, "%lx", val.w);
  }

  // handles with fractional part, stores in num
  else {
    sprintf(num, "%lx.%016lx", val.w, val.f);
    remove_trailing_zeros(num);
  }
  
  // handles +/-, stores in s
  if (fixedpoint_is_neg(val)){
    sprintf(s, "-%s", num);
  }
  else {
    sprintf(s, "%s", num);;
  }

  return s;

} // 13 lines



// Removes trailing zeroes from a hex string. Called by format as hex
//
// Parameters:
//   *hexString - pointer to a hex string
void remove_trailing_zeros(char *hexString){
  
  // iterates backwards through string
  int i = strlen(hexString) - 1 ;
  while (hexString[i] == '0'){
    i--;
  }
  hexString[i+1] = '\0'; // inserts null terminator after last significant digit
} // 4 lines



// Handles possibility of a positive zero.  Called anywhere a positive zero could happen
//
// Parameters:
//   val - a fixedpoint value
//
// Returns:
//  a fixedpoint that is a positive zero if val was a negative zero
Fixedpoint positive_zero(Fixedpoint val){
  if (val.tag == '-' && fixedpoint_is_zero(val)){
    val.tag = '+';
  }
  return val;
} // 3 lines

