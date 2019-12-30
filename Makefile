.PHONY: all clean run test _test

CC		:= g++
C_FLAGS := -std=c++17 -Wall -Werror -Wextra -g -O2 -march=native

BIN			:= bin
SRC			:= src
INCLUDE		:= include
LIB			:= lib
TEST		:= test


TEST_FILES	:= $(filter-out src/main.cpp, $(wildcard src/*.cpp))

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
TEST_EXEC	:= _test.exe
else
EXECUTABLE	:= main
TEST_EXEC	:= _test
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

_test: $(filter-out src/main.cpp, $(TEST)/*.cpp $(TEST_FILES))
	$(CC) $(C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $@ $(LIBRARIES)

test: _test
	./$(TEST_EXEC)
	$(RM) $(TEST_EXEC)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $@ $(LIBRARIES)