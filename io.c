#include "io.h"
#include "arg.h"
#include <stdio.h>

void generate_mode(struct arguments *opts)
{
    if (opts->generate_N == 0)
    {
        fprintf(stderr, "Ошибка: количество генерируемых записей должно быть > 0\n");
        exit(1);
    }
    FILE *out = stdout;

    if (opts->output_file != NULL)
    {
        out = fopen(opts->output_file, "w");
        if (!out)
        {
            fprintf(stderr, "Не удалось открыть файл вывода: %s\n", opts->output_file);
            exit(1);
        }
    }

    srand((unsigned int)time(NULL));

    container *cont = container_create(sizeof(struct house));

    for (unsigned short i = 0; i < opts->generate_N; ++i)
    {
        struct house h = generate_random_house();
        container_push_back(&h, cont);
    }

    write_csv(cont, out);

    if (out != stdout)
    {
        fclose(out);
    }
    container_destroy(cont);
}
void write_csv(const container *c, FILE *out)
{
    Iterator *it = iterator_begin((container *)c);
    while (iterator_is_valid(it))
    {
        struct house *h = (struct house *)iterator_get_value(it);
        fprintf(out, "%s,%s,%d,%hu,%d,%d,%hu,%hu,%.2f\n",
                h->name_of_the_developer,
                h->name_of_microdistrict,
                h->house_type,
                h->year_built,
                h->elevator,
                h->trash_chute,
                h->number_of_apartments,
                h->number_of_floors,
                h->average_apartment_area);
        iterator_next(it);
    }
    iterator_destroy(it);
    it = NULL;
}

void read_csv(container *c, FILE *in)
{
    char line[512];

    while (fgets(line, sizeof(line), in))
    {
        if (line[0] == '\n' || line[0] == '\0')
            continue;

        struct house h;
        char dev[MAX_STRING_LEN], dist[MAX_STRING_LEN];
        int house_type_int;

        int parsed = sscanf(line,
                           "%99[^,],%99[^,],%d,%hu,%d,%d,%hu,%hu,%f",
                           dev,
                           dist,
                           &house_type_int,
                           &h.year_built,
                           &h.elevator,
                           &h.trash_chute,
                           &h.number_of_apartments,
                           &h.number_of_floors,
                           &h.average_apartment_area);

        if (parsed != 9)
        {
            fprintf(stderr, "неполная строка CSV: %s\n", line);
            continue;
        }

        if (house_type_int < 0 || house_type_int > 2)
        {
            fprintf(stderr, "некорректный тип дома: %d\n", house_type_int);
            continue;
        }

        h.house_type = (enum type_of_house)house_type_int;
        
        strncpy(h.name_of_the_developer, dev, MAX_STRING_LEN - 1);
        h.name_of_the_developer[MAX_STRING_LEN - 1] = '\0';
        
        strncpy(h.name_of_microdistrict, dist, MAX_STRING_LEN - 1);
        h.name_of_microdistrict[MAX_STRING_LEN - 1] = '\0';

        container_push_back(&h, c);
    }
}
void sort_mode(struct arguments *opts)
{
    container *cont = container_create(sizeof(struct house));

    FILE *in = stdin;
    if (opts->input_file != NULL)
    {
        in = fopen(opts->input_file, "r");
        if (!in)
        {
            fprintf(stderr, "Не удалось открыть файл ввода: %s\n", opts->input_file);
            container_destroy(cont);
            exit(1);
        }
    }

    read_csv(cont, in);

    if (in != stdin)
        fclose(in);

    int descending = 1;
    if (opts->type_of_sort != NULL &&
        strcmp(opts->type_of_sort, "desc") == 0)
        descending = 0;

    container_sort(cont, comparator, descending);

    FILE *out = stdout;
    if (opts->output_file != NULL)
    {
        out = fopen(opts->output_file, "w");
        if (!out)
        {
            fprintf(stderr, "Не удалось открыть файл вывода: %s\n", opts->output_file);
            container_destroy(cont);
            exit(1);
        }
    }

    write_csv(cont, out);

    if (out != stdout)
        fclose(out);

    container_destroy(cont);
}

void print_mode(struct arguments *opts)
{
    container *cont = container_create(sizeof(struct house));

    FILE *in = stdin;
    char filename[256] = {0};

    if (opts->input_file != NULL)
    {
        in = fopen(opts->input_file, "r");
        if (!in)
        {
            fprintf(stderr, "Не удалось открыть файл ввода: %s\n", opts->input_file);
            container_destroy(cont);
            exit(1);
        }
    }
    else
    {
        printf("Введите имя файла для чтения: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            fprintf(stderr, "Ошибка: не удалось прочитать имя файла из stdin\n");
            container_destroy(cont);
            exit(1);
        }

        filename[strcspn(filename, "\n")] = '\0';

        if (strlen(filename) == 0)
        {
            fprintf(stderr, "Ошибка: не указано имя файла\n");
            container_destroy(cont);
            exit(1);
        }

        in = fopen(filename, "r");
        if (!in)
        {
            fprintf(stderr, "Ошибка: не удалось открыть файл: %s\n", filename);
            container_destroy(cont);
            exit(1);
        }
    }

    read_csv(cont, in);

    if (in != stdin)
    {
        fclose(in);
    }

    FILE *out = stdout;
    if (opts->output_file != NULL)
    {
        out = fopen(opts->output_file, "w");
        if (!out)
        {
            fprintf(stderr, "Ошибка: не удалось открыть файл вывода: %s\n", opts->output_file);
            container_destroy(cont);
            exit(1);
        }
    }

    fprintf(out, "%-20s | %-15s | %-11s | %-4s | %-3s | %-3s | %-8s | %-6s | %-8s\n",
            "Developer",
            "District",
            "Type",
            "Year",
            "Elv",
            "Trc",
            "Apts",
            "Floors",
            "Area");

    fprintf(out, "-------------------------------------------------------------------------------\n");

    Iterator *it = iterator_begin(cont);

    while (iterator_is_valid(it))
    {
        struct house *h = (struct house *)iterator_get_value(it);

        const char *type_str = "unknown";
        if (h->house_type == panel)
            type_str = "panel";
        else if (h->house_type == brick)
            type_str = "brick";
        else if (h->house_type == monolithic)
            type_str = "monolithic";

        const char *elevator_str;
        if (h->elevator == 1)
            elevator_str = "YES";
        else
            elevator_str = "NO";

        const char *trash_str;
        if (h->trash_chute == 1)
            trash_str = "YES";
        else
            trash_str = "NO";

        fprintf(out, "%-20s | %-15s | %-11s | %-4hu | %-3s | %-3s | %-8hu | %-6hu | %8.2f\n",
                h->name_of_the_developer,
                h->name_of_microdistrict,
                type_str,
                h->year_built,
                elevator_str,
                trash_str,
                h->number_of_apartments,
                h->number_of_floors,
                h->average_apartment_area);

        iterator_next(it);
    }

    iterator_destroy(it);

    if (out != stdout)
    {
        fclose(out);
        printf("Результат сохранен в файл: %s\n", opts->output_file);
    }

    container_destroy(cont);
}