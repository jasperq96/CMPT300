# Define what compiler to use and the flags.
CC=cc
CXX=gcc
CCFLAGS= -Wall -g -std=c99 -Werror -pthread -lpthread -lrt

all: candykids

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.c
	$(CC) -c $(CCFLAGS) $<

# Build test_mystring if necessary
candykids: bbuff.o stats.o candykids.o
	$(CXX) $(CCFLAGS) candykids.o bbuff.o stats.o -o candykids

clean:
	rm -f core *.o candykids