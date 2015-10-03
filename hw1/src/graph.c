#include "graph.h"
#include "proctype.h"

Graph *createGraph(int num_nodes) {
  if (num_nodes <= 0) {
    fprintf(stderr, "Can't make graph with 0 nodes\n");
    return NULL;
  }
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (graph == NULL) {
    perror("Failed to allocate memory for the Graph structure");
    return NULL;
  }
  graph->num_nodes = num_nodes;
  graph->node_array = (AdjNode *)malloc(num_nodes * sizeof(AdjNode));
  if (graph->node_array == NULL) {
    perror("Failed to allocate momory for the node_array inside the graph structure");
    return NULL;
  }
  for (int i = 0; i != num_nodes; i++) {
    graph->node_array[i].num_edges = 0;
  }
  return graph;
}

void freeGraph(Graph *graph) {
  AdjNode *node_array = graph->node_array;
  for (int i = 0; i != graph->num_nodes; i++) {
    AdjList *node = node_array[i].head;
    while (node != NULL) {
      AdjList *tmp_node = node->next;
      free(node);
      node = tmp_node;
    }
  }
  free(graph->node_array);
  free(graph);
}

int addEdge(Graph* graph, int src, int dest) {
  int num_nodes = graph->num_nodes;
  if (src < 0 || dest < 0 || src >= num_nodes || dest >= num_nodes) {
    fprintf(stderr, "Failed to add edge (%d, %d) into the graph, index out of bounds\n", src, dest);
      return -1;
  }
  AdjNode *node_array = graph->node_array;
  AdjList *new_node = (AdjList *)malloc(sizeof(AdjList));
  if (new_node == NULL) {
    perror("Failed to add edge (%d, %d) into the graph, can't allocate memory for the new edge\b");
    return -1;
  }
  new_node->node_id = dest;
  new_node->next = node_array[src].head;
  node_array[src].head = new_node;
  node_array[src].num_edges++;
  return 0;
}

void printGraph(Graph* graph) {
  int num_nodes = graph->num_nodes;
  AdjNode *node_array = graph->node_array;
  for (int i = 0; i != num_nodes; i++) {
    printf("%d, %d edges: ", i, node_array[i].num_edges);
    AdjList *node = node_array[i].head;
    while (node != NULL) {
      printf("%d, ", node->node_id);
      node = node->next;
    }
    printf("\n");
  }
}

Graph *reverseGraph(Graph* graph) {
  Graph *graph_rev = (Graph *)malloc(sizeof(Graph));
    if (graph_rev == NULL) {
    perror("Failed to reverse the graph, can't allocate memory for the Graph structure");
    return NULL;
  }
  int num_nodes = graph->num_nodes;
  graph_rev->num_nodes = num_nodes;
  graph_rev->node_array = (AdjNode *)malloc(num_nodes * sizeof(AdjNode));
  if (graph_rev->node_array == NULL) {
    perror("Failed to allocate momory for the node_array inside the reverse graph structure");
    return NULL;
  }
  for (int i = 0; i != num_nodes; i++) {
    graph_rev->node_array[i].num_edges = 0;
  }

  AdjNode *node_array = graph->node_array;
  for (int i = 0; i != num_nodes; i++) {
    AdjList *node = node_array[i].head;
    while (node != NULL) {
      addEdge(graph_rev, node->node_id, i);
      node = node->next;
    }
  }
  return graph_rev;
}


