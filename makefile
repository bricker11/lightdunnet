# Makefile

# the C++ compiler
CXX     = g++

# options to pass to the compiler
CXXFLAGS = -O0 -g3 -std=c++14

# source file
SRCS = $(wildcard *.cpp)

# object file
OBJS = $(patsubst %c, %o, $(SRCS))

# target elf file
TARGET = main

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o:%.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f *~ *.o main *.exe *.stackdump
