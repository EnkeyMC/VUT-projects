PROJECT=IOS_project_2
EXEC=proj2
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g -DDEBUG
LFLAGS=-pthread
OBJ_DIR=obj
ARGS=1 4 20 10 5 5
DEADLOCKARG=20000

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
	./test-deadlock.sh $(DEADLOCKARG)
