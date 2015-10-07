/*login: lixx3899, wang3518
* date: 10/06/2015
* name: Jingxiang Li, Yuchen Wang
* id: 5095269, 4644045 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include "read_graph.h"
#include "checkcycle.h"
#include "proc_utility.h"
#include "graph.h"
#include "run_graph.h"

#define BUFFER_SIZE 1024
#define OUTPUT_DIR "./output"

static int makeOutputDir(char *path);

int main(int argc, char **argv) {
  // read process file as a node array
  ProcNode *proc_node_array;
  int num_proc = read_graph_file(argc, argv, &proc_node_array);
  if (num_proc == -1) return 0;

  // change working dir to OUTPUT_DIR
  if (makeOutputDir(argv[1]) == -1) return 0;

  // check if the process graph has an cycle
  int have_cycle = checkCycle(proc_node_array, num_proc);
  if (have_cycle == 1) {
    printf("Find Cycles\n");
    free(proc_node_array);
    return 0;
  } else if (have_cycle == -1) {
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
static int makeOutputDir(char *path) {
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

  char *file_name = basename(path);
  if (file_name == NULL) {
    perror("can't read name of the graph file");
    return -1;
  }
  // remove char's after '.' sign in the filename
  int length_file_name = strlen(file_name);
  for (int i = 0; i != length_file_name; i++)
    if (file_name[i] == '.') {
      file_name[i] = '\0';
      break;
    }

  // make output dir like OUTPUT_DIR/file_name
  char output_dir[BUFFER_SIZE];
  sprintf(output_dir, "%s/%s", OUTPUT_DIR, file_name);

  if (stat(output_dir, &wd_st) == -1) {
    mkdir(output_dir, 0700);
  }
  if (stat(output_dir, &wd_st) == -1) {
    fprintf(stderr, "can't make output directory %s\n", output_dir);
    perror(NULL);
    return -1;
  }

  // change output dir
  if (chdir(output_dir) == -1) {
    fprintf(stderr, "can't change working directory to %s\n", output_dir);
    perror(NULL);
    return -1;
  }
  return 0;
}
