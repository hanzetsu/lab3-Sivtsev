#include "arg.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int get_arguments_mode(struct arguments *opts, int argcount, char *argvector[])
{

    for (int i = 1; i < argcount; i++)
    {
        if ((strcmp(argvector[i], "--sort") == 0) || (strcmp(argvector[i], "-s") == 0))
        {
            if (opts->mode != NULL)
            {
                fprintf(stderr, "Ошибка: можно указать только один режим (--sort, --generate, --print)\n");
                return 1;
            }
            opts->mode = "sort";
        }
        else if ((strcmp(argvector[i], "--generate") == 0) || (strcmp(argvector[i], "-g") == 0))
        {
            if (opts->mode != NULL)
            {
                fprintf(stderr, "Ошибка: можно указать только один режим (--sort, --generate, --print)\n");
                return 1;
            }
            if (i + 1 < argcount)
            {

                char *end;
                unsigned long temp = strtoul(argvector[i + 1], &end, 10);
                if (*end != '\0')
                {
                    fprintf(stderr, "Ошибка: строка содержит недопустимые символы\n");
                    return 1;
                }
                else if (temp > USHRT_MAX || temp == 0)
                {
                    fprintf(stderr, "Ошибка: число должно быть в диапазоне 1..%d\n", USHRT_MAX);
                    return 1;
                }
                else
                    opts->generate_N = (unsigned short)temp;
            }
            else
            {
                fprintf(stderr, "Вы не ввели параметр после флага\n");
                return 1;
            }
            i++;
            opts->mode = "generate";
        }
        else if ((strcmp(argvector[i], "--print") == 0) || (strcmp(argvector[i], "-P") == 0))
        {
            {
                if (opts->mode != NULL)
                {
                    fprintf(stderr, "Ошибка: можно указать только один режим (--sort, --generate, --print)\n");
                    return 1;
                }

                opts->mode = "print";
            }
        }
    }
    return 0;
}

int get_arguments_input_output_files(struct arguments *opts, int argcount, char *argvector[])
{
    for (int i = 1; i < argcount; i++)
    {
        if (strncmp(argvector[i], "--out=", 6) == 0)
        {
            opts->output_file = argvector[i] + 6;
        }
        else if (strcmp(argvector[i], "-o") == 0)
        {
            if (i + 1 < argcount)
            {
                opts->output_file = argvector[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Ошибка: для флага -o требуется имя файла\n");
                return 1;
            }
        }
        else if (strncmp(argvector[i], "--in=", 5) == 0)
        {
            opts->input_file = argvector[i] + 5;
        }
        else if (strcmp(argvector[i], "-i") == 0)
        {
            if (i + 1 < argcount)
            {
                opts->input_file = argvector[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Ошибка: для флага -i требуется имя файла\n");
                return 1;
            }
        }
    }
    return 0;
}
int get_arguments_type_of_sort(struct arguments *opts, int argcount, char *argvector[])
{
    for (int i = 1; i < argcount; i++)
    {
        if (strncmp(argvector[i], "--type=", 7) == 0)
        {
            if (strcmp(argvector[i] + 7, "asc") == 0 || strcmp(argvector[i] + 7, "desc") == 0)
                opts->type_of_sort = argvector[i] + 7;
            else
            {
                fprintf(stderr, "Ошибка: неправильный тип сортировки");
                return 1;
            }
        }
        else if (strcmp(argvector[i], "-t") == 0)
        {
            if (i + 1 < argcount)
            {
                if (strcmp(argvector[i + 1], "A") == 0)
                    opts->type_of_sort = "asc";
                else if (strcmp(argvector[i + 1], "D") == 0)
                    opts->type_of_sort = "desc";
                else
                {
                    fprintf(stderr, "Ошибка: неправильный тип сортировки. Используйте A для возрастания или D для убывания.\n");
                    return 1;
                }
                i++;
            }
            else
            {
                fprintf(stderr, "Вы не ввели параметр после флага\n");
                return 1;
            }
        }
    }
}
int get_arguments(int argcount, char *argvector[], struct arguments *opts)
{
    *opts = (struct arguments){0};
    opts->type_of_sort = "asc";
    if (get_arguments_mode(opts, argcount, argvector))
        return 1;

    if (get_arguments_input_output_files(opts, argcount, argvector))
        return 1;

    if (get_arguments_type_of_sort(opts, argcount, argvector))
        return 1;

    return 0;
}
