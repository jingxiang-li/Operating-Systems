#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "proc_utility.h"

int main() {
  int num_nodes = 1;
  Graph *graph = createGraph(num_nodes);
  printGraph(graph);
  printf("\n\n");

  Graph *graph_rev = reverseGraph(graph);
  printf("reverse the graph\n");
  printGraph(graph_rev);
  printf("\n\n");

  freeGraph(graph);
  freeGraph(graph_rev);
  return 0;
}
