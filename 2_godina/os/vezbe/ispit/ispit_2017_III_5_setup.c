#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <semaphore.h>

#define check_error(expr, user_msg) \
    do { \
        if (!(expr)) { \
            perror(user_msg); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

#define ARRAY_MAX (1024)

typedef struct {
    sem_t inDataReady;
    int array[ARRAY_MAX];
    unsigned arrayLen;
}OsInputData;

void* create_shm_data(const char* pathname, int size)
{
    int fd = shm_open(pathname, O_RDWR | O_CREAT, 0644);
    check_error(fd != -1, "shm_open");

    check_error(ftruncate(fd, size) != -1, "ftruncate");

    void* addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    check_error(addr != MAP_FAILED, "mmap");

    close(fd);

    return addr;
}

int main(int argc, const char *argv[])
{
    OsInputData* data = create_shm_data(argv[1], sizeof (OsInputData));

    check_error(sem_init(&(data->inDataReady), 1, 0) != -1, "sem_init");

    scanf("%u", &(data->arrayLen));
    for (int i = 0; i < data->arrayLen; i++) {
        scanf("%d", &(data->array[i]));
    }

    check_error(sem_post(&(data->inDataReady)) != -1, "sem_post");
    
    check_error(munmap(data, sizeof (OsInputData)) != -1, "munmap");
    exit(EXIT_SUCCESS);
}
