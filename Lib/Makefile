CC = g++
CFLAGS = -fPIC -Wall -Wextra -std=c++20
LDFLAGS = -shared
TARGET = lib.so
SRC = lib.cpp
OBJ = lib.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)