#include "arg.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void get_arguments_mode(struct arguments *opts, int argcount, char *argvector[])
{

    for (int i = 1; i < argcount; i++)
    {
        if ((strcmp(argvector[i], "--sort") == 0) || (strcmp(argvector[i], "-s") == 0))
        {
            if (opts->mode != NULL)
            {
                puts("Ошибка: можно указать только один режим (--sort, --generate, --print)");
                return;
            }
            opts->mode = "sort";
        }
        if ((strcmp(argvector[i], "--generate") == 0) || (strcmp(argvector[i], "-g") == 0))
        {
            if (opts->mode != NULL)
            {
                puts("Ошибка: можно указать только один режим (--sort, --generate, --print)");
                return;
            }
            if (i + 1 < argcount)
            {
                if (argvector[i + 1][0] == '-')
                {
                    puts("Ошибка: параметр не может быть отрицательным");
                    return;
                }
                else
                {
                    char *end;
                    unsigned long temp = strtoul(argvector[i + 1], &end, 10);
                    if (*end != '\0')
                    {
                        puts("Ошибка: строка содержит недопустимые символы");
                        return;
                    }
                    else if (temp > USHRT_MAX)
                    {
                        puts("Ошибка: число слишком большое");
                        return;
                    }
                    else
                        opts->generate_N = (unsigned short)temp;
                }
            }
            else
            {
                puts("Вы не ввели параметр после флага");
                return;
            }

            opts->mode = "generate";
        }
        if ((strcmp(argvector[i], "--print") == 0) || (strcmp(argvector[i], "-P") == 0))
        {
            {
                if (opts->mode != NULL)
                {
                    puts("Ошибка: можно указать только один режим (--sort, --generate, --print)");
                    return;
                }

                opts->mode = "print";
            }
        }
    }
}

void get_arguments_input_output_files(struct arguments *opts, int argcount, char *argvector[])
{
    for (int i = 1; i < argcount; i++)
    {
        if (strncmp(argvector[i], "--out=", 6) == 0)
        {
            opts->output_file = argvector[i] + 6;
        }
        if (strcmp(argvector[i], "-o") == 0)
        {
            if (i + 1 < argcount)
                opts->output_file = argvector[i + 1];
            else
            {
                puts("Вы не ввели параметр после флага");
                return;
            }
        }
        if (strncmp(argvector[i], "--in=", 5) == 0)
        {
            opts->input_file = argvector[i] + 5;
        }
        if (strcmp(argvector[i], "-i") == 0)
        {
            if (i + 1 < argcount)
                opts->input_file = argvector[i + 1];
            else
            {
                puts("Вы не ввели параметр после флага");
                return;
            }
        }
    }
}
void get_arguments_type_of_sort(struct arguments *opts, int argcount, char *argvector[])
{
    for (int i = 1; i < argcount; i++)
    {
        if (strncmp(argvector[i], "--type=", 7) == 0)
        {
            if (argvector[i] + 7 == "asc" || argvector[i] + 7 == "desc")
                opts->type_of_sort = argvector[i] + 7;
            else
            {
                puts("Ошибка: неправильный тип сортировки");
                return;
            }
        }
        if (strcmp(argvector[i], "-t") == 0)
        {
            if (i + 1 < argcount)
            {
                if (argvector[i + 1] == "asc" || argvector[i] + 7 == "desc")
                    opts->type_of_sort = argvector[i + 1];
                else
                {
                    puts("Ошибка: неправильный тип сортировки");
                    return 0;
                }
            }
            else
            {
                puts("Вы не ввели параметр после флага ");
                return;
            }
        }
    }
}

struct arguments get_arguments(int argcount, char *argvector[])
{
    struct arguments opts = {0};
    opts.type_of_sort = "asc";
    get_arguments_mode(&opts, argcount, argvector);
    get_arguments_input_output_files(&opts, argcount, argvector);
    get_arguments_type_of_sort(&opts, argcount, argvector);

    return opts;
}