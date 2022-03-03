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
	  leaq (%rdi, %r10, 1), %rsi
      cmpb $0, (%rsi)
	  je .LwriteLoopDone
	  pushq %rdi
	  pushq %r10
      movq $1, %rdi
	  movq $1, %rdx
      call write
	  popq %r10
      popq %rdi
	  incq %r10
      jmp .LwriteLoop
    .LwriteLoopDone:
	  addq $8, %rsp
	  ret

    /*
	void hex_write_string(const char s[]){
      size_t i = 0;
      while (s[i] != '\0') {
        if (write(STDOUT_FILENO, &s[i], 1) < 1); // if block handles compiler warning
        i++;
      }
    }*/

    // Read up to 16 bytes from standard input into data_buf.
    // Returns the number of characters read.
    //
    // Parameters:
    //     char[] data_buf - a string to put read bytes into- passed in %rdi
    //              Pre: data_buf must be >= 17 bytes
    // Returns:
    //     number of bytes read into data_buf, returned in %rax
	hex_read:
	  pushq %rbp                     // save frame pointer
	  subq $16, %rsp                 // make 16 bytes on stack for char (only really need 8)
      movq %rsp, %rbp                // point frame pointer to stack pointer
	  movq $0, %rax                  // %rax used as a counter to track number of bytes read
	.LreadLoop:
      cmp $16, %rax                  // check if counter is geater than 16
	  jae .LloopDone                 // exit loop if greater than 16
	  pushq %rdi                     // save sbuf[] reference before call to read
	  pushq %rax                     // save counter before call to read
	  movq $0, %rdi                  // move STDIN_FILENO into 1st argument (%rdi)
	  movq %rbp, %rsi                // move address of c into 2nd argument (%rsi)
	  movq $1, %rdx                  // move 1 (number of bytes) into 3rd argument (%rdx)
	  call read                      // read(STDIN_FILENO, &c, 1); returns number of arguments read in rax
	  cmp $0, %rax                   // check number of values read
	  jbe .LloopDone                 // exit loop if zero bytes were read
	  popq %rax                      // restore counter
	  popq %rdi                      // restore sbuf[] reference
	  movb (%rbp), %r10b             // temporarily store value saved in rbp
	  movb %r10b, (%rdi, %rax, 1)    // store character in proper spot in sbuf[]
	  incq %rax
	  jmp .LreadLoop
    .LloopDone:
      //movb $0, (%rdi, %rax, 1)
	  //popq %rax
	  //popq %rdi
	  addq $16, %rsp 
	  popq %rbp
	  ret
    
	/*
	unsigned hex_read(char data_buf[]) {
       unsigned count = 0;
       char c;
       while (count < 16 && read(STDIN_FILENO, &c, 1) > 0) {
         data_buf[count] = c;
         count++;
       }
       data_buf[count] = '\0';
       return count;
    }*/


	// Format a byte value (in the range 0-255) as string consisting
    // of two hex digits.  The string is stored in sbuf.
    //
    // Parameters:
    //     (%rdi) char byteval - byte to convert to a hex string
    //     (%rsi) char sbuf[] - buffer to store 2-byte hex equivalent string in
    //                   Pre: must have >= 3 bytes
	// Other registers:
	//     (%r11) sores literal 16 for division with divq
    // 
	hex_format_byte_as_hex: 
	  subq $8, %rsp
	  movq %rdi, %rax      // put byteval into %rax
	  movq $0, %rdx        
	  movq $16, %r11
	  divq %r11            // divide %rdx:%rax by 16, quotient in %rax, remainder in %rdx
	  call to_hex          // converts int remainder in %rdx to equivalent hex char in %rdx
	  movb %dl, 1(%rsi)    // moves smallest byte in %rdx (%dl), into the second position of sbuf (%rsi)
	  movq %rax, %rdx      // move quotient in %rax to %rdx for conversion to hex char
	  call to_hex          // converts int quotient in %rdx to equivalent hex char in %rdx
	  movb %dl, (%rsi)     // moves smallest byte in %rdx (%dl), into the first position of sbuf (%rsi)
	  movb $0, 2(%rsi)     // moves null terminator into the third position of sbuf (%rsi)
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
    //     (%rdx) val - integer to convert (between 0-15)
    // Returns:
    //     (%rdx) char equivalent of integer value
    //   
	to_hex:
	  subq $8, %rsp
	  cmp $9, %rdx        // if %rax > 10, jump to else
      ja .LifLetter      
	  addq $48, %rdx      // if it is a number, add 48 to get ascii
	  addq $8, %rsp
	  ret
	.LifLetter:
	  addq $87, %rdx      // if it is a number, add 87 to get ascii
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
	//     (%r11) - stores 16 for divq operation
    //  
	hex_format_offset:
	  subq $8, %rsp       
	  movq $0, 8(%rsi)           // move '\0' into end of string_buf
	  movq $7, %r10              // move 7 to %r10 as index for iteration
	  movq %rdi, %rax            // move offset to %rax for division
	  movq $0, %rdx          
	  movq $16, %r11

	.LdecLoop:
	  cmpq $0, %r10               // compare index (%r10) to zero
	  jl .LdecLoopDone            // if index < 0, end loop
      divq %r11                   // divide %rdx:%rax by 16, quotient in %rax, remainder in %rdx
	  call to_hex                 // converts int remainder in %rdx to equivalent hex char in %rdx
      movb %dl, (%rsi, %r10)      // puts %rdx into the proper index of %rsi (sbuf[])
	  movq $0, %rdx               // reset %rdx to zero
	  decq %r10                   // decrease index by one
	  jmp .LdecLoop               

	.LdecLoopDone:
	  addq $8, %rsp
	  ret

/* vim:ft=gas:
 */
