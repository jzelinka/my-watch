// This file is covered by the LICENSE file in the root of this project.

#include "arguments.h"

arguments_t *create_arguments_t(int argc, char **argv) {
    // returns the processed arguments in a datastructure
    // returns NULL if there are some mistakes or missing command
    arguments_t *args = malloc(sizeof(arguments_t));

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"beep", no_argument, 0, 'b'},
        {"interval", required_argument, 0, 'n'},
        {0, 0, 0, 0},
    };

    args->beep_on = false;
    args->sec_sleep = 2.0;
    args->help_requested = false;
    args->command = NULL;

    int opt, option_index;

    // find the start of the command
    int command_start = 1;
    while (argv[command_start] != NULL) {
        if (!arg_is_option(argv[command_start], long_options)) {
            break;
        }

        if ((argv[command_start + 1] != NULL) &&
            skip_next_arg(argv[command_start], long_options)) {
            command_start++;
        }

        command_start++;
    }

    while ((opt = getopt_long(command_start, argv, "hbn:", long_options,
                              &option_index)) != -1) {
        switch (opt) {
        case 'b':
            args->beep_on = true;
            break;
        case 'n':
            args->sec_sleep = atof(optarg);
            if (args->sec_sleep < 0.001) {
                args->sec_sleep = 0.1;
            }
            break;
        case 'h':
            args->help_requested = true;
            display_help();
            args->command = NULL;
            return args;
        default: // unknown option, display help and exit
            display_help();
            args->command = NULL;
            return args;
        }
    }

    if (command_start == argc) {
        fprintf(stderr, "ERROR: Command missing.\n");
        display_help();
        args->command = NULL;
    } else {
        args->command = &argv[command_start];
    }

    return args;
}

void free_arguments_t(arguments_t *args) { free(args); }

void print_arguments_t(arguments_t *args) {
    printf("Beep: ");
    if (args->beep_on)
        printf("true\n");
    else
        printf("false\n");
    printf("Sleep %fs\n", args->sec_sleep);
    printf("Command: ");
    if (args->command == NULL) {
        printf("missing\n");
        return;
    }
    for (int i = 0; args->command[i] != NULL; i++) {
        printf("%s ", args->command[i]);
    }
    printf("\n");
}

void print_command(arguments_t *args) {
    if (args->command == NULL) {
        return;
    }
    for (int i = 0; args->command[i] != NULL; i++) {
        printf("%s ", args->command[i]);
    }
}

bool skip_next_arg(const char *arg, const struct option *opts) {
    // determines if argument has a required argument field
    for (int i = 0; opts[i].name != 0; i++) {
        // long argument
        int arg_len = strnlen(arg, MAXOPTLEN);
        if (arg_len > 2 && strncmp(&arg[2], opts[i].name, MAXOPTLEN) == 0) {
            return opts[i].has_arg == required_argument;
        }

        // just the last char of the -abcd is considered when dealing with opt
        if (arg_len > 1 && arg[arg_len - 1] == opts[i].val) {
            return opts[i].has_arg == required_argument;
        }
    }
    return false;
}

bool arg_is_option(const char *arg, const struct option *opts) {
    // Argument is an option if it is preceeded with an '-'
    if (strnlen(arg, MAXOPTLEN) > 1 && arg[0] == '-') {
        return true;
    } else {
        return false;
    }
}

void display_help() {
    printf("Usage:\n");
    printf(" watch [options] command\n");
    printf("\n");
    printf("Options:\n");
    printf("  -b, --beep                beep if command has a non-zero exit\n");
    printf("  -n, --interval <secs>     seconds to wait between updates\n");
    printf("\n");
    printf("  -h, --help                display this help and exit\n");
}

int get_usec(arguments_t *args) {
    // s = 1000 000 usec
    return args->sec_sleep * SEC_TO_USEC;
}
