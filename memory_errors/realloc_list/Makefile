CC	 = gcc
CXX	 = g++
CFLAGS	 = -Wall -Wextra -fPIC
CXXFLAGS = -Wall -Wextra
LDFLAGS  = -shared
GTESTFLAGS = -I/usr/local/include  -L/usr/local/lib64 -lgtest -lgtest_main -lpthread

SRC_DIR  = src
TEST_DIR = test

LIB_SRC  = $(SRC_DIR)/realloc_list.c $(SRC_DIR)/realloc_list_iterator.c
LIB_HDR  = $(SRC_DIR)/realloc_list.h $(SRC_DIR)/realloc_list_iterator.h
LIB_OBJ  = $(SRC_DIR)/realloc_list.o $(SRC_DIR)/realloc_list_iterator.o
LIB	 = librealloc_list.so

TEST_SRC = $(wildcard $(TEST_DIR)/*.cc)
TEST_BIN = test_runner

.PHONY: all lib test clean

all: lib

lib: $(LIB)

$(LIB): $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(LIB_HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(LIB) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TEST_SRC) -I$(SRC_DIR) -I$(TEST_DIR) -L. -lrealloc_list $(GTESTFLAGS)
	LD_LIBRARY_PATH=. ./$(TEST_BIN)

clean:
	rm -f $(LIB_OBJ) $(LIB) $(TEST_BIN)
