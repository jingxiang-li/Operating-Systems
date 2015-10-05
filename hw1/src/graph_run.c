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
  // fork children and run processes
  pid_t childid;
  pid_t *child_pids = (pid_t *)malloc(sizeof(pid_t) * num_proc);
  int num_forks = 0;
  for (int i = 0; i != num_proc; i++) {
    // if fork_array[i] == -1, then this process should be executed by the main process
    if (fork_array[i] != -1) continue;

    childid = fork();
    if (childid == -1) {
      // error handling
      fprintf(stderr, "Can't fork child for process %d", i);
      perror(NULL);
    }
    if (childid == 0) {
      // child comes here, run process, exit after execution
      if (runGraphProc(proc_node_array, i, dep_graph, fork_array, num_proc) ==
          -1)
        exit(EXIT_FAILURE);
      exit(EXIT_SUCCESS);
    } else {
      // parent record the pid of child
      child_pids[num_forks++] = childid;
    }
  }

  // wait for all children forked by this process
  int status;
  for (int i = 0; i != num_forks; i++) {
    waitpid(child_pids[i], &status, 0);
    if (!WIFEXITED(status)) {
      printf("Failed to execute process %d\n", child_pids[i]);
      perror(NULL);
      free(child_pids);
      return -1;
    }
  }

  // everything is fine
  free(child_pids);
  return 0;
}

int runGraphProc(ProcNode *proc_node_array, int proc_id, Graph *dep_graph,
                 int *fork_array, int num_proc) {
  printf("%d\n", proc_id);
  return 0;
}
/*
int runGraphProc(ProcNode *proc_node_array, int proc_id, Graph *dep_graph,
                 int *fork_array, int num_proc) {
  ProcNode *proc_node = proc_node_array + proc_id;

  int num_forked_child;
  num_forked_child =
      forkChildren(proc_node_array, proc_id, fork_array, dep_graph);
  if (num_forked_child == -1) {
    fprintf(stderr, "Failed to fork child processes for process %d\n", proc_id);
    return -1;
  }

  if (runProcess(proc_node) == -1) {
    fprintf(stderr, "Failed to run process %d \n", proc_id);
    return -1;
  }

  // once success, create a file named [proc_id] in the tmp file forlder
  // indicating that the process completes succesfully
  if (ifProcSuccess(proc_id)) {
    printf(
        "the status indicator file shouldn't exists, something wrong "
        "happens\n");
    return -1;
  }
  if (markProcSuccess(proc_id) == -1) {
    return -1;
  }
  return 0;
}

int forkChildren(ProcNode *proc_node_array, int proc_id, int *fork_array,
                 Graph *dep_graph) {
  ProcNode proc_node = proc_node_array[proc_id];
  int num_children = proc_node.num_children;
  pid_t *child_pids = (pid_t *)malloc(num_children * sizeof(pid_t));
  if (child_pids == NULL) {
    printf("Failed to allocate memory for the child_pid_array for node %d",
           proc_id);
    perror(NULL);
    return -1;
  }

  pid_t childid;

  int num_forked_child = 0;
  int size_fork_array = sizeof(fork_array) / sizeof(int);
  for (int i = 0; i != size_fork_array; i++) {
    if (fork_array[i] == proc_id) {
      // let proc_id fork i
      childid = fork();
      if (childid == 0) {
        // child here, run process i
        if (runGraphProc(proc_node_array, i, dep_graph, fork_array) == -1) {
          fprintf(stderr, "Failed to execute process %d", i);
          exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
      } else
        child_pids[num_forked_child++] = childid;
    }
  }
  // parent process comes here, wait all children, including those forked by
  // other processes
  int status;
  for (int i = 0; i != num_forked_child; i++) {
    while (waitpid(child_pids[i], &status, 0) == -1)
      ;
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
      printf("Failed to execute excute process %d\n", child_pids[i]);
      free(child_pids);
      exit(EXIT_FAILURE);
    }
  }

  // wait all children by ifProcSuccess
  AdjNode deps = dep_graph->node_array[proc_id];
  int need_wait = deps.num_edges;
  AdjList *dep = NULL;
  while (need_wait != 0) {
    need_wait = 0;
    dep = deps.head;
    while (dep != NULL) {
      int dep_id = dep->node_id;
      if (!ifProcSuccess(dep_id)) need_wait++;
      dep = dep->next;
    }
  }
  free(child_pids);
  return num_forked_child;
}

int fileExist(char *filename) {
  struct stat buffer;
  return (stat(filename, &buffer) == 0);
}

int ifProcSuccess(int proc_id) {
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%d", TMP_FOLDER, proc_id);
  printf("%s\n", status_path);
  if (fileExist(status_path))
    return 0;
  else
    return -1;
}

int markProcSuccess(int proc_id) {
  char status_path[PATH_LENGTH];
  sprintf(status_path, "%s/%d", TMP_FOLDER, proc_id);
  FILE *status_file = fopen(status_path, "wa");
  fclose(status_file);
  if (ifProcSuccess(proc_id)) {
    printf("Failed to mark process %d is succcess", proc_id);
    perror(NULL);
    return 0;
  } else
    return -1;
}
*/
