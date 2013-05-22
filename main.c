#include <stdio.h>

#include "repl.h"

int main(int argc, char *argv[])
{
    FILE* in;

    if (argc > 1)
        in = fopen(argv[1], "r");
    else
        in = stdin;

    repl(in);
    return 0;
}
