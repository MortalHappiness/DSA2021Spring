.PHONY: all clean test
all: # The default target

# tool macros
CC := gcc
CFLAGS := -Wall -static -O2 -std=c11

# compile macros
TARGET := main

# non-phony targets
main: main.o

# phony rules
all: $(TARGET)

clean:
	rm -rf core.* *.o $(TARGET)

test: main
	python test_main.py
