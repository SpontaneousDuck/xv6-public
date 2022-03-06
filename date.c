// Kenneth Witham
// NUID 001068036

#include "types.h"
#include "date.h"
#include "user.h"

int
main(int argc, char **argv)
{
    struct rtcdate d;
    
    date(&d); // call the date system call
    printf(2, "%d/%d/%d %d:%d:%d\n", d.month, d.day, d.year, d.hour, d.minute, d.second); // print results
    exit();
}