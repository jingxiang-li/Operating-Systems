#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "read_graph.h"
#include "checkcycle.h"
#include "proc_utility.h"
#include "graph.h"

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
  // read process file as a node array
  ProcNode *proc_node_array;
  int num_proc = read_graph_file(argc, argv, &proc_node_array);

  // print the node array for debug
  // for (int i = 0; i < num_proc; i++) {
  //   printProcNode(proc_node_array + i);
  //   if (runProcess(proc_node_array + i) == -1)
  //     continue;
  // }

  // change working dir to ./output
  struct stat wd_st = {0};
  if (stat("./output", &wd_st) == -1) {
    mkdir("./output", 0700);
  }
  if (stat("./output", &wd_st) == -1) {
    perror("can't make output directory ./output");
    return -1;
  }

  char wd_buf[BUFFER_SIZE];
  if (getcwd(wd_buf, BUFFER_SIZE) == NULL) {
    perror("can't read currect working directory");
    return -1;
  }
  if (strcat(wd_buf, "/output") == NULL) {
    perror("can't construct new working directory ./output");
    return -1;
  }
  if (chdir(wd_buf) == -1) {
    perror("can't change working directory to ./output");
    return -1;
  }

  // check if the process graph has an cycle
  int *topological_order;
  if (checkCycleFancy(proc_node_array, num_proc, &topological_order) != 0) {
    // have cycle or some error happens
    free(proc_node_array);
    return 0;
  }

  // build proc_graph and dep_graph for the proc_nodes
  Graph *proc_graph, *dep_graph;
  buildGraphs(proc_node_array, num_proc, &proc_graph, &dep_graph);

  // generate fork_array to dertmine who fork who
  int *fork_array = makeForkArray(proc_graph);



  // free resources and return
  free(fork_array);
  freeGraph(proc_graph);
  freeGraph(dep_graph);
  free(proc_node_array);
  return 0;
}
