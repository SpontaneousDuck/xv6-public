// Kenneth Witham
// NUID 001068036

#include "types.h"
#include "user.h"

int
main(int argc, char **argv)
{
    int num = 42;
    
    printf(2, "Original Address: %x\n", &num);
    void* ptr1 = translate(&num); // call the date system call
    printf(2, "Physical Address: %x\n", ptr1);
    void* ptr2 = rtranslate(&ptr1); // call the date system call
    printf(2, "Virtual Address: %x\n", ptr2);
    exit();
}