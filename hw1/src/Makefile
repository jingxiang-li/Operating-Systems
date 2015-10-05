CC=gcc
CFLAGS=-g -Wall -fstack-protector -Wextra -Wunreachable-code -O1 -std=c99

all: graphexec

graphexec: main.o read_graph.o proctype.o makeargv.o checkcycle.o proc_utility.o graph.o graph_run.o
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

graph.o: graph.c graph.h
	$(CC) -c -o $@ $< $(CFLAGS)

graph_run.o: graph_run.c graph_run.h
	$(CC) -c -o $@ $< $(CFLAGS)

testGraph: graph.o test_graph.c proc_utility.o makeargv.o
	$(CC) -o $@ $^ $(CFLAGS)
	./testGraph

testGraphRun: test_graph_run.c graph_run.o proctype.o proc_utility.o graph.o makeargv.o
	$(CC) -o $@ $^ $(CFLAGS)
	./testGraphRun

clean:
	rm -f *.o *~ graphexec tsort* *.out *.txt testGraph testGraphRun
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
