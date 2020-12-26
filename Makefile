.PHONY: all clean run run_test _test

CC		:= g++
OPT_DBG_C_FLAGS := -std=c++17 -Wall -Werror -Wextra -g -O2 -march=native -fsanitize=undefined -fsanitize=address
TEST_C_FLAGS := -std=c++17 -Wall -Werror -Wextra -g -O0 -march=native

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
	$(RM) $(TEST_EXEC)

run: all
	./$(BIN)/$(EXECUTABLE)

_test: $(filter-out src/main.cpp, $(TEST)/*.cpp $(TEST_FILES))
	$(CC) $(TEST_C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $@ $(LIBRARIES)

run_test: _test
	./$(TEST_EXEC)
	$(RM) $(TEST_EXEC)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(OPT_DBG_C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $@ $(LIBRARIES)