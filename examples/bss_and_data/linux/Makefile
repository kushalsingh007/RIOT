all: elf_parser

elf_parser: elf_parser.c ../riot_elf.c
	gcc -I.. elf_parser.c ../riot_elf.c -o elf_parser -g

clean:
	rm elf_parser
