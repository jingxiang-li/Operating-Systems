#ifndef MAKE_ARGV_H
#define MAKE_ARGV_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int makeargv (const char *s, const char *delimiters, char ***argvp);

void freemakeargv(char **argv);

#endif
