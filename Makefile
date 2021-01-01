.PHONY: clean run _run test _test prod _prod

CC		:= g++
OPT_DBG_C_FLAGS := -std=c++17 -Wall -Werror -Wextra -g -Ofast -march=native #-fsanitize=undefined -fsanitize=address
PROD_C_FLAGS := -std=c++17 -Wall -Werror -Wextra -Ofast -march=native -DNDEBUG
TEST_C_FLAGS := -std=c++17 -Wall -Werror -Wextra -g -O0 -march=native 

BIN			:= bin
SRC			:= src
INCLUDE		:= include
LIB			:= lib
TEST		:= test


TEST_FILES	:= $(filter-out src/main.cpp, $(wildcard src/*.cpp))

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
PROD_EXEC	:= main_prod.exe
RUN_EXEC	:= main.exe
TEST_EXEC	:= _test.exe
else
PROD_EXEC	:= main_prod
RUN_EXEC	:= main
TEST_EXEC	:= _test
endif

# all: $(BIN)/$(RUN_EXEC)
clean:
	$(RM) $(BIN)/$(PROD_EXEC)
	$(RM) $(BIN)/$(RUN_EXEC)
	$(RM) $(BIN)/$(TEST_EXEC)

_run: $(SRC)/*.cpp
	$(CC)  $(OPT_DBG_C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $(BIN)/$(RUN_EXEC) $(LIBRARIES)

run: _run
	./$(BIN)/$(RUN_EXEC)

_test: $(filter-out src/main.cpp, $(TEST)/*.cpp $(TEST_FILES))
	$(CC) $(TEST_C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $(BIN)/$(TEST_EXEC) $(LIBRARIES)

test: _test
	./$(BIN)/$(TEST_EXEC)
	$(RM) $(BIN)/$(TEST_EXEC)

_prod: $(SRC)/*.cpp
	$(CC) $(PROD_C_FLAGS) -I$(INCLUDE) -I$(LIB) $^ -o $(BIN)/$(PROD_EXEC) $(LIBRARIES)

prod: _prod
	./$(BIN)/$(PROD_EXEC)