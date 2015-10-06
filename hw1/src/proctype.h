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
  // line number in the graph file is the id
  int id;
  // prog + arguments
  char prog[MAX_ARG_LENGTH];
  // input filename
  char input[MAX_ARG_LENGTH];
  // input filename
  char output[MAX_ARG_LENGTH];
  // children IDs, only used in building graph
  int children[MAX_NUM_CHILDREN];
  // number of children, only used in building graph
  int num_children;
  // status of this process, useless
  int status;
  // pid of this process, useless
  pid_t pid;
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
