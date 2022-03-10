/*
 * Code to implement hexdump
 * CSF Assignment 2 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

// x86-64 assembly implementation of hexdump main function
// Takes input from the user or from file, and prints out
// a hexadecimal equivalent, 16 bytes at a time

// This file includes added helper functions to help main, but
// they are local only to this file, as I designed

/* Hexdump main function, assembly language version */

.section .rodata
  oneSpace: .string " "
  threeSpace: .string "   "
  colonSpace: .string ": "
  newLine: .string "\n"

.section .text

	.globl main

main:
  pushq %r12           // %r12 used to store num chars read   (callee saved)
  pushq %r13           // %r13 used to store num buffers read (callee saved)
  pushq %rbp       

  subq $32, %rsp        // allocate space for a 17 byte string
  movq %rsp, %rbp       // %rbp holds location of buff  (needs 17 bytes, has 32)

  movq %rbp, %rdi      // passes buffer to read into as first argument
  call hex_read        // returns number of chars read & stored in %rbp to %rax
  movq %rax, %r12      // store number of chars read in %r12 (callee saved)

  movq $0, %r13        // initalize index for counting number of buffers read

.LmainLoop:
  cmp $0, %r12
  jle .LmainLoopDone            // if no characters were read, return
  

  movq %r13, %rdi               // pass number of buffers read as first argument
  call print_hex_offset
  incq %r13                     // increment buffer counter

  movq %rbp, %rdi                // pass buffer to write hex equivalent as first argument
  movq %r12, %rsi                // pass number of values read as second argument
  call print_hex_equivalent      
  
  movq %rbp, %rdi                // pass buffer to rewrite to ouput 
  movq %r12, %rsi                // pass number of bytes read 
  call reprint_ascii_form
  
  cmp $16, %r12
  jb .LmainLoopDone              // exit loop early if less than 16 characters read
  
  movq %rbp, %rdi                // pass buffer to place values read 
  call hex_read          
  movq %rax, %r12                // save the number of bytes read

  jmp .LmainLoop

.LmainLoopDone:
  addq $32, %rsp  // return the stack and callee saved registers to inital state
  popq %rbp       
  popq %r13       
  popq %r12
  movq $0, %rax    // Return 0 on completion
	ret              





// Prints the hex equivalent of a 16 byte input buffer
//
// Parameters:
//   *char buff - pointer to a ascii string (%rdi)
//   unsigned chars_read - number of items in the buffer (%rsi)
//
print_hex_equivalent:
    
    pushq %rbp
    subq $16, %rsp
    movq %rsp, %rbp   // allocate 16 bytes for storing values (only 3 needed)
    
    movq $0, %r10     // index for counting hex values written
  
  // C line 55
  .LprintCharsLoop:
    cmp %rsi, %r10        // compare counter to the given number of chars read
    jae .LprintSpaceLoop  // go to space-loop if all characters read from buffer
    
    // save values for every loop iteration
    pushq %r10  
    pushq %rsi
    pushq %rdi
    
    movb (%rdi, %r10), %sil      // temporarily store a charcter to convert to hex
    movq %rsi, %rdi              // pass the character as first argument
    movq %rbp, %rsi              // pass a temporary buffer as second argument
    call hex_format_byte_as_hex

    movq %rbp, %rdi              // pass buffer to write as first argument
    call hex_write_string        // write hex equivalent string
  
    movq $oneSpace, %rdi
    call hex_write_string        // write space after hex equivalent string
    
    // Restore loop values
    popq %rdi
    popq %rsi
    popq %r10
    incq %r10                    // increment counter

    jmp .LprintCharsLoop
  
  .LprintSpaceLoop:
    cmp $16, %r10                // make sure 16 hex values worth of characters were printed
    jae .LloopsDone              // this loop prints as many extra spaces as needed
    
    pushq %r10                   // save loop counter
    movq $threeSpace, %rdi       // pass three spaces to hex_read
    call hex_write_string        // write a three-space ("   ") placeholder
    popq %r10                    // restore loop counter
    
    incq %r10
    jmp .LprintSpaceLoop
  
  .LloopsDone:
    movq $oneSpace, %rdi
    call hex_write_string      // write extra space at the end
    
    addq $16, %rsp             // restore %rsp/%rbp
    popq %rbp
    ret



// Prints the ascii form of a 16 byte input buffer
//
// Parameters:
//   *char buff - pointer to a ascii string (%rdi)
//   unsigned chars_read - number of items in the buffer (%rsi)
//
reprint_ascii_form:
    
    pushq %rbp
    subq $16, %rsp
    movq %rsp, %rbp            // allocated 16 bytes for chars (only really need 2)
    
    movq $0, %r10              // index initalized
   
  .LreprintLoop:
    cmp %rsi, %r10
    jae .LreprintLoopDone      // loop finished if index >= number of chars read
    
    // save loop state
    pushq %r10
    pushq %rsi
    pushq %rdi
    
    movb (%rdi, %r10), %sil     // temporarily store to %r8
    movq %rsi, %rdi             // pass a character as first argument
    call hex_to_printable       // returns printable character in %rax
    
    movb %al, 0(%rbp)           // move printable byte in to string buffer (C 78)
    movb $0, 1(%rbp)            // move null terminator into stirng buffer (C 79)
    movq %rbp, %rdi             // move address in %rbp to %rdi
    call hex_write_string       // format single character and write it
    
    // restore loop state and increment counter (C 77)
    popq %rdi
    popq %rsi
    popq %r10
    incq %r10
    jmp .LreprintLoop
  
  .LreprintLoopDone:
    movq $newLine, %rdi 
    call hex_write_string    // write newLine
    
    // restore %rsp and %rbp
    addq $16, %rsp          
    popq %rbp
    ret



// Prints formatted hex offset
//
// Parameters:
//   num_buffs- the number of buffers full read
//
print_hex_offset:
  pushq %rbp
  subq $16, %rsp
  movq %rsp, %rbp          // allocate 16 bytes on the stack, only need 9

  shl $4, %rdi             // pass number of buffers * 16 as first argument
  movq %rbp, %rsi          // pass buffer to store offset string as second argument
  call hex_format_offset   
  
  movq %rbp, %rdi          // pass buffer as first argument
  call hex_write_string    // write offset string
  
  movq $colonSpace, %rdi   
  call hex_write_string    // write ": " for format
  
  // restore %rsp and %rbp
  addq $16, %rsp
  popq %rbp
  ret



/* vim:ft=gas:
 */
