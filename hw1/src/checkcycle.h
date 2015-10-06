/**
 * This file contains functions for checking if the graph contains a cycle.
 *
 * Author: Jingxiang Li
 */

#include "proctype.h"

#ifndef CHECK_CYCLE_H
#define CHECK_CYCLE_H

/**
 * check if the process graph contains a cycle, using shell command tsort
 * @param  proc_node_array process node array parsed from the graph file
 * @param  num_proc        number of processes
 * @return                 return 1 if there is a cycle, return 0 if there is
 *                         no cycle, return -1 if there is an error
 *                         occurs.
 */
int checkCycle(ProcNode *proc_node_array, int num_proc);

/**
 * check if the process graph contains a cycle, using shell command tsort,
 * provide the topological sort of processes
 * @param  proc_node_array   process node array parsed from the graph file
 * @param  num_proc          number of processes
 * @param  topological_order pointer to array contains topological order of
 *                           processes
 * @return                   return 1 if there is a cycle, return 0 if there
 *                           is no cycle, return -1 if there is an error occurs.
 */
int checkCycleFancy(ProcNode *proc_node_array, int num_proc,
                    int **topological_order);

#endif
