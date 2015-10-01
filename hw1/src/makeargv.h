#ifndef MAKE_ARGV_H
#define MAKE_ARGV_H

int makeargv(const char *s, const char *delimiters, char ***argvp);

void freemakeargv(char **argv);

#endif
