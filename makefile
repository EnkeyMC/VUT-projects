PROJECT=IOS_project_2
EXEC=proj2
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g -DDEBUG
LFLAGS=-pthread
OBJ_DIR=obj
ARGS=7 2 5 5 1 1
DEADLOCKARG=10000

SRCS=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJS=$(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm $(EXEC)

pack: $(SRCS) $(HEADERS) makefile
	zip $(EXEC).zip $^

runnoargs: $(EXEC)
	./$(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

debug: $(EXEC)
	gdb ./$(EXEC)

valgrind: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)

valgrind-full: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)

test-deadlock: $(EXEC)
	./test-deadlock.sh $(ARGS) $(DEADLOCKARG)
