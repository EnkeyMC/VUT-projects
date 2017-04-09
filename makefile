PROJECT=IOS_project_2
EXEC=proj2
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
OBJ_DIR=obj
ARGS=1 4 1 10 5 5

SRCS=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJS=$(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm $(EXEC)

pack: $(SRCS) $(HEADERS) Makefile
	zip $(EXEC).zip $^

run: $(EXEC)
	./$(EXEC)

runargs: $(EXEC)
	./$(EXEC) $(ARGS)
