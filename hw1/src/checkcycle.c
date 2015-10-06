#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proctype.h"
#include "checkcycle.h"

int checkCycle(ProcNode *proc_node_array, int num_proc) {
  int i, j;

  if (proc_node_array == NULL) {
    perror("Failed to check cycle in the graph");
    return -1;
  }
  FILE *tsort_input = fopen("tsort_input.txt", "w");
  if (tsort_input == NULL) {
    perror("Failed to create tsort_input.txt");
    return -1;
  }

  int num_edges = 0;
  for (i = 0; i != num_proc; i++) {
    if (proc_node_array[i].num_children == 0) continue;
    for (j = 0; j != proc_node_array[i].num_children; j++) {
      fprintf(tsort_input, "%d %d\n", i, proc_node_array[i].children[j]);
      num_edges++;
    }
  }
  fclose(tsort_input);
  if (num_edges == 0)  // no cycle
    return 0;

  int tsort_output_fd =
      open("./tsort_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (tsort_output_fd < 0) {
    perror("Error opening tsort_output.txt");
    return -1;
  }

  pid_t child_id;
  child_id = fork();
  if (child_id == -1) {
    perror("Failed to fork a child");
    return -1;
  }
  if (child_id == 0) {
    // this is child, execute tsort here
    // save and redirect stdout and stderr
    int saved_stdout, saved_stderr;
    saved_stdout = dup(STDOUT_FILENO);
    saved_stderr = dup(STDERR_FILENO);
    dup2(tsort_output_fd, STDOUT_FILENO);
    dup2(tsort_output_fd, STDERR_FILENO);
    close(tsort_output_fd);
    // execute tsort
    execlp("tsort", "tsort", "./tsort_input.txt", (char *)NULL);

    // if fails, restore file descriptors
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stdout);
    close(saved_stderr);
    perror("Failed to execute tsort");
    exit(EXIT_FAILURE);
  } else {
    // this is parent, wait for child
    int status;
    wait(&status);
    if (!WIFEXITED(status)) return -1;
  }

  // get tsort output and determine whether these is a cycle
  FILE *tsort_output = fopen("./tsort_output.txt", "r");
  if (tsort_output == NULL) {
    perror("Failed to open tsort_output.txt");
    return -1;
  }
  char ch = fgetc(tsort_output);
  fclose(tsort_output);

  // remove tsort_output.txt and tsort_input.txt
  if (remove("tsort_input.txt") == -1) {
    perror("Unable to delete ./tsort_input.txt");
    return -1;
  }
  if (remove("tsort_output.txt") == -1) {
    perror("Unable to delete ./tsort_output.txt");
    return -1;
  }

  if (ch < '0' || ch > '9') return 1;
  return 0;
}

int checkCycleFancy(ProcNode *proc_node_array, int num_proc,
                    int **topological_order) {
  int i, j;
  if (proc_node_array == NULL) {
    perror("Failed to check cycle in the graph");
    return -1;
  }
  FILE *tsort_input = fopen("tsort_input.txt", "w");
  if (tsort_input == NULL) {
    perror("Failed to create tsort_input.txt");
    return -1;
  }

  int num_edges = 0;
  for (i = 0; i != num_proc; i++) {
    if (proc_node_array[i].num_children == 0) continue;
    for (j = 0; j != proc_node_array[i].num_children; j++) {
      fprintf(tsort_input, "%d %d\n", i, proc_node_array[i].children[j]);
      num_edges++;
    }
  }
  fclose(tsort_input);
  if (num_edges == 0) {
    // no cycle
    *topological_order = (int *)malloc(num_proc * sizeof(int));
    for (int i = 0; i != num_proc; i++) {
      *topological_order[i] = i;
    }
    return 0;
  }

  int tsort_output_fd =
      open("./tsort_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (tsort_output_fd < 0) {
    perror("Error opening tsort_output.txt");
    return -1;
  }

  pid_t child_id;
  child_id = fork();
  if (child_id == -1) {
    perror("Failed to fork a child");
    return -1;
  }
  if (child_id == 0) {
    // this is child, execute tsort here
    // save and redirect stdout and stderr
    int saved_stdout, saved_stderr;
    saved_stdout = dup(STDOUT_FILENO);
    saved_stderr = dup(STDERR_FILENO);
    dup2(tsort_output_fd, STDOUT_FILENO);
    dup2(tsort_output_fd, STDERR_FILENO);
    close(tsort_output_fd);
    // execute tsort
    execlp("tsort", "tsort", "./tsort_input.txt", (char *)NULL);

    // if fails, restore file descriptors
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stdout);
    close(saved_stderr);
    perror("Failed to execute tsort");
    exit(EXIT_FAILURE);
  } else {
    // this is parent, wait for child
    int status;
    wait(&status);
    if (!WIFEXITED(status)) return -1;
  }

  // get tsort output and determine whether these is a cycle
  FILE *tsort_output = fopen("./tsort_output.txt", "r");
  if (tsort_output == NULL) {
    perror("Failed to open tsort_output.txt");
    return -1;
  }

  *topological_order = (int *)malloc(num_proc * sizeof(int));
  const int buffer_size = 32;
  char line[buffer_size];
  size_t len = buffer_size;
  int node_id, ii = 0;
  while (fgets(line, len, tsort_output) != NULL) {
    if (line[0] < '0' || line[0] > '9') {
      // find cycles
      // remove tsort_output.txt and tsort_input.txt
      if (remove("./tsort_input.txt") == -1) {
        perror("Unable to delete ./tsort_input.txt");
        return -1;
      }
      if (remove("./tsort_output.txt") == -1) {
        perror("Unable to delete ./tsort_output.txt");
        return -1;
      }
      fclose(tsort_output);
      return 1;
    }
    sscanf(line, "%d", &node_id);
    (*topological_order)[ii++] = node_id;
  }

  // remove tsort_output.txt and tsort_input.txt
  if (remove("./tsort_input.txt") == -1) {
    perror("Unable to delete ./tsort_input.txt");
    return -1;
  }
  if (remove("./tsort_output.txt") == -1) {
    perror("Unable to delete ./tsort_output.txt");
    return -1;
  }

  fclose(tsort_output);
  return 0;
}
