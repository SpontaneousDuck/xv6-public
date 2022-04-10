// Kenneth Witham
// NUID 001068036

#include "types.h"
#include "user.h"

#define NUM_THREADS 10

int main(int argc, char **argv)
{
    nice(-1);
    for (uint i = 0; i < NUM_THREADS; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            nice(NUM_THREADS-i);
            yield();

            printf(2, "%d ", NUM_THREADS-i);
            exit();
        }
    }
    
    yield();
    
    for (uint i = 0; i < NUM_THREADS; i++)
    {
        wait();
    }
    printf(2,"\nDone!\n");
    exit();
}