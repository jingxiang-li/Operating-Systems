#include <stdio.h>
#include <stdlib.h>
#include "run_graph.h"

int main() {
  int id = 1;
  printf("%d\n", ProcSuccess(id));
  printf("%d\n", markProcSuccess(id));
  printf("%d\n", ProcSuccess(id));
  return 0;
}
