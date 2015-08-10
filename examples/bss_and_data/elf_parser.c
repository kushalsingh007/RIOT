#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "elf.h"
#define MAX_SIZE 100000000

void list_shdr_table_entries(char *);
void list_symbol_info(char *);
void print_symbol_info(Elf32_Half);
void process_symtab(char *, Elf32_Ehdr, int);
void print_rel_type(Elf32_Word);
void print_sym_name(char *, Elf32_Ehdr, int, int);
void process_rel(char *, Elf32_Ehdr, int );
void print_break(void);

char **section_name;
int count;
char file_buff[MAX_SIZE];

int elf_parse(Elf32_Addr elf_addr)
{

    return 0;
}

/* Print all the Section Table Header entries */
void list_shdr_table_entries(char * elf_ptr)
{
    Elf32_Ehdr elfHeader;
    Elf32_Shdr sectionHeader;
    char *stringTable = NULL;

    /* Read the Elf header from Elf File */
    memcpy(&elfHeader, elf_ptr, sizeof(elfHeader));
 
    /* Jump and read from String table index of Section Header Table */
    memcpy(&sectionHeader, elf_ptr + elfHeader.e_shoff + elfHeader.e_shstrndx * elfHeader.e_shentsize, sizeof(sectionHeader));
    stringTable = malloc(sectionHeader.sh_size);

    /* Put the contents of the string table in the array */
    memcpy(stringTable, elf_ptr + sectionHeader.sh_offset, sectionHeader.sh_size);
//    printf("Printing String Table Entries\n");

    /* Initializing array for storing section names */
    section_name = malloc(elfHeader.e_shnum * sizeof(char *));

    /* Print the names of all the sections and process symbol table */
    for(int index=0; index < elfHeader.e_shnum; index++)
    {
        memcpy(&sectionHeader,elf_ptr + elfHeader.e_shoff + index * elfHeader.e_shentsize, sizeof(sectionHeader));
        section_name[count] = malloc(sizeof(char)*(strlen(stringTable + sectionHeader.sh_name)+1));
        strcpy(section_name[count++],stringTable + sectionHeader.sh_name);

/*      Disasble the output for now
        if(index<10)
            printf("[ %d] %s\n",index, stringTable + sectionHeader.sh_name);
        else
            printf("[%d] %s\n",index, stringTable + sectionHeader.sh_name);
 */
    }

    print_break();
    free(stringTable);

}

/* Prints the symbol table and the relocation related info */
void list_symbol_info(char * elf_ptr)
{
    Elf32_Ehdr elfHeader;
    Elf32_Shdr sectionHeader;

    memcpy(&elfHeader, elf_ptr, sizeof(elfHeader));
    /* Finding index of position of symtab */
    for(int index = 0; index < elfHeader.e_shnum; index++)
    {
        memcpy(&sectionHeader, elf_ptr + elfHeader.e_shoff + index * elfHeader.e_shentsize, sizeof(sectionHeader));

        if(sectionHeader.sh_type == SHT_SYMTAB)
            process_symtab(elf_ptr, elfHeader, index);
        else if (sectionHeader.sh_type == SHT_REL)
            process_rel(elf_ptr, elfHeader, index);
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
        printf(" -  %s  ",section_name[shndx]);
}

/* Process the symbol table to give symbol name and it's section type */
void process_symtab(char * elf_ptr, Elf32_Ehdr elfHeader, int sym_index)
{
    char *stringTable = NULL;
    Elf32_Shdr sectionHeader,strtabHeader;
    Elf32_Sym symbol_symtab;
    int num_entries;

    /* Jumping to the symtab entry directly */
    memcpy(&sectionHeader, elf_ptr + elfHeader.e_shoff + sym_index * elfHeader.e_shentsize, sizeof(sectionHeader));

    /* Reading from the String Table linked to it */
    memcpy(&strtabHeader, elf_ptr + elfHeader.e_shoff + sectionHeader.sh_link * elfHeader.e_shentsize, sizeof(strtabHeader));

    stringTable = malloc(strtabHeader.sh_size);
    memcpy(stringTable, elf_ptr + strtabHeader.sh_offset, strtabHeader.sh_size);

    num_entries = sectionHeader.sh_size/sizeof(Elf32_Sym);
    printf("Printing Symtab entries\n");
    printf("Idx  Sym_name  Section/Type\n");

    for(int index = 0; index < num_entries ; index++)
    {
        memcpy(&symbol_symtab, elf_ptr + sectionHeader.sh_offset + index * sizeof(symbol_symtab), sizeof(symbol_symtab));

        if(index < 10)
            printf("( %d) - %s ", index, stringTable + symbol_symtab.st_name);
        else
            printf("(%d) - %s ", index, stringTable + symbol_symtab.st_name);

        print_symbol_info(symbol_symtab.st_shndx);
        printf("\n");
    }

    print_break();
    free(stringTable);

}

/* Print the value and name of the symbol in the symtab table */
/* Currently sort of redundant -- Find some optimized way to code this */
void print_sym_name(char * elf_ptr, Elf32_Ehdr elfHeader, int symtab_index, int strtab_index)
{
    Elf32_Shdr sectionHeader,strtabHeader;
    Elf32_Sym symbol_symtab;
    char *stringTable;

    memcpy(&sectionHeader, elf_ptr + elfHeader.e_shoff + symtab_index * elfHeader.e_shentsize, sizeof(sectionHeader));

    /* Reading from the String Table linked to it */
    memcpy(&strtabHeader, elf_ptr + elfHeader.e_shoff + sectionHeader.sh_link * elfHeader.e_shentsize, sizeof(strtabHeader));

    stringTable = malloc(strtabHeader.sh_size);
    memcpy(stringTable, elf_ptr + strtabHeader.sh_offset, strtabHeader.sh_size);

    if(strtab_index >= strtabHeader.sh_size){
        printf("  Out of Bounds ");
        free(stringTable);
        return ;
    }

    /* Moving to the Symbol table data */
     memcpy(&symbol_symtab, elf_ptr + sectionHeader.sh_offset + strtab_index * sizeof(symbol_symtab), sizeof(symbol_symtab));

    //printf("  %d  %s  ",symbol_symtab.st_value, stringTable + symbol_symtab.st_name);
   // printf("  %d  %d  ",symbol_symtab.st_value, symbol_symtab.st_name);

    if( symbol_symtab.st_shndx == SHN_UNDEF )
        printf(" -  Undefined Symbol  ");
    else if( symbol_symtab.st_shndx == SHN_COMMON)
        printf(" -  COMMON (Currently Unallocated)  ");
    else if (symbol_symtab.st_shndx == SHN_ABS)
        printf(" -  ABS (No relocation)  ");
    else
        printf(" - ** %d ** ",symbol_symtab.st_shndx);

    free(stringTable);
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
void process_rel(char * elf_ptr, Elf32_Ehdr elfHeader, int rel_index)
{
    Elf32_Shdr sectionHeader;
    Elf32_Rel rel_entry;
    int num_entries, symtab_index;

    /* Jumping to the .rel table entry directly */
    memcpy(&sectionHeader, elf_ptr + elfHeader.e_shoff + rel_index * elfHeader.e_shentsize, sizeof(sectionHeader));

    /* Reading the index of the Symbol Table linked to it */
    symtab_index = sectionHeader.sh_link;

    num_entries = sectionHeader.sh_size/sizeof(Elf32_Rel);
    printf("Printing Relocation entries\n");
    printf("Idx   Offset   Info    Value   Name   Sec.Indx    Type\n");

    for(int index = 0; index < num_entries ; index++)
    {
        memcpy(&rel_entry, elf_ptr + sectionHeader.sh_offset + index * sizeof(rel_entry) , sizeof(rel_entry));
        if(index < 10)
            printf("[ %d] -  %" PRIx32 "  %" PRIx32 "   ", index, rel_entry.r_offset,  rel_entry.r_info);
        else
            printf("[%d] -  %" PRIx32 "  %" PRIx32 "   ", index, rel_entry.r_offset,  rel_entry.r_info);

        /* If symbol table index in null/not defined then take symbol value == 0*/
        if(ELF32_R_SYM(rel_entry.r_info) == STN_UNDEF)
            printf(" STN_UNDEF   ");
       else
            print_sym_name(elf_ptr, elfHeader, symtab_index, ELF32_R_SYM(rel_entry.r_info));
        print_rel_type(ELF32_R_TYPE(rel_entry.r_info));
    }

    print_break();
}

void print_break(void)
{
   printf("\n\n-----------------------------\n\n");
}

/* Process the symbol table to locate the file address*/

Elf32_Addr elf_locate(char * elf_ptr, char *file_ptr)
{
    Elf32_Ehdr elfHeader;
    Elf32_Shdr sectionHeader, strtabHeader;
    Elf32_Sym symbol_symtab;

    char *stringTable = NULL;
    int num_entries;

    /* Read the Elf header from Elf File */
    memcpy(&elfHeader, elf_ptr, sizeof(elfHeader));

    /* Finding symtab index */
    for(int index = 0; index < elfHeader.e_shnum; index++)
    {
        memcpy(&sectionHeader, elf_ptr + elfHeader.e_shoff + index * elfHeader.e_shentsize, sizeof(sectionHeader));
        if(sectionHeader.sh_type == SHT_SYMTAB){

            /* Reading from the String Table linked to it */
            memcpy(&strtabHeader, elf_ptr + elfHeader.e_shoff + sectionHeader.sh_link * elfHeader.e_shentsize, sizeof(strtabHeader));

            stringTable = malloc(strtabHeader.sh_size);
            memcpy(stringTable,elf_ptr + strtabHeader.sh_offset, strtabHeader.sh_size);
            num_entries = sectionHeader.sh_size/sizeof(Elf32_Sym);

            for(int i = 0; i < num_entries ; i++)
            {
                memcpy(&symbol_symtab, elf_ptr + sectionHeader.sh_offset + i * sizeof(symbol_symtab), sizeof(symbol_symtab));

                /* Currently we use the array test_elf[] which has type STT_OBJECT -- Maybe be changed to generalize */
                if(ELF32_ST_TYPE(symbol_symtab.st_info) == STT_OBJECT)
                {
                    if(!strcmp(file_ptr, stringTable + symbol_symtab.st_name))
                    {
                        printf("Section Index- %d \n", symbol_symtab.st_shndx);
                        free(stringTable);
                        return symbol_symtab.st_value;
                    }
                }

            }
        }
    }

    free(stringTable);
    return 0;
}

int main()
{
    /* Can optimize this part based on use of malloc or optimized read */
    int fd, file_size;
    Elf32_Addr elf_addr;

    fd = open("/home/kushal/kaspar/RIOT/examples/bss_and_data/bin/samr21-xpro/app-test.elf",O_RDONLY);
    file_size = 0;

    if(fd == -1){
        printf("Error : Unable to open the file\n");
        return 1;;
    }
     
    while (read(fd, &file_buff[file_size++], sizeof(char)));
    list_shdr_table_entries(file_buff);
    elf_addr = elf_locate(file_buff,"test_elf");
    printf("Value- %" PRIx32 "\n", elf_addr);

    if(elf_addr != 0){
        if(elf_parse(elf_addr)){
            printf("Error in parsing the elf file\n");
            return 1;
        }
    }

    list_symbol_info(file_buff);
    close(fd);
    return 0;
}