#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char* str = "Hello Everyone!\n";

    if (argc < 2)
    {
        printf("Usage: %s <name>\n", argv[0]);
        return 1;
    }

    int fd = open("test.txt", O_WRONLY | O_TRUNC | O_CREAT, 0640);
    if (fd == -1)
        return (1);

    printf("fd = %d\n", fd);

    dprintf(fd, "My name is %s\n", argv[1]);

    write(fd, str, strlen(str));

    close(fd);

    return 0;
}
