CC = gcc
CFLAGS = -ansi -pedantic -Wall -g  
LDFLAGS = -lm

SRC_DIRS = dc ic extern first_pass functions labels preprocessor second_pass encode
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

my_program: $(OBJS) main.o
	$(CC) $(OBJS) main.o -o $@ $(LDFLAGS)

all: my_program

clean:
	rm -f my_program $(OBJS) main.o

.PHONY: all clean

