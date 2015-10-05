#include <stdio.h>
#include <stdlib.h>
#include "graph_run.h"

int main() {
  int id = 1;
  printf("%d\n", ProcSuccess(id));
  printf("%d\n", markProcSuccess(id));
  printf("%d\n", ProcSuccess(id));
  return 0;
}
