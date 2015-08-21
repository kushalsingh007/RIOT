#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "riot_elf.h"

#define R_SIZE 100

Elf32_Shdr* getElfSectionHdr(char *elf_ptr, int index)
{return (Elf32_Shdr *) (elf_ptr + ((Elf32_Ehdr *) elf_ptr)->e_shoff + index *((Elf32_Ehdr *) elf_ptr)->e_shentsize);}

Elf32_Sym* getElfSym(char *elf_ptr, Elf32_Off sh_offset, int index)
{return (Elf32_Sym *) (elf_ptr + sh_offset + index * sizeof(Elf32_Sym));}

Elf32_Rel* getElfRel(char *elf_ptr, Elf32_Off sh_offset, int index)
{return (Elf32_Rel *) (elf_ptr + sh_offset + index * sizeof(Elf32_Rel));}

rel_array r_array[R_SIZE];
int count;

int match_section(char *elf_ptr, char *shname, int shndx)
{
    for(int index = 0; index < ((Elf32_Ehdr *)elf_ptr)->e_shnum ; index++)
    {
        Elf32_Shdr *shdr= (Elf32_Shdr *) getElfSectionHdr(elf_ptr, index);
        Elf32_Shdr *strhdr = (Elf32_Shdr *) getElfSectionHdr(elf_ptr, ((Elf32_Ehdr *)elf_ptr)->e_shstrndx);

        if((strcmp(elf_ptr + strhdr->sh_offset + shdr->sh_name, shname)>=0) && (index == shndx))
   			{printf("%s\n"	,elf_ptr + strhdr->sh_offset + shdr->sh_name);return index;}
    }

    return 0;
}

void elf_process_symtab(char *elf_ptr, int sym_index)
{
    Elf32_Shdr *shdr = (Elf32_Shdr *) getElfSectionHdr(elf_ptr, sym_index);
    int num_entries = shdr->sh_size/sizeof(Elf32_Sym);

    for(int index = 0; index < num_entries ; index++)
    {
        Elf32_Sym *symbol = (Elf32_Sym *) getElfSym(elf_ptr, shdr->sh_offset, index);
            
        /* Add condition for weak symbol later (if needed) */
        if((ELF32_ST_TYPE(symbol->st_info) == STT_FUNC) && (ELF32_ST_BIND(symbol->st_info) == STB_GLOBAL) \
            && (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)){

        		if(match_section(elf_ptr,".text",symbol->st_shndx)){
	                r_array[count].name = elf_ptr + getElfSectionHdr(elf_ptr, shdr->sh_link)->sh_offset + symbol->st_name;
	                r_array[count].addr = symbol->st_value;
	                count+=1;
            	}
        }

    }
}

Elf32_Addr compute_imd(Elf32_Addr reg)
{
	Elf32_Addr imm8 = (0xFF & (reg));
	Elf32_Addr imm3 = (0x7000 & (reg))>>12;
	Elf32_Addr i = ((1<< 27) & (reg))>>26;
	Elf32_Addr imm4 = ((0xF0000) & (reg))>>16;
	return ((imm4<<12) + (i<<11) + (imm3<<8) + (imm8));
}

Elf32_Addr elf_jmp_got( Elf32_Addr* plt_jmp)
{
	/* Getting last 16 bits from movw instruction */
	Elf32_Addr movw_imm = compute_imd(((*plt_jmp) & (0xFFFF0000))>>16) + (((*plt_jmp) & (0xFFFF))<<16);
	plt_jmp+=1;

	/* Getting first 16 bits from the movw instruction */
	Elf32_Addr movt_imm = compute_imd(((*plt_jmp) & (0xFFFF0000))>>16) + (((*plt_jmp) & (0xFFFF))<<16);
	plt_jmp+=1;
	/* Add pc+4 to the current value so make address pc relative*/
	return ((movt_imm<<16)|((movw_imm)&(0xFFFF))) + (Elf32_Addr)0x4 + (Elf32_Addr)plt_jmp;

}

void elf_modify_instruction(char *elf_ptr)
{
	for( int index=0; index < ((Elf32_Ehdr *)elf_ptr)->e_shnum; index++){
	    int text_idx = match_section(elf_ptr,".text",index);
	    
	    if(text_idx){
		    Elf32_Shdr* shdr = getElfSectionHdr(elf_ptr,text_idx);
		    Elf32_Half* start = (Elf32_Half *)(elf_ptr + shdr->sh_offset);
		    Elf32_Addr jump_addr=0;
		    while(start < (Elf32_Half *)(elf_ptr + shdr->sh_offset + shdr->sh_size))
		    {
		        if(!(((char)0xf)^((*start)>>12)))
		        	{printf("GOt you bro \n");
		        	 if(((char)1)&((*start)>>11)){


							printf("1 Bit\n");
							jump_addr += (((*start)&((Elf32_Half)0x7FF))<<1);
							printf("Complete jump addr %"PRIx32"\n",(long unsigned)jump_addr);
							printf("PC VALUE high___%"PRIx32"\n",(long unsigned)start);

							printf("Jump %"PRIx32"\n",(long unsigned)jump_addr);

							Elf32_Addr my_got = elf_jmp_got((Elf32_Addr *)jump_addr);


							printf("INSIDE GOT Entry %"PRIx32"\n",*(Elf32_Addr *)my_got);
							printf("ELF_PTR%"PRIx32"\n", (Elf32_Addr)elf_ptr);
							printf("%"PRIx32"\n",(Elf32_Addr)(start+(Elf32_Half)0x4));

							Elf32_Half bl_high_addr =(((*start)>>11)<<11)|(((Elf32_Half)(((*(Elf32_Addr *)my_got - (Elf32_Addr)0x1 - (Elf32_Addr)(start) - (Elf32_Addr)(0x2))))>>1)&(0x7FF));
							memcpy(start,&bl_high_addr,sizeof(Elf32_Half));
							
							printf("Elf - %"PRIx32"\n",(Elf32_Addr)elf_ptr);
							printf("my_got %"PRIx32"\n",*((Elf32_Addr *)(my_got)));
							printf("my_sum %"PRIx32"\n",*((Elf32_Addr *)(my_got))+(Elf32_Addr)elf_ptr );


							printf("Yo THIS THE IMP %"PRIx32" and start was %"PRIx32"\n",(((*(Elf32_Addr *)my_got - (Elf32_Addr)(start) - (Elf32_Addr)(0x2)))),(long unsigned)*start);

							start--;
							printf("my_sym %"PRIx32"\n",((Elf32_Addr )(start)+0x4));

							Elf32_Half bl_low_addr =((Elf32_Half)((*start)>>11)<<11)|(((Elf32_Half)(((*(Elf32_Addr *)my_got - (Elf32_Addr)(start) - (Elf32_Addr)(0x4))))>>12)&(0x07FF));
							memcpy(start,&bl_low_addr,sizeof(Elf32_Half));
							printf("my_sum %"PRIx32"\n",*((Elf32_Addr *)(my_got))+(Elf32_Addr)elf_ptr - (Elf32_Addr)(start) - (Elf32_Addr)(0x4));


							printf("Hi- bl_addr %"PRIx16" and value is --  was %lx\n",(Elf32_Half)bl_low_addr,(Elf32_Addr)bl_high_addr);
							//Elf32_Half high_half = (((*start)>>11)<<11)|();
							start++;
							jump_addr=0;

					}
					else{
						printf("0 Bit\n");
						jump_addr += (Elf32_Addr)(start) + (Elf32_Addr)0x4;
						jump_addr += ((*start)&((Elf32_Half)0x7FF))<<12;

						printf("PC VALUE low___%"PRIx32"\n",(long unsigned)start +0x4);


						printf("%"PRIx16"\n", (unsigned)start);
							printf("High Part+Low+PC %"PRIx32"\n",(long unsigned)jump_addr);
					}
		        	}
		        start++;
		        printf("\n");
		    }


		}
	}

}

void elf_relocate_internal(char *elf_ptr)
{
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)elf_ptr;
    for(int index = 0; index < ehdr->e_shnum; index++)
    {
        Elf32_Shdr *shdr = (Elf32_Shdr *) getElfSectionHdr(elf_ptr, index);
        if(shdr->sh_type == SHT_DYNSYM) 
            elf_process_symtab(elf_ptr, index);
    }
    elf_modify_instruction(elf_ptr);
}
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
                printf("Off=%x ",(unsigned)(elf_ptr + sectionHeader->sh_offset));

        printf("index=%u sh_type=0x%08x\n", index, (unsigned)sectionHeader->sh_type);

        if(sectionHeader->sh_type == SHT_DYNSYM) {
            process_symtab(elf_ptr, index);
        }
        else if (sectionHeader->sh_type == SHT_REL) {
            printf("Return value - %d\n",elf_parse_rel(elf_ptr, index));
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
        printf(" -  %s  ","section based"); //section_name[shndx](TODO)
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

Elf32_Addr getElfSymval(char * elf_ptr, int symtab_index, int index)
{
    Elf32_Shdr *sectionHeader;
    Elf32_Sym *symbol;
    int symtab_entries;

    /* Get the section header info of the symbol table */
    sectionHeader = getElfSectionHdr(elf_ptr, symtab_index);

    /* Calculate the number of entries in the symbol table */
	symtab_entries = sectionHeader->sh_size / sectionHeader->sh_entsize;

    if(index >= symtab_entries){
        printf("  Index: Out of Bounds ");
        return -1;
    }

    /* Moving to the Symbol table data (symbol at the given index) */
    symbol = getElfSym(elf_ptr, sectionHeader->sh_offset,index);
	if(symbol->st_shndx == SHN_UNDEF) {
		/* External symbol, lookup value */
/*		Elf32_Shdr *strtabHeader = getElfSectionHdr(elf_ptr, sectionHeader->sh_link);
		char *sym_name = elf_ptr + strtab->sh_offset + symbol->st_name;
 
		extern void *elf_lookup_symbol(const char *name);
		Elf32_Addr target = elf_lookup_symbol(name);
		 Lookup the external symbol in the external symbol table and get the address 
 
		if(target == NULL) {
			// Extern symbol not found
			if(ELF32_ST_BIND(symbol->st_info) & STB_WEAK) {
				// Weak symbol initialized as 0
				return 0;
			} else {
				ERROR("Undefined External Symbol : %s.\n", name);
				return SOME_ERROR;
			}
		} else {
			return (int)target;
*/
		return 0;
	}
	else if(symbol->st_shndx == SHN_ABS) {
		/* The address of the symbol is absolute */
		return (Elf32_Addr ) symbol->st_value;
	} 
	else {
		/* Internally defined symbols - We deal with virtual addresses (shared file)*/
		return (Elf32_Addr )((Elf32_Addr)(elf_ptr) + symbol->st_value);
	}
}

int elf_parse_rel(char * elf_ptr, int rel_index)
{
    Elf32_Addr sym_value;
    Elf32_Shdr *sectionHeader;
    Elf32_Rel *rel_entry;
    int num_entries, symtab_index;
printf("Look ... here\n");
    /* Jumping to the .rel table entry directly */
    sectionHeader = getElfSectionHdr(elf_ptr, rel_index);

    /* Reading the index of the Symbol Table linked to it */
    symtab_index = sectionHeader->sh_link;

    num_entries = sectionHeader->sh_size/sizeof(Elf32_Rel);

	for(int index = 0; index < num_entries ; index++)
    {
        rel_entry = getElfRel(elf_ptr, sectionHeader->sh_offset, index);

        /* If symbol table index in null/not defined then take symbol value == STN_UNDEF*/
        if(ELF32_R_SYM(rel_entry->r_info) == STN_UNDEF){
            printf(" STN_UNDEF   ");
            return 1;
        }else {
		        print_rel_type(ELF32_R_TYPE(rel_entry->r_info));
				sym_value = getElfSymval(elf_ptr, symtab_index, ELF32_R_SYM(rel_entry->r_info));
			    if(sym_value == 0){
			        printf("Unable to get symbol value \n");
			        return 1;
			    }
	printf("Relocation to be applied when elf_ptr is %"PRIx32" and rel_entry->r_offset is %"PRIx32" and sum is Sum=%"PRIx32"\n"
		        		,(long unsigned)elf_ptr,rel_entry->r_offset,(long unsigned)elf_ptr+rel_entry->r_offset);
		       

			    printf("%"PRIx32"\n",sym_value);
			    /* Can add all the types needed based on need */
		        switch(rel_entry->r_info)
		        {
		        	/* Different types of relocations */
		        	/* Note: Right now default relocation only works */
		        	case(R_ARM_GLOB_DAT):{
		        	/* Dynamic - (S + A) | T */
			        	memcpy(elf_ptr + rel_entry->r_offset,&sym_value,sizeof(Elf32_Addr));
			        	break;
	        		}

	        		case(R_ARM_JUMP_SLOT):{
		        	/* Dynamic - (S + A) | T */
			        	memcpy(elf_ptr + rel_entry->r_offset,&sym_value,sizeof(Elf32_Addr));
			        	break;
	        		}

	        		default:
			        		memcpy(elf_ptr + rel_entry->r_offset,&sym_value,sizeof(Elf32_Addr));	        
	        	}

	        		printf("Relocation to be applied when elf_ptr is %"PRIx32" and rel_entry->r_offset is %"PRIx32" and sum is Value=%"PRIx32"\n"
		        		,(long unsigned)elf_ptr,rel_entry->r_offset,(long unsigned )(elf_ptr+rel_entry->r_offset));
		       
				sym_value = getElfSymval(elf_ptr, symtab_index, ELF32_R_SYM(rel_entry->r_info));
			    printf("LOL     -- %"PRIx32"\n",sym_value);

	        			printf("%lu\n",rel_entry->r_info );
	        	        print_rel_type(ELF32_R_TYPE(rel_entry->r_info));	

	        }

    }

    return 0;
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
    
			/* For .data section variable */
            puts("..");
            print_sym_name(elf_ptr, symtab_index, ELF32_R_SYM(rel_entry->r_info));
            printf("\n\n%"PRIx32"\n\n",(long unsigned)elf_ptr + getElfSymval(elf_ptr, symtab_index, ELF32_R_SYM(rel_entry->r_info)));
        }
        print_rel_type(ELF32_R_TYPE(rel_entry->r_info));
    }
 }