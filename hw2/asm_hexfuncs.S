/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	//.section.bss
	

	.section .text
	  .globl hex_format_byte_as_hex
	  .globl hex_to_printable
	  .globl hex_format_offset
	  //.globl to_hex
    

	/*void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]){
  sbuf[0] = to_hex(byteval / 16);
  sbuf[1] = to_hex(byteval % 16);
  sbuf[2] = '\0';
} */

	hex_format_byte_as_hex: // recieves byteval in %edi/%dil , %rsi stores sbuf
	  movq %rdi, %rax
	  movq $0, %rdx
	  movq $16, %r11
	  divq %r11       // divide %rdx:%rax by 16, quotient in %rax, remainder in %rdx
	  call to_hex
	  movb %dl, 1(%rsi)
	  movq %rax, %rdx
	  call to_hex
	  movb %dl, (%rsi)
	  movb $0, 2(%rsi)
	  ret
   
/*char hex_to_printable(unsigned char byteval){
   if (byteval < 32 || byteval == 127 || byteval > 254){
       return '.';
   }
   return byteval;
}*/

	hex_to_printable: // %rdi or (%dil)b is bassed as byteval equivalent
	  cmp $32, %dil
      jb .LdecNum
	  cmp $254, %dil
      ja .LdecNum
	  cmp $127, %dil
      je .LdecNum
	  movb %dil, %al
	  ret
	.LdecNum:
	   movb $46, %al
	   ret
	
    
	  

	/*char to_hex(unsigned val) {
    if (val < 10) {
        return val + 48;
    }
    else {
        return val + 87;
    }}*/
    
	// val in %rdx, return to %rdx
	to_hex:
	  cmp $9, %rdx   // if %rax > 10, jump to else
      ja .LifLetter
	  addq $48, %rdx
	  ret
	.LifLetter:
	  addq $87, %rdx
	  ret
	  

    // (%rsi) stores buffer
	// (%r10d) is index counter
	// (%)

	hex_format_offset:       // %rdi stores int offset, %rsi stores sbuf[]
	  movq $0, 8(%rsi)       // move '\0' into end of %rsi
	  movq $7, %r10         // move 7 to %rsi as index
	  movq %rdi, %rax        // move offset to %rdx for division
	  movq $0, %rdx          // 
	  movq $16, %r11

	.LdecLoop:
	  cmpq $0, %r10               // compare index to zero
	  jl .LdecLoopDone            // if index < 0, end loop
      divq %r11                   // divide %rdx:%rax by 16, quotient in %rax, remainder in %rdx
	  call to_hex               // calls to_hex, should modify %rdx
      movb %dl, (%rsi, %r10)     // puts %rdx into the proper index of %rsi (sbuf[])
	  movq $0, %rdx
	  decq %r10                   // decrease index by one
	  jmp .LdecLoop               // return to top of loop


	.LdecLoopDone:
	  ret

	/* TODO: add functions here */

/* vim:ft=gas:
 */

/*
void hex_format_offset(unsigned offset, char sbuf[]){
  sbuf[8] = '\0';
  for (int i = 7; i >= 0; i--) {
    sbuf[i] = to_hex(offset % 16);
    offset /= 16;
  }
}*/