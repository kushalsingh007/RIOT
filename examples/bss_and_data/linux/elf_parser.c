#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "riot_elf.h"

char *file_buff = NULL;

int main(int argc, const char* argv[])
{
    int fd, file_size;
    Elf32_Addr elf_addr;
    struct stat st;

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

    if(stat(argv[1], &st) == -1){
        fprintf(stderr, "Error : Unable to determine file size\n");
        return 1;
    }

    file_buff = malloc((st.st_size + 1));

    if(file_buff == NULL){
        fprintf(stderr, "Error : Unable to allocate memory\n");
        return 1;
    }

    file_size = read(fd, file_buff, st.st_size);

    if (file_size <=0) {
        fprintf(stderr, "Error : Unexpected elf file size\n");
        return 1;
    }

    printf("read %u bytes\n", file_size);

    list_symbol_info(file_buff);

    free(file_buff);
    close(fd);

    return 0;
}
