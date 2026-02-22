CC	 = gcc
CXX	 = g++
CFLAGS	 = -Wall -Wextra -fPIC
CXXFLAGS = -Wall -Wextra
LDFLAGS  = -shared
GTESTFLAGS = -I/usr/local/include  -L/usr/local/lib64 -lgtest -lgtest_main -lpthread

SRC_DIR  = src
TEST_DIR = test

LIB_SRC  = $(SRC_DIR)/realloc_list.c
LIB_HDR  = $(SRC_DIR)/realloc_list.h
LIB_OBJ  = $(SRC_DIR)/realloc_list.o
LIB	 = librealloc_list.so

TEST_SRC = $(wildcard $(TEST_DIR)/*.cc)
TEST_BIN = test_runner

.PHONY: all lib test clean

all: lib

lib: $(LIB)

$(LIB): $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(LIB_OBJ): $(LIB_SRC) $(LIB_HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(LIB) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TEST_SRC) -I$(SRC_DIR) -I$(TEST_DIR) -L. -lrealloc_list $(GTESTFLAGS)
	LD_LIBRARY_PATH=. ./$(TEST_BIN)

clean:
	rm -f $(LIB_OBJ) $(LIB) $(TEST_BIN)
