CC=gcc
IDIR=./header
CFLAGS=-Wall -O1 -std=c11 -I$(IDIR)

all: graphexec

graphexec: main.o readGraph.o proctype.o makeargv.o checkcycle.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o: main.c
	$(CC) -c -o $@ $< $(CFLAGS)

readGraphExe.o: readGraphExe.c $(IDIR)/readGraphExe.h
	$(CC) -c -o $@ $< $(CFLAGS)

proctype.o: proctype.c $(IDIR)/proctype.h
	$(CC) -c -o $@ $< $(CFLAGS)

makeargv.o: makeargv.c $(IDIR)/makeargv.h
	$(CC) -c -o $@ $< $(CFLAGS)

checkcycle.o: checkcycle.c $(IDIR)/checkcycle.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~ graphexec tsort*

test:
	./graphexec ../pa1-release/testgraph0.txt
	./graphexec ../pa1-release/testgraph1.txt
	./graphexec ../pa1-release/testgraph2.txt
	./graphexec ../pa1-release/testgraph3.txt
	./graphexec ../pa1-release/testgraph4.txt
	./graphexec ../pa1-release/testgraph5.txt
	./graphexec ../pa1-release/testCycle0.txt
	./graphexec ../pa1-release/testCycle1.txt

