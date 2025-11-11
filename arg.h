#pragma once

struct arguments
{
    char *mode;
    short unsigned generate_N;
    char *input_file;
    char *output_file;
    char *type_of_sort;

};

struct arguments get_arguments (int argcount, char *argvector[]);