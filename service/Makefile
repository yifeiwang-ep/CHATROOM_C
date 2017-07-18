#!Makefile

PROGRAM = Service

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc

#C_FLAGS = -DDEBUG -c -Wall -Wextra -ggdb -I.
C_FLAGS = -c.

all: $(C_OBJECTS)
	@echo compile...
	$(CC) $(C_OBJECTS)  -o $(PROGRAM) -lpthread

.c.o:
	@echo link$< ...
	$(CC) $(C_FLAGS) $< -o $@

.PHONY:clean
clean:
	$(RM) $(C_OBJECTS) Service
