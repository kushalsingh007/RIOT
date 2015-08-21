all: linux

linux: linux.c
	gcc -I../include/ linux.c ../riot_elf.c -o linux -g

clean:
	rm linux
