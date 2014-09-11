#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdbool.h>

/*
ipc: inter process communication

sample of
- fork()
- shared memory
*/

static int s_segId; // segment id

static bool proc_one(void)
{
    char *paddr;

    sleep(1);

    if (fork() == 0) {
        printf("start child 1\n");

        // attach
        if ((paddr = shmat(s_segId, NULL, 0)) == (void *)-1) {
            perror("child1 > shmat failed");
            return false;
        }

        strcpy(paddr, "sample string\n");
        // detach
        if (shmdt(paddr) == -1) {
            perror("child1 > shmdt failed");
            return false;
        }

        printf("proc 1 \n");
        return true;
    }
    return false;
}

static bool proc_two(void)
{
    char *paddr;

    sleep(1);

    if (fork() == 0) {
        // attach
        if ((paddr = shmat(s_segId, NULL, SHM_RDONLY)) == (void *)-1) {
            perror("child2 > shmat failed");
            return false;
        }

        printf("-----\n");
        printf("%s", paddr);
        printf("-----\n");

        // detach
        if (shmdt(paddr) == -1) {
            perror("child2 > shmdt failed");
            return false;
        }

        printf("proc 2 \n");
        return true;
    }
    return false;
}

int main(void)
{
    const int kSize = 100;

    if ( (s_segId = shmget(IPC_PRIVATE, kSize, IPC_CREAT | 0666)) 
         == -1){
        perror("main > shmget failed");
        exit(EXIT_FAILURE);
    }

    if (proc_one() == false) {
        exit(EXIT_FAILURE);
        return;
    }

    if (proc_two() == false) {
        exit(EXIT_FAILURE);
        return;
    }

    int loop;
    for (loop=0; loop < 2; loop++) {
        wait(NULL);
    }

    if (shmctl(s_segId, IPC_RMID, NULL) == -1){
        perror("main > shmctl failed");
        exit(EXIT_FAILURE);
    }

    printf("main > fin\n");

    // fflush(stdout);
    // fflush(stdout);
    return EXIT_SUCCESS;
}