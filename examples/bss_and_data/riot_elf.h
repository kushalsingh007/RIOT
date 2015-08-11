#ifndef RIOT_ELF_H
#define RIOT_ELF_H

#include "elf.h"

void list_shdr_table_entries(char *);
void list_symbol_info(char *);
void print_symbol_info(Elf32_Half);
void process_symtab(char *, Elf32_Ehdr, int);
void print_rel_type(Elf32_Word);
void print_sym_name(char *, Elf32_Ehdr, int, int);
void process_rel(char *, Elf32_Ehdr, int );
void print_break(void);
Elf32_Addr elf_locate(char * elf_ptr, char *file_ptr);

#endif /* RIOT_ELF_H */
