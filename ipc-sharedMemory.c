#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h> // for shared memory
#include <stdbool.h>

/*
ipc: inter process communication
*/

/*
[result]
start child 1
proc 1 
-----
sample string
-----
proc 2 
main > fin
*/

static int s_segId; // segment id

static void proc_one(void)
{
    char *paddr;

    if (fork() == 0) {
        printf("start child 1\n");

        // attach
        if ((paddr = shmat(s_segId, NULL, 0)) == (void *)-1) {
            perror("child1 > shmat failed");
            exit(EXIT_FAILURE);
        }

        // set
        strcpy(paddr, "sample string\n");

        // detach
        if (shmdt(paddr) == -1) {
            perror("child1 > shmdt failed");
            exit(EXIT_FAILURE);
        }

        printf("proc 1 \n");
        exit(EXIT_SUCCESS);
    }
}

static void proc_two(void)
{
    char *paddr;

    if (fork() == 0) {
        sleep(1);

        // attach
        if ((paddr = shmat(s_segId, NULL, SHM_RDONLY)) == (void *)-1) {
            perror("child2 > shmat failed");
            exit(EXIT_FAILURE);
        }

        printf("-----\n");
        // get
        printf("%s", paddr);
        printf("-----\n");

        // detach
        if (shmdt(paddr) == -1) {
            perror("child2 > shmdt failed");
            exit(EXIT_FAILURE);
        }

        printf("proc 2 \n");
        exit(EXIT_SUCCESS);
    }
} 

int main(void)
{
    const int kSize = 100;

    if ( (s_segId = shmget(IPC_PRIVATE, kSize, IPC_CREAT | 0666)) 
         == -1 ){
        perror("main > shmget failed");
        exit(EXIT_FAILURE);
    }

    proc_one();
    proc_two();

    int loop;
    for (loop=0; loop < 2; loop++) {
        wait(NULL);
    }

    if (shmctl(s_segId, IPC_RMID, NULL) == -1){
        perror("main > shmctl failed");
        exit(EXIT_FAILURE);
    }

    printf("main > fin\n");

    return EXIT_SUCCESS;
}