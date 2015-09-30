#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "readGraph.h"
#include "checkcycle.h"

int main(int argc, char **argv) {
  // Graph *graph = createGraph(5);
  // addEdge(graph, 0, 0);
  // addEdge(graph, 0, 1);
  // addEdge(graph, 0, 2);
  // addEdge(graph, 0, 3);
  // addEdge(graph, 1, 1);
  // addEdge(graph, 1, 2);
  // addEdge(graph, 2, 3);
  // addEdge(graph, 2, 0);
  // addEdge(graph, 3, 4);
  // addEdge(graph, 4, 4);
  // printGraph(graph);

  ProcNode *proc_node_array;
  int num_proc;
  num_proc = read_graph_file(argc, argv, &proc_node_array);
  // remember to free proc_node_arrayx
  int i;
  printf("\n");
  for (i = 0; i < num_proc; i++) {
    printProcNodeFormat(proc_node_array + i);
  }
  checkCycle(proc_node_array, num_proc);
  printf("\n\n");
  // return 1 then have cycle, return 0 no cycle, return -1 error happens

  free(proc_node_array);
  return 0;
}
