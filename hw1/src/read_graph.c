#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "makeargv.h"
#include "proctype.h"
#include "read_graph.h"
#include "graph.h"

static const int buffer_size = 1024;
static const int absolute_path_size = 4096;

int countLines(FILE *stream) {
  if (stream == NULL) {
    printf("Failed to open the file stream for counting the number of lines\n");
    perror(NULL);
    return -1;
  }

  int num_lines = 0;
  char line[buffer_size];
  size_t len = buffer_size;
  // note that the last line also has the "\n" sign
  // but in testcycle1 there is no \n in the last line
  while (fgets(line, len, stream) != NULL) {
    if (strcmp(line, "\n") != 0) num_lines++;
  }
  fseek(stream, 0, SEEK_SET);
  return num_lines;
}

int convert_argv_to_ProcNode(char **argv, int count, ProcNode *proc_node) {
  if (argv == NULL || proc_node == NULL) {
    printf(
        "Failed to convert argv to Process Node, argv or proc_node is not "
        "allocated well\n");
    perror(NULL);
    return -1;
  }

  proc_node->id = count;

  memset(proc_node->prog, '\0', 1024);
  strcpy(proc_node->prog, argv[0]);

  memset(proc_node->input, '\0', 1024);
  strcpy(proc_node->input, argv[2]);

  memset(proc_node->output, '\0', 1024);
  strcpy(proc_node->output, argv[3]);
  // strcpy(proc_node->output, "./output/");
  // strcat(proc_node->output, argv[3]);

  memset(proc_node->children, 0, 10);
  if (strcmp(argv[1], "none") == 0)
    proc_node->num_children = 0;
  else {
    int num_children;
    char **argv_for_children;
    if ((num_children =
             makeargv(argv[1], NUM_DELIMITERS, &argv_for_children)) == -1) {
      printf("Failed to construct an argument array for %s\n", argv[1]);
      perror(NULL);
      return -1;
    }
    proc_node->num_children = num_children;
    for (int i = 0; i < num_children; i++)
      proc_node->children[i] = atoi(argv_for_children[i]);
    freemakeargv(argv_for_children);
  }

  proc_node->status = INELIGIBLE;
  proc_node->pid = 0;

  return 0;
}

int convert_string_to_ProcNode(const char *str, int count,
                               ProcNode *proc_node) {
  if (str == NULL || proc_node == NULL) {
    perror(
        "Failed to convert string to ProcNode, str or proc_node is not "
        "well allocated");
    return -1;
  }

  char **myargv;
  int numtokens;
  if ((numtokens = makeargv(str, DELIMITERS, &myargv)) == -1) {
    printf("Failed to construct an argument array for %s\n", str);
    perror(NULL);
    return -1;
  }
  if (numtokens != 4) {
    printf("%s cannot be seperated into 4 parts\n", str);
    return -1;
  }
  if (convert_argv_to_ProcNode(myargv, count, proc_node) == -1) return -1;
  freemakeargv(myargv);
  return 0;
}

int read_graph_file(int argc, char **argv, ProcNode **proc_node_array) {
  if (argc != 2) {
    printf("Usage: %s path/to/file\n", argv[0]);
    return -1;
  }

  char real_path[absolute_path_size];
  if (NULL == realpath(argv[1], real_path)) {
    fprintf(stderr, "Failed to find the absolute path of %s;", argv[1]);
    perror(NULL);
    return -1;
  }

  FILE *stream;
  char line[buffer_size];
  size_t len = buffer_size;
  stream = fopen(real_path, "r");
  if (stream == NULL) {
    printf("Cannot open file %s\n", argv[1]);
    perror(NULL);
    return -1;
  }

  int num_lines = countLines(stream);
  if (num_lines < 1) return -1;

  *proc_node_array = NULL;
  *proc_node_array = (ProcNode *)malloc(sizeof(ProcNode) * (num_lines));
  if (proc_node_array == NULL) {
    printf("Failed to construct process node array\n");
    perror(NULL);
    return -1;
  }

  int count = 0;
  while (fgets(line, len, stream) != NULL && count < num_lines) {
    // printf("Retrieved line of length %zu :\n", read);
    // printf("%s", line);

    // correct this part!!!, if there is empty line, the countLines() should
    // identify it.
    if (strcmp(line, "\n") == 0) continue;
    if (convert_string_to_ProcNode(line, count, *proc_node_array + count) ==
        -1) {
      return -1;
    }
    count++;
  }
  fclose(stream);
  return num_lines;
}

int buildGraphs(ProcNode *proc_node_array, int num_proc, Graph **proc_graph,
                Graph **dep_graph) {
  if (proc_node_array == NULL) {
    fprintf(
        stderr,
        "Failed to build proc_graph and dep_graph for the proc_node_array\n");
    return -1;
  }

  *proc_graph = NULL;
  *proc_graph = createGraph(num_proc);
  if (*proc_graph == NULL) return -1;
  for (int i = 0; i != num_proc; i++) {
    ProcNode proc_node = proc_node_array[i];
    for (int j = 0; j != proc_node.num_children; j++) {
      addEdge(*proc_graph, i, proc_node.children[j]);
    }
  }
  *dep_graph = NULL;
  *dep_graph = reverseGraph(*proc_graph);
  if (*dep_graph == NULL) return -1;
  return 0;
}

int *makeForkArray(Graph *proc_graph) {
  if (proc_graph == NULL) {
    fprintf(stderr,
            "Failed to make fork array for the proc_graph, illigle access\n");
    return NULL;
  }
  int num_proc = proc_graph->num_nodes;
  int *fork_array = (int *)malloc(num_proc * sizeof(int));
  if (fork_array == NULL) {
    perror("Failed to make fork array, can't allocate memory");
    return NULL;
  }
  for (int i = 0; i != num_proc; i++) {
    if (proc_graph->node_array[i].head != NULL)
      fork_array[i] = proc_graph->node_array[i].head->node_id;
    else
      fork_array[i] = -1;
  }
  return fork_array;
}
