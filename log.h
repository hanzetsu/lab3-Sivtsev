#pragma once

typedef enum {
    ERROR,
    DANGER
}log_lvl;

void logg (log_lvl log, const char *format, ...);