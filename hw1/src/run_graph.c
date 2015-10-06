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
#include "run_graph.h"

int runGraph(ProcNode *proc_node_array, Graph *dep_graph, int *fork_array,
             int num_proc) {
  // make temporary folder
  if (makeTmpFolder() == -1)
    return -1;
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
      printf("Failed to wait children\n");
      free(child_1_pids);
      markProcFailure(proc_id);
      exit(EXIT_FAILURE);
    }
    free(child_1_pids);

    if (wait_all(proc_id, dep_graph) == -1) {
      markProcFailure(proc_id);
      exit(EXIT_FAILURE);
    }

    if (runProcess(proc_node_array + proc_id) == -1) {
      markProcFailure(proc_id);
      exit(EXIT_FAILURE);
    }

    markProcSuccess(proc_id);
    exit(EXIT_SUCCESS);
  }
  return childid;
}

int wait_children(pid_t *pids, int num_pids) {
  // printf("comes to wait_children\n");
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

  while (1) {
    int num_finished = 0;
    dep = deps.head;
    while (dep != NULL) {
      int dep_id = dep->node_id;
      if (ProcSuccess(dep_id) == 1)
        num_finished++;
      else if (ProcSuccess(dep_id) == -1)
        return -1;
      dep = dep->next;
    }
    if (num_finished == deps.num_edges) break;
  }
  return 0;
}

int fileExist(char *filename) {
  // struct stat buffer;
  // return stat(filename, &buffer) == 0;
  return access(filename, F_OK) != -1;
}

int ProcSuccess(int proc_id) {
  // if something fancy happens, this funcion will also return 0;
  // only check -1 an 1, never check 0
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%df", TMP_FOLDER, proc_id);
  if (fileExist(status_path)) return -1;
  sprintf(status_path, "%s/%d", TMP_FOLDER, proc_id);
  if (fileExist(status_path)) return 1;
  return 0;
}

int markProcSuccess(int proc_id) {
  printf("Process %d succeed\n", proc_id);
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%d", TMP_FOLDER, proc_id);
  FILE *status_file = fopen(status_path, "wa");
  fclose(status_file);
  if (ProcSuccess(proc_id) != 1) {
    printf("Failed to mark process %d is succcess", proc_id);
    perror(NULL);
    return -1;
  }
  return 0;
}

int markProcFailure(int proc_id) {
  printf("Process %d failed\n", proc_id);
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%df", TMP_FOLDER, proc_id);
  FILE *status_file = fopen(status_path, "wa");
  fclose(status_file);
  if (ProcSuccess(proc_id) != -1) {
    printf("Failed to mark process %d is failed", proc_id);
    perror(NULL);
    return -1;
  }
  return 0;
}

int makeTmpFolder() {
  char tmp_dir_prog[PATH_LENGTH];
  sprintf(tmp_dir_prog, "rm -rf %s", TMP_FOLDER);
  if (-1 == system(tmp_dir_prog)) {
    perror("Failed to rm previous temporary folder");
    return -1;
  }
  sprintf(tmp_dir_prog, "mkdir -p %s", TMP_FOLDER);
  if (-1 == system(tmp_dir_prog)) {
    perror("Failed to make temporary folder");
    return -1;
  }
  return 0;
}
