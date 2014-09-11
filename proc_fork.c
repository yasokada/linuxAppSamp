#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
[result]
child 2
child 1
child 2
child 1
child 2
child 1
main
*/

static void proc_one(void)
{
    int i;

    if (fork() == 0) {
        for(i=0; i<3; i++) {
            sleep(1);
            printf("child 1\n");
        }
        exit(EXIT_SUCCESS);
    }
}

static void proc_two(void)
{
    int i;

    if (fork() == 0) {
        for(i=0; i<3; i++) {
            sleep(1);
            printf("child 2\n");
        }
        exit(EXIT_SUCCESS);
    }
}

int main(void)
{
    proc_one();
    proc_two();

    int loop;
    for (loop=0; loop < 2; loop++) {
        wait(NULL);
    }

    printf("main\n");

    return EXIT_SUCCESS;
}