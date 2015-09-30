#ifndef READ_GRAPH_H
#define READ_GRAPH_H

#define _GNU_SOURCE // getline() is not C standard but GNU extension
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "makeargv.h"
#include "proctype.h"

#define DELIMITERS ":\n"
#define NUM_DELIMITERS " "

int countLines (FILE *stream);

void convert_argv_to_ProcNode (char **argv, int count, ProcNode *proc_node);

void convert_string_to_ProcNode (const char *line, int count, ProcNode *proc_node);

int read_graph_file (int argc, char **argv, ProcNode **proc_node_array);





#endif