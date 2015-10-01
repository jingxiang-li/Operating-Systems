#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "read_graph.h"
#include "checkcycle.h"
#include "proc_utility.h"

int main(int argc, char **argv) {
  // read process file as a node array
  ProcNode *proc_node_array;
  int num_proc = read_graph_file(argc, argv, &proc_node_array);

  // print the node array for debug
  // for (int i = 0; i < num_proc; i++) {
  //   printProcNodeFormat(proc_node_array + i);
  //   if (runProcess(proc_node_array + i) == -1)
  //     continue;
  // }

  // check if the process graph has an cycle
  int *topological_order;
  if (checkCycleFancy(proc_node_array, num_proc, &topological_order) == 1) {
    free(proc_node_array);
    return 0;
  }

  // try to run processes
  for (int i = 0; i < num_proc; i++) {
    // printProcNodeFormat(proc_node_array + topological_order[i]);
    if (runProcess(proc_node_array + topological_order[i]) == -1) continue;
  }

  // make a temporary folder for holding the status files of processes
  // struct stat st = {0};
  // if (stat("./.tmp", &st) == -1)
  //     mkdir("./.tmp", 0700);

  free(proc_node_array);
  return 0;
}
