#include <stdio.h>
#include "proctype.h"

int printProcNode(ProcNode *n) {
  if (n == NULL) {
    perror("Can't print the process node, illegal access");
    return -1;
  }
  printf("id: %d\n", n->id);
  printf("prog: %s\n", n->prog);
  printf("input: %s\n", n->input);
  printf("output: %s\n", n->output);
  printf("num_children: %d\n", n->num_children);
  printf("children: ");
  for (int i = 0; i < n->num_children; i++) {
    printf("%d, ", n->children[i]);
  }
  printf("\n");
  printf("status: %d\n", n->status);
  printf("pid: %d\n", n->pid);
  printf("\n");
  return 0;
}

int printProcNodeFormat(ProcNode *n) {
  if (n == NULL) {
    perror("Can't print format the process node, illegal access");
    return -1;
  }
  printf("%s:", n->prog);
  if (n->num_children == 0)
    printf("none:");
  else {
    for (int i = 0; i != n->num_children - 1; i++)
      printf("%d ", n->children[i]);
    printf("%d:", n->children[n->num_children - 1]);
  }
  printf("%s:%s", n->input, n->output);
  printf("\n");
  return 0;
}
