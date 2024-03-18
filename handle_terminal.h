// This file is covered by the LICENSE file in the root of this project.

// Uses various escape sequences to accomplish the desired terminal behaviour.
#pragma once

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define SWITCH_ALT_SCREEN "\e[?1049h\e[1;1H"
#define SWITCH_FROM_SCREEN "\e[?1049l"
#define DISABLE_CURSOR "\e[?25l"
#define ENABLE_CURSOR "\e[?25h"
#define CLEAR_SCREEN "\e[1J\e[1;1H"

void change_input_handling();
void default_input_handling();
void input_echo_on();
void input_echo_off();
void clear_alt_screen();
void default_screen();
void my_handling_of_CTRLC();
bool keep_running();
void beep();
