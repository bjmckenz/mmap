#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define OBJECT_NAME "/ShareMeBaby"
#define SHARED_MEM_SIZE 1024
#define CMDLEN 256

int main(int argc, char* argv[])
{
    char buffer[CMDLEN];

    if (argc > 1 && !strcmp("-del", argv[1])) {
        shm_unlink(OBJECT_NAME);
        printf("Shared memory object '%s' deleted\n", OBJECT_NAME);
        return 0;
    }

    // get the shared memory object ("file-like")
    int fd = shm_open(OBJECT_NAME, O_RDWR | O_CREAT, 0640);
    ftruncate(fd, SHARED_MEM_SIZE);

    // Now map it to memory for convenience
    char *obj = mmap (NULL, SHARED_MEM_SIZE,
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        // delete included newline
        buffer[strlen(buffer)-1] = '\0';
        if (!strncmp(buffer, "exit", strlen("exit")))
            break;
        if (!strncmp(buffer, "print", strlen("print"))) {
            printf("-> %s\n", obj);
            continue;
        }
        if (!strncmp(buffer, "set", strlen("set"))) {
            strcpy(obj, buffer + strlen("set")+1);
            continue;
        }
        if (!strncmp(buffer, "add", strlen("add"))) {
            sprintf(obj+strlen(obj), "\n%s", buffer + strlen("add")+1);
            continue;
        }
        printf("Unknown command\n");
    }

    msync(obj, SHARED_MEM_SIZE, MS_SYNC);
    munmap(obj, SHARED_MEM_SIZE);

    close(fd);
    shm_unlink(OBJECT_NAME);

    return 0;
}
