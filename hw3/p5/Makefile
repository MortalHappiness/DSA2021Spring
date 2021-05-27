.PHONY: all clean unittest test
all: # The default target

# tool macros
CC := gcc
CXX := g++
CFLAGS := -Wall -static -O2 -std=c11
CXXFLAGS := -Wall

# compile macros
TARGET := main
PROBLEM_ID := 48

GITROOT := $(shell git rev-parse --show-toplevel)
TEST_DIR := $(GITROOT)/tests/$(PROBLEM_ID)

# non-phony targets
main: main.o

test_main.o: main.c

test_main: test_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lpthread

# phony rules
all: $(TARGET)

clean:
	rm -rf core.* *.o $(TARGET)

unittest: test_main
	./test_main

test: main
	@mkdir -p $(TEST_DIR)/bin; cp main $(TEST_DIR)/bin/main; cd $(TEST_DIR); python test.py
