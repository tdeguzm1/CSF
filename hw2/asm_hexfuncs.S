/*
 * Code to implement hex functions
 * CSF Assignment 2 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* add read-only data such as strings here */

	//.section.bss
	

	.section .text
	  .globl hex_format_byte_as_hex
	  .globl hex_to_printable
	  .globl hex_format_offset
	  .globl hex_write_string
	  .globl hex_read
    
    // Write given nul-terminated string to standard output.
    //
    // Parameters:
    //     char[] s - a string to be written (in %rdi)
    //  
	hex_write_string:
      subq $8, %rsp
      movq $0, %r10
	.LwriteLoop:
	  leaq (%rdi, %r10), %rsi  // store char address for comparison and write
      cmpb $0, (%rsi)
	  je .LwriteLoopDone       // finish if null terminator is reached
	  
	  pushq %rdi
	  pushq %r10
      movq $1, %rdi            // move STDOUT_FILENO into 1st argument
      movq $1, %rdx            // move 1 (byte) into third argument
	  call write
	  popq %r10
      popq %rdi
	  incq %r10
      jmp .LwriteLoop
    
	.LwriteLoopDone:
	  addq $8, %rsp           // restore stack pointer
	  ret


    // Read up to 16 bytes from standard input into data_buf.
    // Returns the number of characters read.
    //
    // Parameters:
    //     char[] data_buf - a string to put read bytes into- passed in %rdi
    //              Pre: data_buf must be >= 17 bytes
    // Returns:
    //     number of bytes read into data_buf, returned in %rax
	hex_read:
	  subq $8, %rsp
      movq %rdi, %rsi            // move buffer to write to into second argument
	  movq $0, %rdi              // move STDIN_FILENO into 1st argument (%rdi)
	  movq  $16, %rdx            // move 16 (number of bytes) into 3rd argument (%rdx)
	  call read
	  addq $8, %rsp
	  ret


	// Format a byte value (in the range 0-255) as string consisting
    // of two hex digits.  The string is stored in sbuf.
    //
    // Parameters:
    //     (%rdi) char byteval - byte to convert to a hex string
    //     (%rsi) char sbuf[] - buffer to store 2-byte hex equivalent string in
    //                   Pre: must have >= 3 bytes
    // 
	hex_format_byte_as_hex: 
	  subq $8, %rsp
	  pushq %rdi
	  shr $4, %rdi           // divide byteval by 16
	  call to_hex            // get first digit of hex value
	  movb %dil, (%rsi)      // save first digit of hex value
	  popq %rdi              // restore byteval
	  andq $15, %rdi         // %rdi = %rdi % 16
	  call to_hex            // get second digit of hex value
	  movb %dil, 1(%rsi)     // save second digit of hex value
	  movb $0, 2(%rsi)       // add null terminator to end
	  addq $8, %rsp
	  ret
   


    // Convert a byte value (in the range 0-255) to a printable character
    // value.  If byteval is already a printable character, it is returned
    // unmodified.  If byteval is not a printable character, then the
    // ASCII code for '.' should be returned.
    //
    // Parameters:
    //     (%rdi) byteval - byte to convert to a printable; only really in (%dil)
    // Returns:
    //     char that is printable in (%rax) 
    // 
	hex_to_printable:
	  subq $8, %rsp
	  cmp $32, %dil
      jb .LdecNum         // if char is < 32, jump
	  cmp $127, %dil
      jae .LdecNum        // if char is >= 127, jump
	  movb %dil, %al      // move lowest bit of %rdi (%dil) to %rax (%al) for return
	  addq $8, %rsp
	  ret
	.LdecNum:             // if not printable character:
	   movb $46, %al      // move '.' (46) to %rax (%al) for return
	   addq $8, %rsp
	   ret
	
    
	  

    // Convert a single integer (0-15) to a single hex char
    //
    // Parameters:
    //     (%rdi) val - integer to convert (between 0-15)
    // Returns:
    //     (%rdi) char equivalent of integer value
    //              Post: This method only modifies %rdi
    //   
	to_hex:
	  subq $8, %rsp
	  cmp $9, %rdi       // if %rax > 10, jump to else
      ja .LifLetter      
	  addq $48, %rdi      // if it is a number, add 48 to get ascii
	  addq $8, %rsp
	  ret
	.LifLetter:
	  addq $87, %rdi      // if it is a number, add 87 to get ascii
	  addq $8, %rsp
	  ret
	  


    // Format an unsigned value as an offset string consisting of exactly 8
    // hex digits.  The formatted offset is stored in sbuf, which must
    // have enough room for a string of length 8 (assume given properly sized sbuf).
    //
    // Parameters:
    //     (%rdi) offset - integer offset size
    //     (%rsi) char sbuf[] - buffer to store offset string in
	//
	// Other registers:
	//     (%r10) - index to iterate over given string buffer
    //  
	hex_format_offset:
	  subq $8, %rsp       
	  movq $0, 8(%rsi)            // move '\0' into end of string_buf
	  movq $7, %r10               // move 7 to %r10 as index for iteration         

	.LdecLoop:
	  cmpq $0, %r10                // compare index (%r10) to zero
	  jl .LdecLoopDone             // if index < 0, end loop
      pushq %rdi                  
	  andq $15, %rdi                // %rdi = %rdi % 16
	  call to_hex                   // converts %rdi to hex value
      movb %dil, (%rsi, %r10, 1)    // puts low byte of %rdi into the proper index of %rsi (sbuf[])
	  popq %rdi                     // restore %rdi
	  shr $4, %rdi                  // divide %rdi by 16
	  decq %r10                     // decrease index by one
	  jmp .LdecLoop               

	.LdecLoopDone:
	  addq $8, %rsp
	  ret

/* vim:ft=gas:
 */
