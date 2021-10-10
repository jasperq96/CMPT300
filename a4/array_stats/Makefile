CC=gcc
CCFLAGS= -g -std=c99 -D _GNU_SOURCE -static 

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)

%.o : %.c
	$(CC) -c $(CCFLAGS) $<

test: array_stats_test.o
	$(CC) $(CCFLAGS) array_stats_test.o -o test

transfer:
	scp -P 10022 test root@localhost: #10022 is the port number being used

clean:
	rm -f core *.o test