#ifndef GRAPH_RUN_H
#define GRAPH_RUN_H

#include "proctype.h"
#include "proc_utility.h"
#include "graph.h"
#define PATH_LENGTH 256
#define TMP_FOLDER "/tmp/csci4061JingxiangLi"

/**
 * Run graph file
 * @proc_node_array: array of ProcNodes read from graph file
 * @dep_graph: dependency graph parsed from graph file
 * @fork_array: array of process ids determine fork dependencies
 * @num_proc: number of processes in the graph file
 * @return: 0 on success; -1 on failure.
 */
int runGraph(ProcNode *proc_node_array, Graph *dep_graph, int *fork_array,
             int num_proc);

/**
 * Fork and run process [proc_id]
 * @proc_id: id of process
 * @proc_node_array: array of ProcNodes read from graph file
 * @fork_array: array of process ids determine fork dependencies
 * @num_proc: number of processes in the graph file
 * @dep_graph: dependency graph parsed from graph file
 * @return: pid of the forked process on success; -1 on failure.
 */
pid_t forkAndRun(int proc_id, ProcNode *proc_node_array, int *fork_array,
                 int num_proc, Graph *dep_graph);

/**
 * Wait all children processes forked by the parent
 * @pids: array of pids to be waited
 * @num_pids: size of pids
 * @return: 0 on success; -1 on failure
 */
int wait_children(pid_t *pids, int num_pids);

/**
 * Wait all dependencies for process [proc_id]
 * @proc_id, id of process
 * @dep_graph, dependency graph parsed from graph file
 * @return: 0 on success; -1 on failure
 */
int wait_all(int proc_id, Graph *dep_graph);

/**
 * Check if [filename] exists
 * @filename: path to the file
 * @return: 1 if the file exists; 0 otherwise.
 */
int fileExist(char *filename);

/**
 * Check if process completes successfully
 * @proc_id: id of process
 * @return: 1 if the process completes successfully; 0 otherwise.
 */
int ProcSuccess(int proc_id);

/**
 * Mark that the process completes successfully
 * @proc_id: id of process
 * @return: 0 on no error; otherwise -1.
 */
int markProcSuccess(int proc_id);

#endif
