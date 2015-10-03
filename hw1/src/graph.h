#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

/*
  This file contains data structure and functions for graph. The implementaion is adjacency list. Each graph has an array of AdjNode with size num_nodes. Each adjNode has a list of AdjList for storing node_id's it points to.

  For example, if a graph contains 7 edges (0, 1), (0, 2), (1, 2), (2, 0), (2, 1), (3, 2), (3, 0), then our graph data structure will look like this:

  0: 1 -> 2
  1: 2
  2: 0 -> 1
  3: 2 -> 0

  where
  node_array = [0, 1, 2, 3]
  for AdjNode 0, the AdjList will be 1 -> 2 suggesting that 0 points to 1 and 2.
*/

typedef struct AdjList_t {
  int node_id;
  struct AdjList_t *next;
} AdjList;

typedef struct AdjNode_t {
  int num_edges;
  struct AdjList_t* head;
} AdjNode;

typedef struct Graph_t {
  int num_nodes;
  struct AdjNode_t* node_array;
} Graph;


Graph *createGraph(int num_nodes);

Graph *reverseGraph(Graph* graph);

void freeGraph(Graph *graph);

int addEdge(Graph* graph, int src, int dest);

void printGraph(Graph *graph);

#endif
