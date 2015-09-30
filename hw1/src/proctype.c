#include "proctype.h"

void printProcNode (ProcNode *n) {
	printf("id: %d\n", n->id);
	printf("prog: %s\n", n->prog);
	printf("input: %s\n", n->input);
	printf("output: %s\n", n->output);
	printf("num_children: %d\n", n->num_children);
	int i;
	printf("children: ");
	for (i = 0; i < n->num_children; i++) {
		printf("%d, ", n->children[i]);
	}
	printf("\n");
	printf("status: %d\n", n->status);
	printf("pid: %d\n", n->pid);
	printf("\n");
}
