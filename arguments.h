// This file is covered by the LICENSE file in the root of this project.

// Parses command line arguments
// and prepares them for execution in the main function.

#pragma once

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOPTLEN 20
#define DEFAULT_SLEEP 2.0
#define SEC_TO_USEC 1000000

typedef struct {
    float sec_sleep;
    bool beep_on;
    bool help_requested;
    char **command;
} arguments_t;

arguments_t *create_arguments_t(int argc, char **argv);
void free_arguments_t(arguments_t *args);
void print_arguments_t(arguments_t *args);
void print_command(arguments_t *args);
bool skip_next_arg(const char *arg, const struct option *opts);
bool arg_is_option(const char *arg, const struct option *opts);
void display_help();
int get_usec(arguments_t *args);
