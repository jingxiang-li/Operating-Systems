/**
 * This file contains functions for reading and parsing graph file to a
 * proc_node array
 *
 * Author: Jingxiang Li
 */
#include <stdio.h>
#include "proctype.h"
#include "graph.h"

#ifndef READ_GRAPH_H
#define READ_GRAPH_H
#define DELIMITERS ":\n"
#define NUM_DELIMITERS " "

/**
 * count lines of a file
 * @param  stream the file stream
 * @return        number of lines on success, -1 on failure
 */
int countLines(FILE *stream);

/**
 * convert argv to a process node
 * @param  argv      argv parsed from the graph file
 * @param  count     index of the process
 * @param  proc_node pointer to the target process node
 * @return           0 on success, -1 on failure
 */
int convert_argv_to_ProcNode(char **argv, int count, ProcNode *proc_node);

/**
 * convert a string in graph file to a process node
 * @param  line      string in the graph file
 * @param  count     index of the process
 * @param  proc_node pointer to the target process node
 * @return           0 on success, -1 on failure
 */
int convert_string_to_ProcNode(const char *line, int count,
                               ProcNode *proc_node);

/**
 * parse a graph file to an array of process node
 * @param  argc            argc in the main function, should be 2
 * @param  argv            argv in the main function, argv[1] is the path to
 *                         graph file
 * @param  proc_node_array pointer to the target process node array
 * @return                 number of processes on success, -1 on failure
 */
int read_graph_file(int argc, char **argv, ProcNode **proc_node_array);

/**
 * buld process graph and process dependency graph from the proc_node_array
 * @param  proc_node_array array of process node
 * @param  num_proc        number of processes
 * @param  proc_graph      pointer to the target process graph
 * @param  dep_graph       pointer to the target dependency graph
 * @return                 0 on success, -1 on failure
 */
int buildGraphs(ProcNode *proc_node_array, int num_proc, Graph **proc_graph,
                Graph **dep_graph);

/**
 * make fork array to determine the fork dependency, process i should be
 * forked by fork_array[i], if fork_array[i] == -1, then i should be forked
 * by the main process.
 * @param  proc_graph pointer to the process graph parsed from the graph file
 * @return            return pointer to the fork_array on success,
 *                    otherwise NULL
 */
int *makeForkArray(Graph *proc_graph);

#endif
