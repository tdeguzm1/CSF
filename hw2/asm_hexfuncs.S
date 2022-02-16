/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text
	  .globl hex_format_byte_as_hex
	  .globl hex_to_printable
	  .globl hex_format_offset

	hex_format_byte_as_hex:
	  ret

	hex_to_printable:
	  ret

	hex_format_offset:       // %rdi stores int offset, %rsi stores sbuf[]
	  movq $0, 8(%rsi)       // move '\0' into end of %rsi
	  movq $7, %r10          // move 7 to %rsi as index
	  movq %rdi, %rax        // move offset to %rdx for division
	  movq $0, %rdx          
	  movq $15, %r11

	.LdecLoop:
	  cmpq $0, %r10               // compare index to zero
	  jb .LdecLoopDone            // if index < 0, end loop
      divq %r11                   // divide %rdx:%rax by 16, quotient in %rax, remainder in %rdx
	  //call to_hex               // calls to_hex, should modify %rdx
      movq %rdx, (%rsi, %r10)     // puts %rdx into the proper index of %rsi (sbuf[])
	  movq %rax, %rdx             // move quotient into %rdx
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