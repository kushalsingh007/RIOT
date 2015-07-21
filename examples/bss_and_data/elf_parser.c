#include <elf.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void list_shdr_table_entries(FILE *, Elf32_Ehdr);
void list_symbol_info(FILE *, Elf32_Ehdr);
void print_symbol_info(Elf32_Half);
void process_symtab(FILE *, Elf32_Ehdr, int);
void print_rel_type(Elf32_Word);
void print_sym_name(FILE *, Elf32_Ehdr, int, int);
void process_rel(FILE *, Elf32_Ehdr, int );
void print_break(void);

char **section_name;
int count;

/* Print all the Section Table Header entries */
void list_shdr_table_entries(FILE *ElfFile, Elf32_Ehdr elfHeader)
{
    Elf32_Shdr sectionHeader;
    char *stringTable = NULL;

    /* Jump to String table index of Section Header Table (w.r.t the start of the file) */
    fseek(ElfFile, elfHeader.e_shoff + elfHeader.e_shstrndx * elfHeader.e_shentsize, SEEK_SET);

    /* Reading Section Header for String Table and adding the contents in stringTable array */
    fread(&sectionHeader, 1, sizeof(sectionHeader), ElfFile);
    fseek(ElfFile, sectionHeader.sh_offset, SEEK_SET);

    stringTable = malloc(sectionHeader.sh_size);
    fread(stringTable, 1, sectionHeader.sh_size, ElfFile);
    printf("Printing String Table Entries\n");

    /* Print the names of all the sections and process symbol table */
    for(int index=0; index < elfHeader.e_shnum; index++)
    {
        fseek(ElfFile, elfHeader.e_shoff + index * elfHeader.e_shentsize, SEEK_SET);
        fread(&sectionHeader, 1, sizeof(sectionHeader), ElfFile);

        section_name[count] = malloc(sizeof(char)*(strlen(stringTable + sectionHeader.sh_name)+1));
        strcpy(section_name[count++],stringTable + sectionHeader.sh_name);

        if(index<10)
            printf("[ %d] %s\n",index, stringTable + sectionHeader.sh_name);
        else
            printf("[%d] %s\n",index, stringTable + sectionHeader.sh_name);
    }

    print_break();
    free(stringTable);

}

/* Prints the symbol table and the relocation related info */
void list_symbol_info(FILE *ElfFile, Elf32_Ehdr elfHeader)
{
    Elf32_Shdr sectionHeader;

    /* Finding index of position of symtab */
    for(int index = 0; index < elfHeader.e_shnum; index++)
    {
        fseek(ElfFile, elfHeader.e_shoff + index * elfHeader.e_shentsize, SEEK_SET);
        fread(&sectionHeader, 1, sizeof(sectionHeader), ElfFile);

        if(sectionHeader.sh_type == SHT_SYMTAB)
            process_symtab(ElfFile, elfHeader, index);
        else if (sectionHeader.sh_type == SHT_REL)
            process_rel(ElfFile, elfHeader, index);
    }

}

/* Prints the section associated with the symbol */
void print_symbol_info(Elf32_Half shndx)
{
    if( shndx == SHN_UNDEF )
        printf(" -  Undefined Symbol\n");
    else if( shndx == SHN_COMMON)
        printf(" -  COMMON (Currently Unallocated)\n");
    else if (shndx == SHN_ABS)
        printf(" -  ABS (No relocation)\n");
    else
        printf(" -  %s\n",section_name[shndx]);
}

/* Process the symbol table to give symbol name and it's section type */
void process_symtab(FILE *ElfFile, Elf32_Ehdr elfHeader, int index)
{
    char *stringTable = NULL;
    Elf32_Shdr sectionHeader,strtabHeader;
    Elf32_Sym symbol_symtab;
    int num_entries;

    /* Jumping to the symtab entry directly */
    fseek(ElfFile, elfHeader.e_shoff + index * elfHeader.e_shentsize, SEEK_SET);
    fread(&sectionHeader, 1, sizeof(sectionHeader), ElfFile);

    /* Reading from the String Table linked to it */
    fseek(ElfFile, elfHeader.e_shoff + sectionHeader.sh_link * elfHeader.e_shentsize, SEEK_SET);
    fread(&strtabHeader, 1, sizeof(strtabHeader), ElfFile);

    stringTable = malloc(strtabHeader.sh_size);
    fseek(ElfFile, strtabHeader.sh_offset, SEEK_SET);
    fread(stringTable, 1, strtabHeader.sh_size, ElfFile);

    /* Moving to the Symbol table data */
    fseek(ElfFile, sectionHeader.sh_offset, SEEK_SET);

    num_entries = sectionHeader.sh_size/sizeof(Elf32_Sym);
    printf("Printing Symtab entries\n");
    printf("Idx  Sym_name  Section/Type\n");

    for(int i = 0; i < num_entries ; i++)
    {
        fread(&symbol_symtab, 1, sizeof(symbol_symtab), ElfFile);

        if(i < 10)
            printf("( %d) - %s ", i, stringTable + symbol_symtab.st_name);
        else
            printf("(%d) - %s ", i, stringTable + symbol_symtab.st_name);

        print_symbol_info(symbol_symtab.st_shndx);
    }

    print_break();
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
            printf("Unlisted/Unknown/Unsupported Relocation Type (%d)", ELF32_R_TYPE(r_info));
	}
    printf("  \n");
}

/* Print the value and name of the symbol in the symtab table */
void print_sym_name(FILE *ElfFile, Elf32_Ehdr elfHeader, int symtab_index, int index)
{
    Elf32_Shdr sectionHeader,strtabHeader;
    Elf32_Sym symbol_symtab;
    char *stringTable;

    fseek(ElfFile, elfHeader.e_shoff + symtab_index * elfHeader.e_shentsize, SEEK_SET);
    fread(&sectionHeader, 1, sizeof(sectionHeader), ElfFile);

    /* Reading from the String Table linked to it */
    fseek(ElfFile, elfHeader.e_shoff + sectionHeader.sh_link * elfHeader.e_shentsize, SEEK_SET);
    fread(&strtabHeader, 1, sizeof(strtabHeader), ElfFile);

    stringTable = malloc(strtabHeader.sh_size);
    fseek(ElfFile, strtabHeader.sh_offset, SEEK_SET);
    fread(stringTable, 1, strtabHeader.sh_size, ElfFile);

    /* Moving to the Symbol table data */
    fseek(ElfFile, sectionHeader.sh_offset, SEEK_SET);

    for(int i=0; i <= index; i++)
     fread(&symbol_symtab, 1, sizeof(symbol_symtab), ElfFile);

    printf("  %d  %s  ",symbol_symtab.st_value, stringTable + symbol_symtab.st_name);

    free(stringTable);
}

/* Print the symbol related relocation info from the relocation table (currently .rel only) */
void process_rel(FILE *ElfFile, Elf32_Ehdr elfHeader, int index)
{
    Elf32_Shdr sectionHeader;
    Elf32_Rel rel_entry;
    int num_entries, symtab_index;
    fpos_t current_pos;

    /* Jumping to the .rel table entry directly */
    fseek(ElfFile, elfHeader.e_shoff + index * elfHeader.e_shentsize, SEEK_SET);
    fread(&sectionHeader, 1, sizeof(sectionHeader), ElfFile);

    /* Reading the index of the Symbol Table linked to it */
    symtab_index = sectionHeader.sh_link;

    /* Moving to the Relocation (.rel) table data */
    fseek(ElfFile, sectionHeader.sh_offset, SEEK_SET);

    num_entries = sectionHeader.sh_size/sizeof(Elf32_Rel);
    printf("Printing Relocation entries\n");
    printf("Idx   Offset   Info    Value   Name   Type\n");

    for(int i = 0; i < num_entries ; i++)
    {
        fread(&rel_entry, 1, sizeof(rel_entry), ElfFile);
        fgetpos(ElfFile, &current_pos);
        if(i < 10)
            printf("[ %d] -  %" PRIx32 "  %" PRIx32 "   ", i, rel_entry.r_offset,  rel_entry.r_info);
        else
            printf("[%d] -  %" PRIx32 "  %" PRIx32 "   ", i, rel_entry.r_offset,  rel_entry.r_info);

        print_sym_name(ElfFile, elfHeader, symtab_index, ELF32_R_SYM(rel_entry.r_info));
        print_rel_type(ELF32_R_TYPE(rel_entry.r_info));
        fsetpos(ElfFile, &current_pos);
    }

    print_break();
}

void print_break(void)
{
   printf("\n\n-----------------------------\n\n");
}

int main(int argc, char** argv)
{
    FILE *ElfFile = NULL;
    Elf32_Ehdr elfHeader;

    ElfFile = fopen("/home/kushal/mind/RIOT/tests/struct_tm_utility/bin/native/struct_tm_utility.elf","rb");

    if(ElfFile == NULL)
    {
        printf("Error : Unable to open the file\n");
        return 1;;
    }

    /* Read the Elf header from Elf File */
    fread(&elfHeader, 1, sizeof(elfHeader), ElfFile);

    /* Initializing array for storing section names */
    section_name = malloc(elfHeader.e_shnum * sizeof(char *));

    /* Process the symbol table and symtab */
    list_shdr_table_entries(ElfFile, elfHeader);
    list_symbol_info(ElfFile, elfHeader); 

    for(int i =0;i<count;i++)
        free(section_name[i]);
    free(section_name);

    fclose(ElfFile);

return 0;
}
