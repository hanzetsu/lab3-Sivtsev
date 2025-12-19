#include "io.h"
#include "arg.h"
#include <stdio.h>
#include "log.h"

void generate_mode(struct arguments *opts)
{
    if (opts->generate_N == 0)
    {
        logg(1,"Ошибка: количество генерируемых записей должно быть > 0\n");
    }
    FILE *out = stdout;

    if (opts->output_file != NULL)
    {
        out = fopen(opts->output_file, "w");
        if (!out)
        {
            logg(1,"Не удалось открыть файл вывода: %s\n", opts->output_file);
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

    foreach (c,it)
    {
        struct house *h = (struct house *)iterator_get_value(it);
        fprintf(out, "%s,%s,%d,%hu,%d,%d,%hu,%hu,%.2f\n",
                h->name_of_the_developer,
                h->name_of_microdistrict,
                h->house_type,
                h->year_built,
                h->elevator,
                h->trash,
                h->number_of_apartments,
                h->number_of_floors,
                h->average_apartment_area);
    }
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
                           &h.trash,
                           &h.number_of_apartments,
                           &h.number_of_floors,
                           &h.average_apartment_area);

        if (parsed != 9)
        {
            logg(0, "неполная строка CSV: %s\n", line);
            continue;
        }

        if (house_type_int < 0 || house_type_int > 2)
        {
            logg(0, "некорректный тип дома: %d\n", house_type_int);
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
            container_destroy(cont);
            logg(1, "Не удалось открыть файл ввода: %s\n", opts->input_file);
        }
    }

    read_csv(cont, in);

    if (in != stdin)
        fclose(in);

    int descending = 0;
    if (opts->type_of_sort != NULL &&
        strcmp(opts->type_of_sort, "desc") == 0)
        descending = 1;

    container_sort_quick(cont, comparator, descending);

    FILE *out = stdout;
    if (opts->output_file != NULL)
    {
        out = fopen(opts->output_file, "w");
        if (!out)
        {
            container_destroy(cont);
            logg(1, "Не удалось открыть файл вывода: %s\n", opts->output_file);
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
            container_destroy(cont);
            logg(1, "Не удалось открыть файл ввода: %s\n", opts->input_file);
        }
    }
    else
    {
        printf("Введите имя файла для чтения: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)

        {
            container_destroy(cont);
            logg(1, "Ошибка: не удалось прочитать имя файла из stdin\n");
        }

        if (strlen(filename) == 0)
        {
            container_destroy(cont);
            logg(1, "Ошибка: не указано имя файла\n");
        }

        in = fopen(filename, "r");
        if (!in)
        {
            container_destroy(cont);
            logg(1, "Ошибка: не удалось открыть файл: %s\n", filename);
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
            container_destroy(cont);
            logg(1, "Ошибка: не удалось открыть файл вывода: %s\n", opts->output_file);
        }
    }

    fprintf(out, "%-40s | %-30s | %-15s | %-4s | %-5s | %-5s | %-10s | %-8s | %-10s\n",
            "Застройщик",
            "Район  ",
            "Тип",
            "Год",
            "Лифт",
            "Мусоропровод",
            "Квартиры",
            "Этажи",
            "Площадь");

    fprintf(out, "------------------------------------------------------------------------------------------------------------\n");

    Iterator *it = iterator_begin(cont);

    while (iterator_is_valid(it))
    {
        struct house *h = (struct house *)iterator_get_value(it);

        const char *type_str = "Неизвестно";
        if (h->house_type == panel)
            type_str = "Панельный";
        else if (h->house_type == brick)
            type_str = "Кирпичный";
        else if (h->house_type == monolithic)
            type_str = "Монолитный";

        const char *elevator_str;
        if (h->elevator == 1)
            elevator_str = "Да";
        else
            elevator_str = "Нет";

        const char *trash_str;
        if (h->trash == 1)
            trash_str = "Да";
        else
            trash_str = "Нет";

        fprintf(out, "%-40.40s | %-30.30s | %-15.15s | %-4hu | %-5s | %-5s | %-10hu | %-8hu | %10.2f\n",
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