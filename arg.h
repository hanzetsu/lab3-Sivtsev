#pragma once

struct arguments
{
    char *mode;
    unsigned short generate_N;
    char *input_file;
    char *output_file;
    char *type_of_sort;

};

int get_arguments(int argcount, char *argvector[], struct arguments *opts);