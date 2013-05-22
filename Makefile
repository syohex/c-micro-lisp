TARGET=simple-lisp
OBJS=environment.o main.o object.o eval.o read.o repl.o
CFLAGS=-Wall -Werror
CC=gcc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -g3 $(CFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) -g3 $(CFLAGS) -c $<

clean:
	rm $(OBJS) $(TARGET)
