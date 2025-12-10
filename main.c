#include <stdio.h>
#include <string.h>
#include "io.h"
#include "arg.h"

int main(int argc, char **argv)
{
    struct arguments opts;

    if (get_arguments(argc, argv, &opts) != 0)
    {
        fprintf(stderr, "Ошибка: неверные аргументы\n");
        return 1;
    }

    if (opts.mode == NULL)
    {
        fprintf(stderr, "Ошибка: не указан режим (--generate, --sort, --print)\n");
        return 1;
    }

    if (strcmp(opts.mode, "generate") == 0)
    {
        generate_mode(&opts);
    }
    else if (strcmp(opts.mode, "sort") == 0)
    {
        sort_mode(&opts);
    }
    else if (strcmp(opts.mode, "print") == 0)
    {
        print_mode(&opts);
    }
    else
    {
        fprintf(stderr, "Неизвестный режим: %s\n", opts.mode);
        return 1;
    }

    return 0;
}
