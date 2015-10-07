/**
 * This file contains functions for running the graph processes
 *
 * Implementation Details:
 *
 * First, we need proc_node_array, dep_graph and fork_array read from
 * the graph file.
 *
 * To run the graph, we use fork_array to determine which processes should
 * be forked by the current process, and call forkAndRun on those processes,
 * then wait for all dependencies and finally execute current process. The
 * whole procedure can be done recursively, the pseudo code for the process is
 *
 * forkAndRun(process_id) {
 *   fork();
 *   if (child)
 *     for i: fork_array[i] == process_id
 *       forkAndRun(i)
 *     wait_all_dependencies(process_id)
 *     execute(process_id);
 *     exit()
 *   if (parent)
 *     wait()
 *     return
 * }
 *
 * Note that if fork_array[i] == -1, then i should be called forkAndRun
 * by the main process, which means they should be called forkAndRun
 * at the very beginning.
 *
 * To determine the status of each process, we use "file indicator".
 * That is, once a process completes successfully, we create a file named
 * "[proc_id]" in the temporary folder. if a process failed to execute, we
 * create a file named "[proc_id]f" in the temporary folder. Then it's easy
 * check status of any process in the graph -- just check if the file exists.
 *
 * Author: Jingxiang Li
 */

#ifndef RUN_GRAPH_H
#define RUN_GRAPH_H

#include "proctype.h"
#include "proc_utility.h"
#include "graph.h"
#define PATH_LENGTH 1024
#define TMP_FOLDER "/tmp/csci4061JingxiangLi"

/**
 * Run graph file
 * @param  proc_node_array array of process node read from graph file
 * @param  dep_graph       dependency graph parsed from the graph file
 * @param  fork_array      array of process ids determine fork dependencies
 * @param  num_proc        number of processes in the graph file
 * @return                 0 on success; -1 on failure
 */
int runGraph(ProcNode *proc_node_array, Graph *dep_graph, int *fork_array,
             int num_proc);

/**
 * Fork, wait and run the process [proc_id]
 * @param  proc_id         id of the process
 * @param  proc_node_array array of ProcNodes read from graph file
 * @param  fork_array      array of process id's determine fork dependencies
 * @param  num_proc        number of processes in the graph file
 * @param  dep_graph       dependency graph parsed from graph file
 * @return                 pid of the forked process on success; -1 on failure
 */
pid_t forkAndRun(int proc_id, ProcNode *proc_node_array, int *fork_array,
                 int num_proc, Graph *dep_graph);

/**
 * Wait for all child processes forked by the parent
 * @param  pids     array of pids to be waited
 * @param  num_pids size of pids
 * @return          0 on success; -1 on failure
 */
int wait_children(pid_t *pids, int num_pids);

/**
 * Wait all dependencies for process [proc_id] by "file indicator"
 * @param  proc_id   id of process
 * @param  dep_graph dependency graph parsed from graph file
 * @return           0 on success; -1 on failure
 */
int wait_all(int proc_id, Graph *dep_graph);

/**
 * Check if [filename] exists
 * @param  filename path to the file
 * @return          1 if the file exists; 0 otherwise
 */
int fileExist(char *filename);

/**
 * Check the status of a process by "file indicator"
 * @param  proc_id id of process
 * @return         1 if the process completes successfully;
 *                 -1 if the process failed to execute;
 *                 0 if the process is running or something fancy happens.
 *                 NEVER CHECK 0 while using this function!!!
 */
int ProcSuccess(int proc_id);

/**
 * Mark that the process completes successfully using "file indicator"
 * @param  proc_id id of process
 * @return         0 on success; -1 otherwise
 */
int markProcSuccess(int proc_id);

/**
 * Mark that the process failed to execute using "file indicator"
 * @param  proc_id id of process
 * @return         0 on success; -1 otherwise
 */
int markProcFailure(int proc_id);

/**
 * Make a temporary folder for holding those "file indicators"
 * @return 0 on success; -1 otherwise
 */
int makeTmpFolder();

#endif
