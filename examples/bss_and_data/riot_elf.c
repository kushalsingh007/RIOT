#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "riot_elf.h"

int elf_parse(Elf32_Addr elf_addr)
{

    return 0;
}

Elf32_Shdr* getElfSectionHdr(char *elf_ptr, int index)
{
	return (Elf32_Shdr *) (elf_ptr + ((Elf32_Ehdr *) elf_ptr)->e_shoff + index *((Elf32_Ehdr *) elf_ptr)->e_shentsize);
}

Elf32_Sym* getElfSym(char *elf_ptr, Elf32_Off sh_offset, int index)
{
	return (Elf32_Sym *) (elf_ptr + sh_offset + index * sizeof(Elf32_Sym)); 
}

Elf32_Rel* getElfRel(char *elf_ptr, Elf32_Off sh_offset, int index)
{
	return (Elf32_Rel *) (elf_ptr + sh_offset + index * sizeof(Elf32_Rel));
}
#if 0
/* Print all the Section Table Header entries */
void list_shdr_table_entries(char *elf_ptr)
{
	Elf32_Ehdr *elfHeader;
    Elf32_Shdr *sectionHeader;
    char *stringTable = NULL;

    elfHeader = (Elf32_Ehdr *) elf_ptr;
    sectionHeader = getElfSectionHdr(elf_ptr, elfHeader->e_shstrndx);

    /* Put the contents of the string table in the array */
    stringTable = elf_ptr + sectionHeader->sh_offset;

    /* Print the names of all the sections and process symbol table */
    for(int index=0; index < elfHeader->e_shnum; index++)
    {
        sectionHeader = getElfSectionHdr(elf_ptr, index);

        if(index<10)
            printf("[ %d] %s\n",index, stringTable + sectionHeader->sh_name);
        else
            printf("[%d] %s\n",index, stringTable + sectionHeader->sh_name);

    }

    print_break();

}
#endif

/* Prints the symbol table and the relocation related info */
void list_symbol_info(char * elf_ptr)
{
    Elf32_Ehdr *elfHeader;
    Elf32_Shdr *sectionHeader;

    elfHeader = (Elf32_Ehdr *) elf_ptr;

    /* Finding index of position of symtab */
    for(int index = 0; index < elfHeader->e_shnum; index++)
    {
        sectionHeader = getElfSectionHdr(elf_ptr, index);
        printf("index=%u sh_type=0x%08x\n", index, (unsigned)sectionHeader->sh_type);

        if(sectionHeader->sh_type == SHT_DYNSYM) {
            process_symtab(elf_ptr, index);
        }
        else if (sectionHeader->sh_type == SHT_REL) {
            process_rel(elf_ptr, index);
        }
    }

}

/* Prints the section associated with the symbol */
void print_symbol_info(Elf32_Half shndx)
{
    if( shndx == SHN_UNDEF )
        printf(" -  Undefined Symbol  ");
    else if( shndx == SHN_COMMON)
        printf(" -  COMMON (Currently Unallocated)  ");
    else if (shndx == SHN_ABS)
        printf(" -  ABS (No relocation)  ");
    else
        printf(" -  %s  ","section_name (TODO)" /*section_name[shndx]*/);
}

/* Process the symbol table to give symbol name and it's section type */
void process_symtab(char* elf_ptr, int sym_index)
{
    char *stringTable = NULL;
    Elf32_Shdr *sectionHeader, *strtabHeader;
    Elf32_Sym *symbol_symtab;
    int num_entries;

    /* Jumping to the symtab entry directly */
    sectionHeader = getElfSectionHdr(elf_ptr, sym_index);

    /* Reading from the String Table linked to ot */
    strtabHeader = getElfSectionHdr(elf_ptr, sectionHeader->sh_link);

    stringTable = elf_ptr + strtabHeader->sh_offset;

    num_entries = sectionHeader->sh_size/sizeof(Elf32_Sym);
    printf("Printing Symtab entries\n");
    printf("Idx  Sym_name  Section/Type\n");

    for(int index = 0; index < num_entries ; index++)
    {
        symbol_symtab = getElfSym(elf_ptr, sectionHeader->sh_offset, index);

        if(index < 10)
            printf("( %d) - %s ", index, stringTable + symbol_symtab->st_name);
        else
            printf("(%d) - %s ", index, stringTable + symbol_symtab->st_name);

        print_symbol_info(symbol_symtab->st_shndx);
        printf("\n");
    }

    print_break();
}

/* Print the value and name of the symbol in the symtab table */
/* Currently sort of redundant -- Find some optimized way to code this */
void print_sym_name(char * elf_ptr, int symtab_index, int strtab_index)
{
    Elf32_Shdr *sectionHeader, *strtabHeader;
    Elf32_Sym *symbol_symtab;
    char *stringTable;

    sectionHeader = getElfSectionHdr(elf_ptr, symtab_index);

    /* Reading from the String Table linked to it */
    strtabHeader = getElfSectionHdr(elf_ptr, sectionHeader->sh_link);

    stringTable = elf_ptr + strtabHeader->sh_offset;

    if(strtab_index >= strtabHeader->sh_size){
        printf("  Out of Bounds ");
        return ;
    }

    /* Moving to the Symbol table data */
    symbol_symtab = getElfSym(elf_ptr, sectionHeader->sh_offset,strtab_index);

    //printf("  %d  %s  ",symbol_symtab.st_value, stringTable + symbol_symtab.st_name);
   // printf("  %d  %d  ",symbol_symtab.st_value, symbol_symtab.st_name);

    if( symbol_symtab->st_shndx == SHN_UNDEF )
        printf(" -  Undefined Symbol  ");
    else if( symbol_symtab->st_shndx == SHN_COMMON)
        printf(" -  COMMON (Currently Unallocated)  ");
    else if (symbol_symtab->st_shndx == SHN_ABS)
        printf(" -  ABS (No relocation)  ");
    else
        printf(" - ** %d ** ",symbol_symtab->st_shndx);

    (void)stringTable;
}

/* Print the type of relocation (Currently a subset of all possible) */
void print_rel_type(Elf32_Word r_info)
{
    switch(ELF32_R_TYPE(r_info))
    {
        case R_386_NONE:
            printf("R_386_NONE No relocation");
            break;
        case R_386_32:
            printf("R_386_32 (Symbol + Offset)");
            break;
        case R_386_PC32:
            printf("R_386_PC32 (Symbol + Offset - Section Offset)");
            break;
        case R_386_GOT32:
            printf("R_386_GOT32 (G + A - P)");
            break;
        case R_386_PLT32:
            printf("R_386_PLT32 (L + A - P)");
            break;
        case R_386_COPY:
            printf("R_386_COPY (none)");
            break;
        case R_386_GLOB_DAT:
            printf("R_386_GLOB_DAT (S)");
            break;
        case R_386_JMP_SLOT:
            printf("R_386_JMP_SLOT (S)");
            break;
        case R_386_RELATIVE:
            printf("R_386_RELATIVE (B + A)");
            break;
        case R_386_GOTOFF:
            printf("R_386_GOTOFF (S + A - GOT)");
            break;
        case R_386_GOTPC:
            printf("R_386_GOTOFF (GOT + A - P)");
            break;
        default:
            printf("Unlisted/Unknown/Unsupported Relocation Type");// (%d)", ELF32_R_TYPE(r_info));
    }
    printf("  \n");
}

/* Print the symbol related relocation info from the relocation table (currently .rel only) */
void process_rel(char * elf_ptr, int rel_index)
{
    Elf32_Shdr *sectionHeader;
    Elf32_Rel *rel_entry;
    int num_entries, symtab_index;

    /* Jumping to the .rel table entry directly */
    sectionHeader = getElfSectionHdr(elf_ptr, rel_index);

    /* Reading the index of the Symbol Table linked to it */
    symtab_index = sectionHeader->sh_link;

    num_entries = sectionHeader->sh_size/sizeof(Elf32_Rel);

    printf("Printing %i Relocation entries\n", num_entries);
    printf("Idx   Offset   Info    Value   Name   Sec.Indx    Type\n");

    for(int index = 0; index < num_entries ; index++)
    {
        rel_entry = getElfRel(elf_ptr, sectionHeader->sh_offset, index);
        printf("[%2d] -  %" PRIx32 "  %" PRIx32 "   ", index, rel_entry->r_offset,  rel_entry->r_info);

        /* If symbol table index in null/not defined then take symbol value == STN_UNDEF*/
        if(ELF32_R_SYM(rel_entry->r_info) == STN_UNDEF)
            printf(" STN_UNDEF   ");
        else {
            puts("..");
            print_sym_name(elf_ptr, symtab_index, ELF32_R_SYM(rel_entry->r_info));
        }
        print_rel_type(ELF32_R_TYPE(rel_entry->r_info));
    }

    print_break();
}

void print_break(void)
{
   printf("\n\n-----------------------------\n\n");
}

/* Process the symbol table to locate the file address*/
#if 0
Elf32_Addr elf_locate(char * elf_ptr, char *file_ptr)
{
    Elf32_Ehdr *elfHeader;
    Elf32_Shdr *sectionHeader, *strtabHeader;
    Elf32_Sym *symbol_symtab;

    char *stringTable = NULL;
    int num_entries;

    elfHeader = (Elf32_Ehdr *) elf_ptr;

    /* Finding symtab index */
    for(int index = 0; index < elfHeader->e_shnum; index++)
    {
        sectionHeader = getElfSectionHdr(elf_ptr, index);
        if(sectionHeader->sh_type == SHT_SYMTAB){

            /* Reading from the String Table linked to it */
            strtabHeader = getElfSectionHdr(elf_ptr, sectionHeader->sh_link);

            stringTable = elf_ptr + strtabHeader->sh_offset;
            num_entries = sectionHeader->sh_size/sizeof(Elf32_Sym);

            for(int i = 0; i < num_entries ; i++)
            {
                symbol_symtab = getElfSym(elf_ptr, sectionHeader->sh_offset, i);

                /* Currently we use the array test_elf[] which has type STT_OBJECT -- Maybe be changed to generalize */
                if(ELF32_ST_TYPE(symbol_symtab->st_info) == STT_OBJECT)
                {
                    if(!strcmp(file_ptr, stringTable + symbol_symtab->st_name))
                    {
                        printf("Section Index- %d  Addr Value- %" PRIx32 "\n", symbol_symtab->st_shndx,symbol_symtab->st_value);
                        return symbol_symtab->st_value;
                    }
                }

            }
        }
    }

    return 0;
}
#endif