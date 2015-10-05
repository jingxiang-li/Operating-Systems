#ifndef GRAPH_RUN_H
#define GRAPH_RUN_H

#include "proctype.h"
#include "proc_utility.h"
#include "graph.h"
#define PATH_LENGTH 256
#define TMP_FOLDER "/tmp/csci4061JingxiangLi"

int runGraph(ProcNode *proc_node_array, Graph *dep_graph,
             int *fork_array, int num_proc);

int runGraphProc(ProcNode *proc_node_array, int proc_id, Graph *dep_graph,
                 int *fork_array, int num_proc);

int forkChildren(ProcNode *proc_node_array, int proc_id, Graph *dep_graph,
                 int *fork_array, int num_proc);

int fileExist(char *filename);

int ifProcSuccess(int proc_id);

int markProcSuccess(int proc_id);

#endif
