.PHONY: all clean run test _test

CC		:= clang++
C_FLAGS := -std=c++17 -Wall -Werror -Wextra -g

BIN			:= bin
SRC			:= src
INCLUDE		:= include
LIB			:= lib
TEST		:= test
TEST_EXEC	:= _test

TEST_FILES	:= $(filter-out src/main.cpp, $(wildcard src/*.cpp))

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
else
EXECUTABLE	:= main
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