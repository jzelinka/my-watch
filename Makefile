.PHONY= all clean
CFLAGS+= -Wall
CC=gcc
COMMANDN=watch

all: $(COMMANDN)

$(COMMANDN): watch.c handle_terminal.o arguments.o
	$(CC) $(CFLAGS) watch.c handle_terminal.o arguments.o -o $(COMMANDN)

handle_terminal.o: handle_terminal.c handle_terminal.h
	$(CC) $(CFLAGS) -c handle_terminal.c -o handle_terminal.o

arguments.o: arguments.c arguments.h
	$(CC) $(CFLAGS) -c arguments.c -o arguments.o

format: 
	clang-format -i *.c
	clang-format -i *.h

clean:
	rm -f watch
	rm -f *.o
