#include <sys/types.h>

#ifndef PROC_TYPE_H
#define PROC_TYPE_H

#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
#define MAX_NUM_NODES 50
#define MAX_NUM_CHILDREN 10

typedef struct {
  int id;             // corresponds to line number in graph text file
  char prog[1024];    // prog + arguments
  char input[1024];   // filename
  char output[1024];  // filename
  int children[10];   // children IDs
  int num_children;   // how many children this node has
  int status;
  pid_t pid;  // track it when it's running
} ProcNode;

void printProcNode(ProcNode *n);

void printProcNodeFormat(ProcNode *n);

#endif
