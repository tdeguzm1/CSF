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
	/* TODO: add your code here */

	ret


// Prints the hex equivalent of a 16 byte input buffer
//
// Parameters:
//   *char buff - pointer to a ascii string (%rdi)
//   unsigned chars_read - number of items in the buffer (%esi)
//
print_hex_equivalent:
    

    ret

reprint_ascii_form:


    ret

/* vim:ft=gas:
 */
