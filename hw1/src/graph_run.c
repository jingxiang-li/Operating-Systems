#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "proctype.h"
#include "proc_utility.h"
#include "graph.h"
#include "graph_run.h"

int runGraph(ProcNode *proc_node_array, Graph *dep_graph, int *fork_array,
             int num_proc) {
  system("rm -rf /tmp/csci4061JingxiangLi");
  system("mkdir -p /tmp/csci4061JingxiangLi");
  // fork children and run processes
  pid_t childid;
  pid_t *child_pids = (pid_t *)malloc(sizeof(pid_t) * num_proc);
  int num_forks = 0;
  for (int i = 0; i != num_proc; i++) {
    // if fork_array[i] == -1, then process i should be forked and run
    if (fork_array[i] != -1) continue;
    childid = forkAndRun(i, proc_node_array, fork_array, num_proc, dep_graph);
    if (childid == -1) {
      fprintf(stderr, "Can't fork child for process %d", i);
      perror(NULL);
      break;
    }
    child_pids[num_forks++] = childid;
  }

  // wait for all children forked by this process
  if (wait_children(child_pids, num_forks) == -1) {
    free(child_pids);
    return -1;
  }

  // everything is fine
  free(child_pids);
  return 0;
}

pid_t forkAndRun(int proc_id, ProcNode *proc_node_array, int *fork_array,
                 int num_proc, Graph *dep_graph) {
  pid_t childid = fork();
  if (childid == -1) {
    // if failed, parent return -1
    fprintf(stderr, "Can't fork child for process %d", proc_id);
    perror(NULL);
    return -1;
  }
  if (childid == 0) {
    // fork and run all processes associated with proc_id
    // after running, exit success or failure
    pid_t childid_1;
    pid_t *child_1_pids = (pid_t *)malloc(sizeof(pid_t) * num_proc);
    int num_child_1 = 0;
    for (int i = 0; i != num_proc; i++) {
      if (fork_array[i] != proc_id) continue;
      childid_1 =
          forkAndRun(i, proc_node_array, fork_array, num_proc, dep_graph);
      child_1_pids[num_child_1++] = childid_1;
    }
    if (wait_children(child_1_pids, num_child_1) == -1) {
      free(child_1_pids);
      exit(EXIT_FAILURE);
    }
    free(child_1_pids);
    wait_all(proc_id, dep_graph);
    if (runProcess(proc_node_array + proc_id) == -1)
      exit(EXIT_FAILURE);
    if (markProcSuccess(proc_id) == -1)
      exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
  }
  return childid;
}

int wait_children(pid_t *pids, int num_pids) {
  int status;
  int ret_val = 0;
  for (int i = 0; i != num_pids; i++) {
    waitpid(pids[i], &status, 0);
    if (!WIFEXITED(status)) {
      printf("Failed to execute process %d\n", pids[i]);
      perror(NULL);
      ret_val = -1;
      continue;
    }
  }
  return ret_val;
}

int wait_all(int proc_id, Graph *dep_graph) {
  AdjNode deps = dep_graph->node_array[proc_id];
  AdjList *dep = NULL;
  int num_wait = 1;
  while (num_wait != 0) {
    num_wait = 0;
    dep = deps.head;
    while (dep != NULL) {
      int dep_id = dep->node_id;
      if (!ProcSuccess(dep_id)) num_wait++;
      dep = dep->next;
    }
  }
  return 0;
}

int fileExist(char *filename) {
  struct stat buffer;
  return stat(filename, &buffer) == 0;
}

int ProcSuccess(int proc_id) {
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%d", TMP_FOLDER, proc_id);
  // printf("%s\n", status_path);
  return fileExist(status_path);
}

int markProcSuccess(int proc_id) {
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%d", TMP_FOLDER, proc_id);
  FILE *status_file = fopen(status_path, "wa");
  fclose(status_file);
  if (ProcSuccess(proc_id) == -1) {
    printf("Failed to mark process %d is succcess", proc_id);
    perror(NULL);
    return -1;
  } else
    return 0;
}
