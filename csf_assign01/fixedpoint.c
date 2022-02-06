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

// You can remove this once all of the functions are fully implemented
//static Fixedpoint DUMMY;

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
}


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
  assert(num.w == whole);
  assert(num.f == frac);
  assert(num.tag == '+');
  return num;
}


//Helper for MS2
void setLast4bits(uint64_t* num, char letter){
  *num = *num << 4;
  if (isalpha(letter)){
    *num += (letter - 87);
  }
  else {
    *num += (letter - 48);
    }
 }

int validChar(Fixedpoint *val, char ch, int count){
  if (count > 16);
  else if (isdigit(ch) || (ch >= 'a' && ch <= 'f')){
    return 1;
  }
  val->tag = '/';
  return 0;
}
    
//Part of MS2
Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint num;
  num.w = 0;
  num.f = 0;
  uint64_t len = strlen(hex);
  
  uint64_t start = 0;
  if (hex[0] == '-'){
    num.tag = '-';
    start = 1;
  }
  else if (validChar(&num, hex[0], 1)){
    num.tag = '+';
  }

  char *decimal = strchr(hex, '.');
  for (int i = 0; &hex[i+start] != decimal && i+start < len; i++){
    validChar(&num, hex[i+start], i+1);
    setLast4bits(&num.w, hex[i+start]);
  }

  //count++;
  if (decimal != NULL){
    int i;
    for (i = 1; &hex[strlen(hex)] != &decimal[i]; i++){
      validChar(&num, decimal[i], i);
      setLast4bits(&num.f, decimal[i]);
    }
    num.f = num.f << ((16-(i-1))*4);
  }
  return num;
}


// Get the whole part of the given Fixedpoint value.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   a uint64_t value which is the whole part of the Fixedpoint value
uint64_t fixedpoint_whole_part(Fixedpoint val) {
  uint64_t whole = val.w;
  assert(whole == val.w);
  return whole;
}

// Get the fractional part of the given Fixedpoint value.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   a uint64_t value which is the fractional part of the Fixedpoint value 
uint64_t fixedpoint_frac_part(Fixedpoint val) {
  uint64_t frac = val.f;
  assert(frac == val.f);
  return frac;
}


//Part of MS2
Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left) && fixedpoint_is_valid(right));

  Fixedpoint sum;

  if (fixedpoint_is_neg(left) == fixedpoint_is_neg(right)){ 

    // Check if + or -
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

    // check if fixedpoint whole overflowed
    if (is_in_overflow(sum.w, left.w, right.w)){
      sum = to_overflow(sum);
    }
  }

  else if (fixedpoint_is_neg(left)){

    left = fixedpoint_negate(left);
    sum = fixedpoint_sub(right, left);
  }
  else {
    right = fixedpoint_negate(right);
    sum = fixedpoint_sub(left, right);
  }

  sum = positive_zero(sum);
  return sum;
}

// Helper funcitons for MS2
char is_in_overflow(uint64_t sum, uint64_t num1, uint64_t num2){
  if (sum < num1 || sum < num2){
    return 1;
  }
  return 0;
}

int fixedpoint_mag_greater_than(Fixedpoint left, Fixedpoint right) {
  if ((left.w > right.w)  || ((left.w == right.w) && (left.f > right.f))){
    return 1;
  }
  return 0;
}

//Part of MS2
Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left) && fixedpoint_is_valid(right));

  Fixedpoint diff;

  if (fixedpoint_is_neg(left)  == fixedpoint_is_neg(right)){
    
    if (fixedpoint_mag_greater_than(right, left)){
      diff = fixedpoint_sub(right, left);
      diff = fixedpoint_negate(diff);
    }
    else {

      // Check if + or -
      if (fixedpoint_is_neg(left)){
        diff.tag = '-';
      }
      else {
        diff.tag = '+';
      }

      // add parts together
      diff.w = left.w - right.w;
      diff.f = left.f - right.f;
      //printf("next check if borrow");

      // check if fractional part requires borrowing
      if (right.f > left.f) {
        diff.w -= 1;
        //printf("borrow occurred\n");
      }

    }
  }

  else if (fixedpoint_is_neg(left)){
    right = fixedpoint_negate(right);
    diff = fixedpoint_add(right, left);
  }
  else {
    right = fixedpoint_negate(right);
    diff = fixedpoint_add(left, right);
  }
    
  return diff;
}



//Part of MS2
Fixedpoint fixedpoint_negate(Fixedpoint val) {
  switch (val.tag){
    case '+':
      val.tag = '-';
      break;
    case '-':
      val.tag = '+';
      break;
    case 'O':
      val.tag = 'o';
      break;
    case 'o':
      val.tag = 'O';
      break;
    case 'U':
      val.tag = 'u';
      break;
    case 'u':
      val.tag = 'U';
      break;
    default:
      val.tag = '/';
      break;
  }
  val = positive_zero(val);
  return val;
}

//Part of MS2
Fixedpoint fixedpoint_halve(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));
  if (val.f % 2 == 1){
    return to_underflow(val);
  }
  val.f = val.f >> 1;
  if (val.w % 2 == 1){
    val.f += (((uint64_t)1) << 63);
  }
  val.w = val.w >> 1;
  return val;
}

Fixedpoint to_underflow(Fixedpoint val){
  if (fixedpoint_is_neg(val)){
    val.tag = 'u';
  }
  else {
    val.tag = 'U';
  }
  return val;
}

//Part of MS2
Fixedpoint fixedpoint_double(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));
  if (val.w >= (((uint64_t)1) << 63)){
    return to_overflow(val);
  }
  val.w = val.w << 1;
  if (val.f >= (((uint64_t)1) << 63)){
    val.w += 1;
  }
  val.f = val.f << 1;
  return val;
}

Fixedpoint to_overflow(Fixedpoint val){
  if (fixedpoint_is_neg(val)){
    val.tag = 'o';
  }
  else {
    val.tag = 'O';
  }
  return val;
}

//Part of MS2
int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left) && fixedpoint_is_valid(right));
  if (fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)){
    return -1;
  }
  else if (fixedpoint_is_neg(right) && !fixedpoint_is_neg(left)){
    return 1;
  }
  else if (right.w == left.w && left.f == right.f){
    return 0;
  }
  else if (!fixedpoint_is_neg(right) && !fixedpoint_is_neg(left)){
    if (left.w > right.w){
      return 1;
    }
    else if (left.w < right.w){
      return -1;
    }
    else {
      if (left.f > right.f){
        return 1;
      }
      return -1;
    }
  }
  else if (fixedpoint_is_neg(right) && fixedpoint_is_neg(left)){
    if (left.w < right.w){
      return 1;
    }
    else if (left.w > right.w){
      return -1;
    }
    else {
      if (left.f < right.f){
        return 1;
      }
      return -1;
    }
  }
  return -2;
}

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
}

//Part of MS2
int fixedpoint_is_err(Fixedpoint val) {
  if (val.tag == '/'){
    return 1;
  }
  return 0;
}

//Part of MS2
int fixedpoint_is_neg(Fixedpoint val) {
  if (val.tag == '-'){
    return 1;
  }
  return 0;
}

//Part of MS2
int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if (val.tag == 'o'){
    return 1;
  }
  return 0;
}

//Part of MS2
int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if (val.tag == 'O'){
    return 1;
  }
  return 0;
}

//Part of MS2
int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if (val.tag == 'u'){
    return 1;
  }
  return 0;
}

//Part of MS2
int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if (val.tag == 'U'){
    return 1;
  }
  return 0;
}

//Part of MS2
int fixedpoint_is_valid(Fixedpoint val) {
  if (val.tag == '+' || val.tag == '-'){
    return 1;
  }
  return 0;
}

//Part of MS2
char *fixedpoint_format_as_hex(Fixedpoint val) {
  
  assert(fixedpoint_is_valid(val));
  char *s = malloc(35);

  char num[34];
  
  if (val.f == 0){
    sprintf(num, "%lx", val.w);
  }
  else {
    sprintf(num, "%lx.%016lx", val.w, val.f);
    remove_trailing_zeros(num);
  }

  if (fixedpoint_is_neg(val)){
    sprintf(s, "-%s", num);
  }
  else {
    sprintf(s, "%s", num);;
  }

  return s;

}

void remove_trailing_zeros(char *hexString){
  int i = strlen(hexString) - 1 ;
  while (hexString[i] == '0'){
    i--;
  }
  hexString[i+1] = '\0';
}

Fixedpoint positive_zero(Fixedpoint val){
  if (val.tag == '-' && fixedpoint_is_zero(val)){
    val.tag = '+';
  }
  return val;
}

