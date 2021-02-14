.PHONY: all clean unittest test
all: # The default target

# tool macros
CC := gcc
CXX := g++
CFLAGS := -Wall
CXXFLAGS := -Wall

# compile macros
TARGET := gcd test_bigint

# source files and dependency files
SRC := $(wildcard *.c*)
DEPS := $(addsuffix .d, $(basename $(SRC)))

# include auto-generated dependency files
include $(DEPS)

# non-phony targets
gcd: gcd.o bigint.o

test_bigint: test_bigint.o bigint.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lpthread

%.d: %.c*
	$(CC) -MM $(CPPFLAGS) $< > $@;

# phony rules
all: $(TARGET)

clean:
	rm -rf core.* *.o *.d $(TARGET)

unittest: test_bigint
	./test_bigint

test: gcd
	python test_gcd.py
