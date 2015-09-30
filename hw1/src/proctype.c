#include <stdio.h>
#include "proctype.h"

void printProcNode(ProcNode *n) {
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
}

void printProcNodeFormat(ProcNode *n) {
  printf("%s:", n->prog);
  for (int i = 0; i!= n->num_children; i++)
    printf("%d ", n->children[i]);
  printf(":%s:%s", n->input, n->output);
  printf("\n");
}

//echo wazzup:2 3:blank-file.txt:1echo2.out
