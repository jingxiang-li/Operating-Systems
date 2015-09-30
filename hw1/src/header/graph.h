#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "exetype.h"

typedef struct {
    int num_children;
    int children [MAX_NUM_CHILDREN];
} Node;
 
typedef struct {
    int num_nodes;
    Node nodes [MAX_NUM_NODES];
} Graph;

Graph createGraph(int num_nodes);

void addEdge(Graph *graph, int node_src, int node_dest);

void printGraph(Graph *graph);

#endif