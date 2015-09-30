#include <stdio.h>
#include "proctype.h"

#ifndef READ_GRAPH_H
#define READ_GRAPH_H
#define DELIMITERS ":\n"
#define NUM_DELIMITERS " "

int countLines(FILE *stream);

void convert_argv_to_ProcNode(char **argv, int count, ProcNode *proc_node);

void convert_string_to_ProcNode(const char *line, int count,
                                ProcNode *proc_node);

int read_graph_file(int argc, char **argv, ProcNode **proc_node_array);

#endif
