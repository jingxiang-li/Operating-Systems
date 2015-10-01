#include "proctype.h"

#ifndef CHECK_CYCLE_H
#define CHECK_CYCLE_H

int checkCycle(ProcNode *proc_node_array, int num_proc);

int checkCycleFancy(ProcNode *proc_node_array, int num_proc,
                    int **topological_order);

#endif