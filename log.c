#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "log.h"

void logg (log_lvl lvl, const char *format, ...) {
    va_list args;

    const char *log_filename = "log.txt";
    FILE *log_file = fopen(log_filename, "a");

    va_start(args,format);
    vfprintf (log_file, format, args);
    va_end(args);
    fclose(log_file);
if(lvl == DANGER) {
    exit(EXIT_FAILURE);
}
}