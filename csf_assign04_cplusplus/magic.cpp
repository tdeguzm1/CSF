/*
 * CSF Assignment 4
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include "elf_names.h"

int check_if_not_elf(size_t file_size, int fd);
void print_elf_summary(Elf64_Ehdr* elf_header);
void print_symbols(Elf64_Ehdr* elf_header, Elf64_Shdr* sh_table, int sym_index, int sym_strings_idx);
void print_section_summary(Elf64_Ehdr* elf_header);
void print_individual_sections(Elf64_Ehdr* elf_header, unsigned num_sections, Elf64_Shdr* sh_table, char* string_table);
void print_symbols(Elf64_Ehdr* elf_header, Elf64_Shdr* sh_table, int sym_index, int sym_strings_idx);


/** Main driver function for elf file analysis
 *
 *  Parameters-
 *    Command line arguments
 *    [1] - String representing the file to be read
 */
int main(int argc, char **argv) {
  
  // checks that at least one CMD-line argument was provided
  if (argc < 1) {
    std::cerr << "No command-line args." << std::endl;
    return 1;
  }
  
  // open file and check for success
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    std::cerr << "Unable to open input file." << std::endl;
    return -1;
  }
  
  // look at opened file to figure out length
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    std::cerr << "Unable to determine size of input file." << std::endl;
    return -2;
  }
  
  // check if the file is encoded as an elf file
  size_t file_size = statbuf.st_size;
  if (!check_if_not_elf(file_size, fd)) {
    return 0;
  }

  // load information from elf into virtual memory
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == ((void*)-1)) {
    std::cerr << "Unable to execute mmap." << std::endl;
    return -2;
  }
  
  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data; // typecast data to an elf header

  print_elf_summary(elf_header); // print the summary

} // 20 lines


/** Check if file is tagged as an elf file
 *
 *  Parameters-
 *    file_size - number of bytes to make sure it is long enough
 *    fd - opened file designation
 *
 *  Returns:
 *     0 if not an elf, 1 if it is an elf
 *
 *  Assumptions: if the file is tagged as an elf, then it is an elf
 */
int check_if_not_elf(size_t file_size, int fd) {
  if(file_size > 4){
    char test[5];
    test[4] = '\0';
    char expected[5];
    expected[0] = 0x7f;
    expected[1] = 'E';
    expected[2] = 'L'; 
    expected[3] = 'F'; 
    expected[4] = '\0';
    read(fd, test, 4);
    if (strcmp(test, expected) != 0) {
      std::cout << " Not an ELF file" << std::endl;
      return 0;
    }
  } else {
    std::cout << "Not an ELF file" << std::endl;
    return 0;
  }
  return 1;
} // 16 lines

/** Print the infomation available in the elf_header
 *
 *  Parameters-
 *    elf_header - pointer to the header in the elf file
 */
void print_elf_summary(Elf64_Ehdr* elf_header) {
  std::string endian;
  if(elf_header->e_ident[EI_DATA] == 1) {
    endian = "Little endian";
  } else if (elf_header->e_ident[EI_DATA] == 2) {
    endian = "Big Endian";
  }

  std::cout << "Object file type: " << get_type_name(elf_header->e_type) << std::endl;
  std::cout << "Instruction set: " << get_machine_name(elf_header->e_machine) << std::endl;
  std::cout << "Endianness: " << endian << std::endl;

  print_section_summary(elf_header);

} // 9 lines


/** Finds information needed to print section summaries
 *
 *  Parameters-
 *    elf_header - pointer to the header in the elf file
 */
void print_section_summary(Elf64_Ehdr* elf_header) {

  unsigned num_sections = elf_header->e_shnum;
  Elf64_Shdr* section_header_table = (Elf64_Shdr*) ((char*) elf_header + elf_header->e_shoff);
  Elf64_Shdr* string_table_header = &(section_header_table[elf_header->e_shstrndx]);
  char* string_table = ((char*) elf_header + string_table_header->sh_offset);
  print_individual_sections(elf_header, num_sections, section_header_table, string_table);

}

/** Iterates over the secitons, printing their information
 *
 *  Parameters-
 *    elf_header - pointer to the header in the elf file
 *    num_sections - number of sections in the elf file
 *    sh_table - pointer to the section header table
 *    string_table - pointer to the start of the string table data
 */
void print_individual_sections(Elf64_Ehdr* elf_header, unsigned num_sections, Elf64_Shdr* sh_table, char* string_table) {
  int sym_table_index = -1;
  int sym_table_strings_index = -1;
  for (unsigned i = 0; i < num_sections; i++) {
    Elf64_Shdr my_sect = sh_table[i];

    Elf64_Word name_index = my_sect.sh_name;
    char* name = &(string_table[name_index]);
    
    // detect if the symbol table or string symbol table has been found
    if (strcmp(".symtab", name) == 0) {
      sym_table_index = i;
    } else if (strcmp(".strtab", name) == 0) {
      sym_table_strings_index = i;
    } 

    Elf64_Off type = my_sect.sh_type;
    Elf64_Off offset = my_sect.sh_offset;
    Elf64_Off size = my_sect.sh_size;
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", i, name, type, offset, size);
  }

  print_symbols(elf_header, sh_table, sym_table_index, sym_table_strings_index);
} // 15 lines

/** Iterates over the symbols, printing their information
 *
 *  Parameters-
 *    elf_header - pointer to the header in the elf file
 *    sh_table - pointer to the section header table
 *    sym_index - index to the start of the symbol table
 *    sym_strings_idx - index to the start of the symbol string table
 */
void print_symbols(Elf64_Ehdr* elf_header, Elf64_Shdr* sh_table, int sym_index, int sym_strings_idx) {
  
  Elf64_Shdr* sh_symbol_table = &sh_table[sym_index];
  Elf64_Sym* symbol_table = (Elf64_Sym*) ((char*) elf_header + sh_symbol_table->sh_offset);
  unsigned num_symbols = sh_symbol_table->sh_size/sh_symbol_table->sh_entsize;

  //symbol string table
  Elf64_Shdr* sh_symbol_string_table = &sh_table[sym_strings_idx];
  char* symbol_string_table = ((char*) elf_header + sh_symbol_string_table->sh_offset);
  
  for (unsigned i = 0; i < num_symbols; i++) {
    Elf64_Sym my_sym = symbol_table[i]; // use like address
    
    Elf64_Word name_index = my_sym.st_name;
    char* name = &(symbol_string_table[name_index]);
    
    Elf64_Off size = my_sym.st_size;
    Elf64_Off info = my_sym.st_info;
    Elf64_Off other = my_sym.st_other;
    printf("Symbol %d: name=%s, size=%lx, info=%lx, other=%lx\n", i, name, size, info, other);

  }
} // 13 lines
