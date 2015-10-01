#include "proctype.h"

#ifndef PROC_UTILITY_H
#define PROC_UTILITY_H
#define ARGV_DELIMITERS " "

int runProcess(ProcNode *n);

char **procNodeToArgv(ProcNode *n);

#endif
