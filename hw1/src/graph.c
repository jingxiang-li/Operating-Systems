#include "graph.h"

Graph createGraph(int num_nodes) {
    Graph graph;
    graph.num_nodes = num_nodes;
}
 
void addEdge(Graph *graph, int node_src, int node_dest) {
    // check node_src and node_dest
    Node *node = graph->nodes + node_src;
    // check overflow here
    (node->children)[node->num_children] = node_dest;
    node->num_children++;
}
 
void printGraph(Graph *graph) {
    int i, j; // for loop iterators
    for (i = 0; i < graph->num_nodes; i++) {
        printf("%d, %d edges: ", i, (graph->nodes + i)->num_children);
        Node *node = graph->nodes + i;
        for (j = 0; j < node->num_children; j++) {
            printf("%d, ", node->children[j]);
        }
        printf("\n");
    }
}