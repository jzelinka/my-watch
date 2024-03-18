// This file is covered by the LICENSE file in the root of this project.

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "arguments.h"
#include "handle_terminal.h"

void my_exit(int exit_code, arguments_t *args_to_clean) {
    free_arguments_t(args_to_clean);
    exit(exit_code);
}

int main(int argc, char **argv) {
    arguments_t *args = create_arguments_t(argc, argv);

    if (args->help_requested) {
        my_exit(EXIT_SUCCESS, args);
    }
    if (args->command == NULL) {
        my_exit(EXIT_FAILURE, args);
    }

    change_input_handling();
    pid_t cpid;

    while (keep_running()) {
        // create pipes to transfer the input between child and parent
        int pipefd[2];
        int err_pipefd[2];

        if (pipe(pipefd) == -1) {
            perror("ERROR: Pipe failed.");
            my_exit(EXIT_FAILURE, args);
        }

        if (pipe(err_pipefd) == -1) {
            perror("ERROR: Error pipe failed.");
            my_exit(EXIT_FAILURE, args);
        }

        // create the child process executing main command
        cpid = fork();
        if (cpid == -1) {
            perror("ERROR: Fork failed.");
            my_exit(EXIT_FAILURE, args);
        }

        if (cpid == 0) {
            // execute the command in the child process
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(err_pipefd[1], STDERR_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            close(err_pipefd[0]);
            close(err_pipefd[1]);
            execvp(args->command[0], args->command);
            fprintf(stderr, "ERROR: Unable to execute the command.\n");
            my_exit(EXIT_FAILURE, args);
        } else {
            // handle the output of the child process
            // and invoke it again after sleep
            close(pipefd[1]);
            close(err_pipefd[1]);

            int wstatus;
            int w = wait(&wstatus);
            if (w == -1) {
                perror("ERROR: Wait fail.");
                my_exit(EXIT_FAILURE, args);
            }

            if (WIFEXITED(wstatus)) {
                clear_alt_screen();

                char buf;
                printf("Every %.1fs: ", args->sec_sleep);
                print_command(args);
                printf("\n");
                printf("\n");
                while (read(pipefd[0], &buf, 1) > 0)
                    write(STDOUT_FILENO, &buf, 1);
                if (args->beep_on && WEXITSTATUS(wstatus) != 0) {
                    while (read(err_pipefd[0], &buf, 1) > 0)
                        write(STDOUT_FILENO, &buf, 1);
                    beep();
                }

                printf("\nCommand exited with status=%d\n",
                       WEXITSTATUS(wstatus));
                usleep((unsigned int)get_usec(args));
            }
            close(pipefd[0]);
            close(err_pipefd[0]);
        }
    }

    if (cpid != 0) { // the parent process does its cleanup
        default_input_handling();
        free_arguments_t(args);
    }

    return EXIT_SUCCESS;
}
