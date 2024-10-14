#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FILENAME "test.txt"
#define HEADER "My name is "

int main(int argc, char* argv[])
{
    char* file_as_mem;

    if (argc < 2) {
        printf("Usage: %s <name>\n", argv[0]);
        return 1;
    }

    int fd = open(FILENAME, O_RDWR | O_CREAT, (mode_t)0600);

    // need the file size to map the file
    struct stat filestat;
    fstat(fd, &filestat);
    int file_size = filestat.st_size;

    file_as_mem
        = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("Changing name to '%s'\n", argv[1]);
    strncpy(file_as_mem + strlen(HEADER), argv[1], strlen(argv[1]));

    msync(file_as_mem, file_size, MS_SYNC);
    munmap(file_as_mem, file_size);

    close(fd);
    return 0;
}
