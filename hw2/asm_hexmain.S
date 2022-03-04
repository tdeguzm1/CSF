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
  twoSpace: .string "  "
  threeSpace: .string "   "
  colonSpace: .string ": "
  newLine: .string "\n"

.section .text

	.globl main

main:
  pushq %rbx        
  pushq %rbp
  subq $40, %rsp        // allocate space for a 9 byte string and 17 byte string
  movq %rsp, %rbx       // %rbx holds location of offset_buff (needs 9 bytes, has 16)
  leaq 16(%rsp), %rbp   // %rbp holds location of buff  (needs 17 bytes, has 24)

  
  movq %rbp, %rdi     
  call hex_read       // returns number of chars read stored %rbp to %rax
  
  movq $0, %r10       // initalize index for counting number of buffers read

.LmainLoop:
  cmp $0, %rax
  jle .LmainLoopDone  // if no characters were read, return
  

  pushq %rax
  pushq %r10
  shl $4, %r10  
  movq %r10, %rdi          // pass number of buffers read * 16 as first argument
  movq %rbx, %rsi          // pass buffer to store offset string as second argument
  call hex_format_offset   

  movq %rbx, %rdi
  call hex_write_string    // write offset string
  movq $colonSpace, %rdi   
  call hex_write_string    // write ": " for format

  popq %r10                // get %r10/%rax values back off the stack               
  popq %rax
  incq %r10                // increment num_buffs read counter

  pushq %rax
  pushq %r10
  movq %rbp, %rdi                // pass buffer to write hex equivalent
  movq %rax, %rsi                // pass number of values read
  call print_hex_equivalent      
  popq %r10
  popq %rax
  

  pushq %rax
  pushq %r10
  movq %rbp, %rdi                // pass buffer to rewrite to ouput 
  movq %rax, %rsi                // pass number of bytes read 
  call reprint_ascii_form
  popq %r10
  popq %rax

  cmp $16, %rax
  jb .LmainLoopDone      // exit loop early if less than 16 characters read

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
    pushq %rbp
    subq $16, %rsp
    movq %rsp, %rbp   // allocate 16 bytes for storing values (only 3 needed)
    movq $0, %r10     // index for counting hex values written
  
  .LprintCharsLoop:
    cmp %rsi, %r10
    jae .LprintSpaceLoop  // go to space-loop if all characters read from buffer
    
    pushq %r10
    pushq %rsi
    pushq %rdi
    
    movb (%rdi, %r10), %sil      // just need to store this value temporarily
    movq %rsi, %rdi              // pass a character as first argument
    movq %rbp, %rsi              // pass a temporary buffer as second argument
    call hex_format_byte_as_hex

    movq %rbp, %rdi
    call hex_write_string        // write hex equivalent string
  
    movq $oneSpace, %rdi
    call hex_write_string        // write space after hex equivalent string
    
    popq %rdi
    popq %rsi
    popq %r10
    incq %r10                    // incement counter

    jmp .LprintCharsLoop
  
  .LprintSpaceLoop:
    cmp $16, %r10
    jae .LloopsDone              // this loop prints as many extra spaces as needed

    pushq %r10
    movq $threeSpace, %rdi
    call hex_write_string        // write a three-space ("   ") placeholder
    popq %r10

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
    movq %rsp, %rbp  // allocated 16 bytes for chars (only really need 2)
    movq $0, %r10    // index initalized

  .LreprintLoop:
    cmp %rsi, %r10
    jae .LreprintLoopDone
    pushq %r10
    pushq %rsi
    pushq %rdi
    
    movb (%rdi, %r10), %r8b    // just temporarily store to %r8
    movq %r8, %rdi             // pass a character as first argument
    call hex_to_printable      // returns printable character in %rax
    
    movb %al, 0(%rbp)          
    movb $0, 1(%rbp)
    movq %rbp, %rdi           
    call hex_write_string      // format single character and write it
    
    popq %rdi
    popq %rsi
    popq %r10
    incq %r10
    jmp .LreprintLoop
  
  .LreprintLoopDone:
    movq $newLine, %rdi
    call hex_write_string    // write newLine

    addq $16, %rsp           // restore %rsp/%rbp
    popq %rbp
    ret

/* vim:ft=gas:
 */
