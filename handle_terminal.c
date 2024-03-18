// This file is covered by the LICENSE file in the root of this project.

#include "handle_terminal.h"

bool volatile keep_running_switch = true;
struct termios orig_termios;
bool alt_screen_set = false;

void change_input_handling() {
    printf(DISABLE_CURSOR);
    fflush(stdout);
    input_echo_off();
    my_handling_of_CTRLC();
}

void default_input_handling() {
    default_screen();
    printf("\e[?25h"); // enables cursor
    input_echo_on();
}

void input_echo_on() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void input_echo_off() {
    // Current terminal settings are stored to retrieve them later.
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void alt_screen() {
    printf(SWITCH_ALT_SCREEN);
    fflush(stdout);
}

void default_screen() {
    printf(SWITCH_FROM_SCREEN);
    fflush(stdout);
    alt_screen_set = false;
}

void sigintHandler(int sig_num) {
    keep_running_switch = false;
    fflush(stdout);
}

void clear_alt_screen() {
    if (!alt_screen_set) {
        alt_screen();
        alt_screen_set = true;
    } else {
        printf(CLEAR_SCREEN);
        fflush(stdout);
    }
}

void my_handling_of_CTRLC() { signal(SIGINT, sigintHandler); }

bool keep_running() { return keep_running_switch; }

void beep() {
    printf("\a");
    fflush(stdout);
}
