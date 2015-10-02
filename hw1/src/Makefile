CC=gcc
CFLAGS=-g -Wall -fstack-protector -Wextra -Wunreachable-code -O1 -std=c99

all: graphexec

graphexec: main.o read_graph.o proctype.o makeargv.o checkcycle.o proc_utility.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o: main.c
	$(CC) -c -o $@ $< $(CFLAGS)

read_graph.o: read_graph.c read_graph.h
	$(CC) -c -o $@ $< $(CFLAGS)

proctype.o: proctype.c proctype.h
	$(CC) -c -o $@ $< $(CFLAGS)

makeargv.o: makeargv.c makeargv.h
	$(CC) -c -o $@ $< $(CFLAGS)

checkcycle.o: checkcycle.c checkcycle.h
	$(CC) -c -o $@ $< $(CFLAGS)

proc_utility.o: proc_utility.c proc_utility.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~ graphexec tsort* *.out *.txt
	rm -rf ./output

test:
	./graphexec ../res/pa1-release/testgraph0.txt
	./graphexec ../res/pa1-release/testgraph1.txt
	./graphexec ../res/pa1-release/testgraph2.txt
	./graphexec ../res/pa1-release/testgraph3.txt
	./graphexec ../res/pa1-release/testgraph4.txt
	./graphexec ../res/pa1-release/testgraph5.txt
	./graphexec ../res/pa1-release/testCycle0.txt
	./graphexec ../res/pa1-release/testCycle1.txt
	./graphexec ../res/pa1-release/testfail0.txt
