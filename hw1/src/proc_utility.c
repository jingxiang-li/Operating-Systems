#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "makeargv.h"
#include "proc_utility.h"

static const int buffer_size = 1024;

int runProcess(ProcNode *n) {
  if (n == NULL) {
    fprintf(stderr,
            "Can't run the process, illegal access to the processs node!!\n");
    return -1;
  }

  pid_t child_id;
  child_id = fork();
  if (child_id == -1) {
    fprintf(stderr, "Failed to fork a child\n");
    return -1;
  }
  if (child_id == 0) {
    // this is the child, execute the process
    if (strcmp(n->output, "blank-out.txt") != 0) {
      // we need a output file
      int output_fd = open(n->output, O_WRONLY | O_CREAT | O_TRUNC, 0700);
      if (output_fd < 0) {
        fprintf(stderr, "Failed to create or write %s\n", n->output);
        exit(EXIT_FAILURE);
      }
      char **execl_argv = procNodeToArgv(n);
      if (execl_argv == NULL) {
        fprintf(stderr,
                "Failed to construct an argument array for the process\n");
        exit(EXIT_FAILURE);
      }

      // save stdout
      int saved_stdout = dup(STDOUT_FILENO);
      dup2(output_fd, STDOUT_FILENO);
      close(output_fd);
      // printf("%s\n", execl_argv[0]);
      execvp(execl_argv[0], execl_argv + 1);

      // restore stdout
      dup2(saved_stdout, STDOUT_FILENO);
      fprintf(stderr, "Failed to execute the program\n");
      exit(EXIT_FAILURE);
    } else {
      // we don't need a output file
      char **execl_argv = procNodeToArgv(n);
      if (execl_argv == NULL) {
        fprintf(stderr,
                "Failed to construct an argument array for the process\n");
        exit(EXIT_FAILURE);
      }
      execvp(execl_argv[0], execl_argv + 1);
      fprintf(stderr, "Failed to execute the program\n");
      exit(EXIT_FAILURE);
    }
  }
  // parent here
  int status;
  wait(&status);
  if (!WIFEXITED(status)) return -1;
  return 0;
}

char **procNodeToArgv(ProcNode *n) {
  char *prog = n->prog;
  char input[buffer_size];
  strcpy(input, n->input);

  // check if the input file is available
  FILE *stream;
  stream = fopen(input, "r");
  if (stream == NULL) {
    if (strcmp(input, "blank-file.txt") == 0)
      input[0] = '\0';
    else {
      fprintf(stderr, "Not avaliable input file %s\n", input);
      return NULL;
    }
  } else
    fclose(stream);

  char **prog_argv;
  int numtokens;
  if ((numtokens = makeargv(prog, ARGV_DELIMITERS, &prog_argv)) == -1) {
    fprintf(stderr, "Failed to construct an argument array for %s\n", prog);
    return NULL;
  }

  // construct a proc_buffer used for making argv
  char *proc_buffer = (char *)malloc(buffer_size * sizeof(char));
  proc_buffer[buffer_size - 1] = '\0';
  snprintf(proc_buffer, buffer_size - 1, "%s %s %s ", prog_argv[0],
           prog_argv[0], input);
  for (int i = 1; i != numtokens; i++) {
    strncat(proc_buffer, prog_argv[i], buffer_size - 1 - strlen(proc_buffer));
    strncat(proc_buffer, " ", buffer_size - 1 - strlen(proc_buffer));
  }
  // printf("%s\n", proc_buffer);

  freemakeargv(prog_argv);

  char **execl_argv;
  if ((numtokens = makeargv(proc_buffer, ARGV_DELIMITERS, &execl_argv)) == -1) {
    fprintf(stderr, "Failed to construct an argument array for %s\n",
            proc_buffer);
    return NULL;
  }
  return execl_argv;
}
