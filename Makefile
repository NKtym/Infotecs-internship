CC = g++
CFLAGS = -std=c++20 -Wall -Wextra -pthread
LIB_DIR = ./Lib
PROG1_DIR = ./Prog1
PROG2_DIR = ./Prog2
LIB_SRC = $(LIB_DIR)/lib.cpp
LIB_OBJ = $(LIB_SRC:.cpp=.o)
PROG1_SRC = $(PROG1_DIR)/prog1.cpp
PROG2_SRC = $(PROG2_DIR)/prog2.cpp
PROG1_OBJ = $(PROG1_SRC:.cpp=.o)
PROG2_OBJ = $(PROG2_SRC:.cpp=.o)
SOCKET_LIB = -ldl

LIB_OUTPUT = $(LIB_DIR)/lib.a
PROG1_OUTPUT = $(PROG1_DIR)/prog1
PROG2_OUTPUT = $(PROG2_DIR)/prog2

all: $(PROG1_OUTPUT) $(PROG2_OUTPUT)

$(PROG1_OUTPUT): $(PROG1_OBJ) $(LIB_OUTPUT)
	$(CC) $(CFLAGS) -o $@ $^ $(SOCKET_LIB)

$(PROG2_OUTPUT): $(PROG2_OBJ) $(LIB_OUTPUT)
	$(CC) $(CFLAGS) -o $@ $^ $(SOCKET_LIB)

$(LIB_OUTPUT): $(LIB_OBJ)
	ar rcs $@ $^

$(PROG1_OBJ): $(PROG1_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG2_OBJ): $(PROG2_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OBJ): $(LIB_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROG1_OBJ) $(PROG2_OBJ) $(LIB_OBJ) $(PROG1_OUTPUT) $(PROG2_OUTPUT) $(LIB_OUTPUT)

.PHONY: all clean
