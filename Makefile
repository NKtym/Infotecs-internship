LIB_DIR := Lib
PROG1_DIR := Prog1
PROG2_DIR := Prog2
LIB_NAME := lib.so

CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -fPIC -shared
LDFLAGS := -ldl

all: lib prog1 prog2

lib:
	$(CC) $(CFLAGS) -o $(LIB_DIR)/$(LIB_NAME) $(LIB_DIR)/*.cpp

prog1: lib
	$(CC) -o $(PROG1_DIR)/prog1 $(PROG1_DIR)/*.cpp -ldl -rdynamic

prog2: lib
	$(CC) -o $(PROG2_DIR)/prog2 $(PROG2_DIR)/*.cpp -ldl -rdynamic

clean:
	rm -f $(LIB_DIR)/$(LIB_NAME) $(PROG1_DIR)/prog1 $(PROG2_DIR)/prog2