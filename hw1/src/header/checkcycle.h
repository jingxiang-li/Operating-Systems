#ifndef CHECK_CYCLE_H
#define CHECK_CYCLE_H

#include <unistd.h>
#include "proctype.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int checkCycle (ProcNode *proc_node_array, int num_proc);

#endif