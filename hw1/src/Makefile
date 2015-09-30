CC=gcc
CFLAGS=-Wall -O1 -std=c11

all: graphexec

graphexec: main.o readGraph.o proctype.o makeargv.o checkcycle.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o: main.c
	$(CC) -c -o $@ $< $(CFLAGS)

readGraphExe.o: readGraphExe.c readGraphExe.h
	$(CC) -c -o $@ $< $(CFLAGS)

proctype.o: proctype.c proctype.h
	$(CC) -c -o $@ $< $(CFLAGS)

makeargv.o: makeargv.c makeargv.h
	$(CC) -c -o $@ $< $(CFLAGS)

checkcycle.o: checkcycle.c checkcycle.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~ graphexec tsort*

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
