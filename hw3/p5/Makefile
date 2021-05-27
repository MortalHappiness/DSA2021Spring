.PHONY: all clean test
all: # The default target

# tool macros
CC := gcc
CFLAGS := -Wall -static -O2 -std=c11

# compile macros
TARGET := main
PROBLEM_ID := 48

GITROOT := $(shell git rev-parse --show-toplevel)
TEST_DIR := $(GITROOT)/tests/$(PROBLEM_ID)

# non-phony targets
main: main.o

# phony rules
all: $(TARGET)

clean:
	rm -rf core.* *.o $(TARGET)

test: main
	@mkdir -p $(TEST_DIR)/bin; cp main $(TEST_DIR)/bin/main; cd $(TEST_DIR); python test.py
