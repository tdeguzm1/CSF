/*
 * Code to implement hexdump
 * CSF Assignment 2 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

// x86-64 assembly implementation of hexdump main function
// Takes input from the user or from file, and prints out
// a hexadecimal equivalent, 16 bytes at a time

/* Hexdump main function, assembly language version */

.section .rodata
  oneSpace: .string " "
  threeSpace: .string "   "
  colonSpace: .string ": "
  newLine: .string "\n"

.section .text

	.globl main

main:
  pushq %rbx        
  pushq %rbp

  // C lines 20 & 21
  subq $40, %rsp        // allocate space for a 9 byte string and 17 byte string
  movq %rsp, %rbx       // %rbx holds location of offset_buff (needs 9 bytes, has 16)
  leaq 16(%rsp), %rbp   // %rbp holds location of buff  (needs 17 bytes, has 24)

  // C line 23
  movq %rbp, %rdi     // passes buffer to read into as first argument
  call hex_read       // returns number of chars read & stored in %rbp to %rax
  
  // C line 22
  movq $0, %r10       // initalize index for counting number of buffers read

// C line 26
.LmainLoop:
  cmp $0, %rax
  jle .LmainLoopDone  // if no characters were read, return
  

  pushq %rax          // save number of characters read
  pushq %r10          // save number of buffers read
  
  // C line 29
  shl $4, %r10             // multiply number of buffers by 16
  movq %r10, %rdi          // pass result of multipication as first argument
  movq %rbx, %rsi          // pass buffer to store offset string as second argument
  call hex_format_offset   
  
  // C line 30
  movq %rbx, %rdi
  call hex_write_string    // write offset string
  
  // C line 31
  movq $colonSpace, %rdi   
  call hex_write_string    // write ": " for format
  
  
  // C line 32
  popq %r10                // restore %r10 as buffer counter               
  popq %rax                // restore %rax as bytes read counter
  incq %r10                // increment buffer counter


  // C line 35
  pushq %rax
  pushq %r10
  movq %rbp, %rdi                // pass buffer to write hex equivalent as first argument
  movq %rax, %rsi                // pass number of values read as second argument
  call print_hex_equivalent      
  popq %r10
  popq %rax
  
  // C line 36
  pushq %rax
  pushq %r10
  movq %rbp, %rdi                // pass buffer to rewrite to ouput 
  movq %rax, %rsi                // pass number of bytes read 
  call reprint_ascii_form
  popq %r10
  popq %rax
  
  // C lines 39 and 40
  cmp $16, %rax
  jb .LmainLoopDone      // exit loop early if less than 16 characters read
  
  // C line 42
  movq %rbp, %rdi        // pass buffer to place values read 
  call hex_read          
  

  jmp .LmainLoop

.LmainLoopDone:
  addq $40, %rsp   // return the stack and callee saved registers to inital state
  popq %rbp
  popq %rbx
	ret


// Prints the hex equivalent of a 16 byte input buffer
//
// Parameters:
//   *char buff - pointer to a ascii string (%rdi)
//   unsigned chars_read - number of items in the buffer (%rsi)
//
print_hex_equivalent:
    
    // C line 54
    pushq %rbp
    subq $16, %rsp
    movq %rsp, %rbp   // allocate 16 bytes for storing values (only 3 needed)
    
    // C line 53
    movq $0, %r10     // index for counting hex values written
  

  // C line 55
  .LprintCharsLoop:
    cmp %rsi, %r10        // compare counter to the given number of chars read
    jae .LprintSpaceLoop  // go to space-loop if all characters read from buffer
    
    // save values for every loop iteration
    pushq %r10  
    pushq %rsi
    pushq %rdi
    
    // C line 56
    movb (%rdi, %r10), %sil      // temporarily store a charcter to convert to hex
    movq %rsi, %rdi              // pass the character as first argument
    movq %rbp, %rsi              // pass a temporary buffer as second argument
    call hex_format_byte_as_hex

    // C line 57
    movq %rbp, %rdi
    call hex_write_string        // write hex equivalent string
  
    // C line 58
    movq $oneSpace, %rdi
    call hex_write_string        // write space after hex equivalent string
    
    // Restore loop values
    popq %rdi
    popq %rsi
    popq %r10
    incq %r10                    // increment counter

    jmp .LprintCharsLoop
  
  // C line 61
  .LprintSpaceLoop:
    cmp $16, %r10                // make sure 16 hex values worth of characters were printed
    jae .LloopsDone              // this loop prints as many extra spaces as needed
    
    // C line 62
    pushq %r10                   // save loop counter
    movq $threeSpace, %rdi       // pass three spaces to hex_read
    call hex_write_string        // write a three-space ("   ") placeholder
    popq %r10                    // restore loop counter
    
    // C line 63
    incq %r10
    jmp .LprintSpaceLoop
  
  // C line 65
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
    
    // C line 76
    pushq %rbp
    subq $16, %rsp
    movq %rsp, %rbp  // allocated 16 bytes for chars (only really need 2)
    
    // C line 77
    movq $0, %r10    // index initalized
   
   // also C line 77
  .LreprintLoop:
    cmp %rsi, %r10
    jae .LreprintLoopDone
    
    // save loop state
    pushq %r10
    pushq %rsi
    pushq %rdi
    
    // C line 78
    movb (%rdi, %r10), %sil    // temporarily store to %r8
    movq %rsi, %rdi             // pass a character as first argument
    call hex_to_printable      // returns printable character in %rax
    
    movb %al, 0(%rbp)          // move printable byte in to string buffer (C 78)
    movb $0, 1(%rbp)           // move null terminator into stirng buffer (C 79)
    movq %rbp, %rdi            // move address in %rbp to %rdi
    call hex_write_string      // format single character and write it
    
    // restore loop state and increment counter (C 77)
    popq %rdi
    popq %rsi
    popq %r10
    incq %r10
    jmp .LreprintLoop
  
  // C line 82
  .LreprintLoopDone:
    movq $newLine, %rdi 
    call hex_write_string    // write newLine
    
    // restore %rsp and %rbp
    addq $16, %rsp          
    popq %rbp
    ret

/* vim:ft=gas:
 */
