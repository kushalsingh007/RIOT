#ifndef RIOT_ELF_H
#define RIOT_ELF_H

#include "elf.h"

void list_shdr_table_entries(char *);
void list_symbol_info(char *);
void print_symbol_info(Elf32_Half);
void process_symtab(char *, int);
void print_rel_type(Elf32_Word);
void print_sym_name(char *, int, int);
void process_rel(char *, int );
void print_break(void);
Elf32_Addr elf_locate(char *, char *);
Elf32_Shdr* getElfSectionHdr(char *, int);
Elf32_Sym* getElfSym(char *, Elf32_Off, int);
Elf32_Rel* getElfRel(char *, Elf32_Off, int);
int elf_parse_rel(char *elf_ptr,int index);
void elf_relocate_internal(char *);

typedef struct rel_array{
    char *name;
    Elf32_Addr addr;
}rel_array;


rel_array r_array[100];
int count;
#endif /* RIOT_ELF_H */
