/**
 * This file contains type declarations and utility functions for process node
 *
 * Author: Jingxiang Li
 */
#include <sys/types.h>

#ifndef PROC_TYPE_H
#define PROC_TYPE_H

#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
#define MAX_NUM_NODES 50
#define MAX_NUM_CHILDREN 10
#define MAX_ARG_LENGTH 1024

/**
 * data structure for holding information of a process
 */
typedef struct {
  int id;                       // corresponds to line number in graph text file
  char prog[MAX_ARG_LENGTH];    // prog + arguments
  char input[MAX_ARG_LENGTH];   // filename
  char output[MAX_ARG_LENGTH];  // filename
  int children[MAX_NUM_CHILDREN];  // children IDs
  int num_children;                // how many children this node has
  int status;
  pid_t pid;  // track it when it's running
} ProcNode;

/**
 * print the process node in a "key: value" format
 * @param  n a pointer to the process node
 * @return   0 on success, -1 otherwise
 */
int printProcNode(ProcNode *n);

/**
 * print the process node in the style of the graph file
 * @param  n a pointer to the process node
 * @return   0 on success, -1 otherwise
 */
int printProcNodeFormat(ProcNode *n);

#endif
