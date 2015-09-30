#include "./header/checkcycle.h"

int checkCycle (ProcNode *proc_node_array, int num_proc) {
	int i, j;

	if (proc_node_array == NULL) {
		fprintf(stderr, "Failed to check cycle in the graph\n");
		return -1;
	}
	FILE *tsort_input = fopen("tsort_input.txt", "w");
	if (tsort_input == NULL) {
		fprintf(stderr, "Failed to create tsort_input.txt\n");
		return -1;
	}
	for (i = 0; i != num_proc; i++) {
		if (proc_node_array[i].num_children == 0)
			continue;
		for (j = 0; j != proc_node_array[i].num_children; j++) {
			fprintf(tsort_input, "%d %d\n", i, proc_node_array[i].children[j]);
		}
	}
	fclose(tsort_input);

	int saved_stdout, saved_stderr;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);

	int tsort_output_fd = open("./tsort_output.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if (tsort_output_fd < 0){
		fprintf(stderr, "Error opening tsort_output.txt\n");
		return -1;
	} else {
		dup2(tsort_output_fd, STDOUT_FILENO);
		dup2(tsort_output_fd, STDERR_FILENO);
		close(tsort_output_fd);
	}

	pid_t child_id;
	child_id = fork();
	int success_tsort = 1;
	if (child_id == 0) {
		// this is child, execute tsort here
		execl("/usr/bin/tsort", "/usr/bin/tsort", "./tsort_input.txt", NULL);
		success_tsort = 0;
	} else {
		// this is parent, wait for child
		wait(NULL);
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stdout);
		close(saved_stderr);
	}
	if (!success_tsort) {
		fprintf(stderr, "Failed to execute tsort\n");
		return -1;
	}

	FILE *tsort_output = fopen("./tsort_output.txt", "r");
	if (tsort_output == NULL) {
		fprintf(stderr, "Failed to open tsort_output.txt\n");
		return -1;
	}
	char ch = fgetc(tsort_output);
	fclose(tsort_output);
	if (ch == '/') {
		printf("FIND CYCLE!!!\n");
		return 1;
	}
	printf("NO CYCLE!!!\n");
	return 0;
}
