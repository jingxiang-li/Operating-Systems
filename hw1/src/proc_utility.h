/**
 * This file contains utility functions for running processes
 *
 * Author: Jingxiang Li
 */
#include "proctype.h"

#ifndef PROC_UTILITY_H
#define PROC_UTILITY_H
#define ARGV_DELIMITERS " "

/**
 * run process given a process node
 * @param  n a pointer to the process node
 * @return   0 on success, -1 otherwise.
 */
int runProcess(ProcNode *n);

/**
 * parse a process node to argv for execl
 * @param  n a pointer to the process node
 * @return   a well structured argv on success, NULl otherwise
 */
char **procNodeToArgv(ProcNode *n);

#endif
