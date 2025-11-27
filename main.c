#include <stdio.h>
#include "arg.h"
int main (int argcount, char *argvector[]) {
struct arguments opts;

if (get_arguments(argcount, argvector, &opts) != 0) {
    return 1;
}

printf("mode=%s, type of sort=%s, input file = %s", opts.mode, opts.type_of_sort, opts.input_file);

    return 0;
}