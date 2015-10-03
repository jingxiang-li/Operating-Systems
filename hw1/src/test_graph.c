#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main() {
  int num_nodes = 5;
  Graph *graph = createGraph(num_nodes);
  addEdge(graph, 0, 1);
  addEdge(graph, 0, 2);
  addEdge(graph, 0, 3);
  addEdge(graph, 1, 2);
  addEdge(graph, 2, 3);
  addEdge(graph, 3, 4);
  addEdge(graph, 4, 3);
  printGraph(graph);
  printf("\n\n");

  Graph *graph_rev = reverseGraph(graph);
  printGraph(graph_rev);

  freeGraph(graph);
  freeGraph(graph_rev);
  return 0;
}
