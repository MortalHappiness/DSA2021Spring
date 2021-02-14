.PHONY: all clean test
all: # The default target

# tool macros
CC := gcc
CFLAGS := -Wall -g

# compile macros
TARGET := nonogram

# non-phony targets
nonogram: nonogram.o

# phony rules
all: $(TARGET)

clean:
	rm -rf core.* *.o $(TARGET)

test: nonogram
	python test_nonogram.py
