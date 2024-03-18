# ITS8020-final-project

This repository contains an implementation of the `watch` command.
The binary can be built with the help of appended `Makefile`.
The command includes options to set the amount of idling seconds
and beep on unsuccessful execution of the watched command.

The code utilizes `getopt` to gather the command line options,
`fork` and `exec` to execute the watched command and
`pipe` to propagate the output between the parent and child process.
The appearance of the terminal is handled using escape sequences in `printf`
statements and subsequent `fflush` calls.

The behaviour of commands requiring longer time during the execution can be
tested by:

	./watch ls

then opening a new terminal in this directory and creating a file with:

    touch testfile

The file should appear in the output of the `watch` command.
