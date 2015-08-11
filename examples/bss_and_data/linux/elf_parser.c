#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "riot_elf.h"

#define MAX_SIZE 100000000

char file_buff[MAX_SIZE];

int main(int argc, const char* argv[])
{
    /* Can optimize this part based on use of malloc or optimized read */
    int fd, file_size;
    Elf32_Addr elf_addr;

    if (argc < 2) {
        fprintf(stderr, "usage: elf_parse <elf-file>\n");
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    file_size = 0;

    if(fd == -1){
        fprintf(stderr, "Error : Unable to open the file\n");
        return 1;
    }

    file_size = read(fd, file_buff, MAX_SIZE);

    if (file_size <=0) {
        fprintf(stderr, "Error : unexpected elf file size\n");
        return 1;
    }

    printf("read %u bytes\n", file_size);
    elf_addr = elf_locate(file_buff, "test_elf" );
    printf("Value- %" PRIx32 "\n", elf_addr);

    close(fd);

    return 0;
}
