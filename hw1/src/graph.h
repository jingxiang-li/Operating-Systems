/**
 * This file contains data structure and functions for graph. The
 * implementation is adjacency list. Each graph has an array of AdjNode with
 * size num_nodes. Each adjNode has a list of AdjList for storing node_id's it
 * points to.
 *
 * For example, if a graph contains 7 edges (0, 1), (0, 2), (1, 2), (2, 0),
 * (2, 1), (3, 2), (3, 0), then our graph data structure will look like this:
 *
 * 0: 1 -> 2
 * 1: 2
 * 2: 0 -> 1
 * 3: 2 -> 0
 *
 * where
 * node_array = [0, 1, 2, 3]
 * for AdjNode 0, the AdjList will be 1 -> 2 suggesting that 0
 * points to 1 and 2.
 *
 * Author: Jingxiang Li
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

/**
 * data structure for holding edges
 */
typedef struct AdjList_t {
  int node_id;
  struct AdjList_t *next;
} AdjList;

/**
 * data structure for holding nodes
 */
typedef struct AdjNode_t {
  int num_edges;
  struct AdjList_t *head;
} AdjNode;

/**
 * data structure for graph
 */
typedef struct Graph_t {
  int num_nodes;
  struct AdjNode_t *node_array;
} Graph;

/**
 * create a graph object
 * @param  num_nodes number of nodes in the graph
 * @return           return a pointer to the graph object on success,
 *                   otherwise NULL
 */
Graph *createGraph(int num_nodes);

/**
 * reverse a given graph
 * @param  graph a pointer to the graph object to be reversed
 * @return       return a pointer to the reversed graph object on success,
 *               otherwise NULL
 */
Graph *reverseGraph(Graph *graph);

/**
 * free the graph object
 * @param graph a pointer to the graph object
 */
void freeGraph(Graph *graph);

/**
 * add edge to the graph
 * @param  graph a pointer to the graph object
 * @param  src   source node
 * @param  dest  destination node
 * @return       0 on success, otherwise -1
 */
int addEdge(Graph *graph, int src, int dest);

/**
 * print the graph to standard output
 * @param graph a pointer to the graph object
 */
void printGraph(Graph *graph);

#endif
