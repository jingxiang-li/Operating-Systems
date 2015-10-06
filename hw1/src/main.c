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
#include "run_graph.h"

#define BUFFER_SIZE 1024
#define OUTPUT_DIR "../output"

static int makeOutputDir();

int main(int argc, char **argv) {
  // change working dir to OUTPUT_DIR
  if (makeOutputDir() == -1)
    return 0;

  // read process file as a node array
  ProcNode *proc_node_array;
  int num_proc = read_graph_file(argc, argv, &proc_node_array);

  // check if the process graph has an cycle
  int *topological_order;
  if (checkCycleFancy(proc_node_array, num_proc, &topological_order) != 0) {
    // have cycle
    printf("Find Cycles\n");
    free(proc_node_array);
    return 0;
  }

  // build proc_graph and dep_graph for the proc_nodes
  Graph *proc_graph, *dep_graph;
  buildGraphs(proc_node_array, num_proc, &proc_graph, &dep_graph);

  // generate fork_array to dertmine the fork dependency
  // proc i should be forked by fork_array[i]
  // if fork_array[i] == -1, then this proc should be forked by the main process
  // at the very begining
  int *fork_array = makeForkArray(proc_graph);

  // run Graph file
  runGraph(proc_node_array, dep_graph, fork_array, num_proc);

  // free resources and return
  free(fork_array);
  freeGraph(proc_graph);
  freeGraph(dep_graph);
  free(proc_node_array);
  return 0;
}

/**
 * make and change output directory using const string OUTPUT_DIR
 * @return 0 on success; -1 otherwise
 */
static int makeOutputDir() {
  // make a OUTPUT_DIR
  struct stat wd_st;
  if (stat(OUTPUT_DIR, &wd_st) == -1) {
    mkdir(OUTPUT_DIR, 0700);
  }
  if (stat(OUTPUT_DIR, &wd_st) == -1) {
    fprintf(stderr, "can't make output directory %s\n", OUTPUT_DIR);
    perror(NULL);
    return -1;
  }
  // change the OUTPUT_DIR
  char cur_wd[BUFFER_SIZE];
  if (getcwd(cur_wd, BUFFER_SIZE) == NULL) {
    perror("can't read currect working directory");
    return -1;
  }
  char new_wd[BUFFER_SIZE];
  sprintf(new_wd, "%s/%s", cur_wd, OUTPUT_DIR);
  if (chdir(new_wd) == -1) {
    fprintf(stderr, "can't change working directory to %s\n", new_wd);
    perror(NULL);
    return -1;
  }
  return 0;
}
