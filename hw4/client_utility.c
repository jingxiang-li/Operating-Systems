/**
 * This file contains implementations of some utility functions for the
 * client side of twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 09:41:03 PM CST
 */

#include "./client_utility.h"
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const int kBufferSize = 256;

int change_wd(char *input_file_path) {
    char *input_path_dup, *wd;
    input_path_dup = strdup(input_file_path);
    wd = dirname(input_path_dup);
    if (NULL == wd) {
        fprintf(stderr, "Failed to get the parent directory for %s\n",
                input_path_dup);
        perror(NULL);
        return -1;
    }
    if (-1 == chdir(wd)) {
        fprintf(stderr, "Failed to change working directory to %s\n", wd);
        perror(NULL);
        return -1;
    }
    free(input_path_dup);
    return 0;
}

int compile_output(char *client_name, char *city_name, char *keywords) {
    char output_path[kBufferSize];
    sprintf(output_path, "./%s.result", client_name);

    FILE *output_file = fopen(output_path, "w+");
    if (NULL == output_file) {
        fprintf(stderr, "Failed to create file %s\n", output_path);
        perror(NULL);
        return -1;
    }

    fprintf(output_file, "%s : %s\n", city_name, keywords);

    fclose(output_file);
    return 0;
}
