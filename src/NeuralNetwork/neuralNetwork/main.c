#include <stdio.h>
#include <stdlib.h>
#include "XOR.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("ERROR");
        return 1;
    }
    unsigned long param = strtoul(argv[1], NULL, 10);
    XOR(param);
    return 0;
}
