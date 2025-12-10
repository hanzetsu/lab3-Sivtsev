#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "sort.h"
#include "arg.h"
#include "container.h"

void generate_mode(struct arguments *opts);
void sort_mode(struct arguments *opts);
void print_mode(struct arguments *opts);
void read_csv(container *c, FILE *in);
void write_csv(const container *c, FILE *out);