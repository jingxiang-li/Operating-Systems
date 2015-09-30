#include "readGraph.h"

int countLines (FILE *stream) {
	if (stream == NULL) {
		fprintf(stderr, "Failed to count the number of lines");
		exit(EXIT_FAILURE);
	}
	int num_lines = 0;
	char *line = NULL;
    size_t len = 0;
    ssize_t read;
	// note that the last line also has the "\n" sign
	// but in testcycle1 there is no \n in the last line
	while ((read = getline(&line, &len, stream)) != -1) {
		if (strcmp(line, "\n") != 0) num_lines++;
	}
	if (line != NULL)
		free(line);
	fseek(stream, 0, SEEK_SET);
	return num_lines;
}

void convert_argv_to_ProcNode (char **argv, int count, ProcNode *proc_node) {
	if (argv == NULL || proc_node == NULL) {
		fprintf(stderr, "Failed to convert argv to Process Node\n");
		exit(EXIT_FAILURE);
	}

	int i;

	proc_node->id = count;

	memset(proc_node->prog, '\0', 1024);
	strcpy(proc_node->prog, argv[0]);

	memset(proc_node->input, '\0', 1024);
	strcpy(proc_node->input, argv[2]);

	memset(proc_node->output, '\0', 1024);
	strcpy(proc_node->output, argv[3]);

	memset(proc_node->children, 0, 10);
	if (strcmp(argv[1], "none") == 0)
		proc_node->num_children = 0;
	else {
		int num_children;
		char **argv_for_children;
		if ((num_children = makeargv(argv[1], NUM_DELIMITERS, &argv_for_children)) == -1) {
			fprintf(stderr, "Failed to construct an argument array for %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}
		proc_node->num_children = num_children;
		for (i = 0; i < num_children; i++)
			proc_node->children[i] = atoi(argv_for_children[i]);
		freemakeargv(argv_for_children);
	}

	proc_node->status = INELIGIBLE;

	proc_node->pid = 0;
}

void convert_string_to_ProcNode (const char *str, int count, ProcNode *proc_node) {
	// check proc_node here
	char **myargv;
	int numtokens;
	if ((numtokens = makeargv(str, DELIMITERS, &myargv)) == -1) {
		fprintf(stderr, "Failed to construct an argument array for %s\n", str);
		exit(EXIT_FAILURE);
	}
	if (numtokens != 4) {
		fprintf(stderr, "%s cannot be seperated into 4 parts\n", str);
		exit(EXIT_FAILURE);
	}
	convert_argv_to_ProcNode(myargv, count, proc_node);
	freemakeargv(myargv);
	// printProcNode(proc_node);
}

int read_graph_file (int argc, char **argv, ProcNode **proc_node_array) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s string\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;


	stream = fopen(argv[1], "r");
	if (stream == NULL) {
    	printf("Cannot open file %s\n", argv[1]);
    	exit(EXIT_FAILURE);
	}

	int num_lines = countLines(stream);

	*proc_node_array = NULL;
	*proc_node_array = (ProcNode *) malloc(sizeof(ProcNode) * (num_lines + 1));
	// check pointer here
	int count = 0;
   	while ((read = getline(&line, &len, stream)) != -1) {
		// printf("Retrieved line of length %zu :\n", read);
		// printf("%s", line);

		// correct this part!!!, if there is empty line, the countLines() should identify it.
		if (strcmp(line, "\n") == 0) {
			continue;
		}
		convert_string_to_ProcNode(line, count, *proc_node_array + count);
		count++;
	}
	if (line)
    	free(line);
	fclose(stream);
	return num_lines;
}

