/**
 * This file contains the main functions for recursively decoding
 * or encoding all files in a given directory
 *
 * Usage: ./codec_4061 -[ed] <input_directory> <output_directory>
 *
 * Author: Jingxiang Li
 * Date: Fri 23 Oct 2015 01:16:24 AM CDT
 */

#include "./decode.h"
#include "./encode.h"
#include "./utility.h"
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// static int kPathSize = 1024;
static const int kBufferSize = 4096;
static const int kMaxInodeNum = 64;

int main(int argc, char **argv) {
    // check arguments
    if (argc != 4) {
        fprintf(
            stderr,
            "Usage: ./codec_4061 -[ed] <input_directory> <output_directory>\n");
        return -1;
    }
    char *flag = argv[1];
    char *input_dir = argv[2];
    char *output_dir = argv[3];

    // make and change to the output folder
    if (-1 == make_output_folder(output_dir, input_dir)) return -1;

    // make the report file
    char report_file_path[kBufferSize];
    if (sprintf(report_file_path, "../%s_report.txt", basename(input_dir)) <
        0) {
        fprintf(stderr, "Failed to construct path to the report file\n");
        perror(NULL);
        return -1;
    }
    FILE *report_file = fopen(report_file_path, "w");

    // make the hard link array
    ino_t hardlink_array[kMaxInodeNum];
    int size_of_hardlink_array = 0;

    // recursively process the directory
    if (0 == strcmp(flag, "-e")) {
        recursive_dir("./", encode_file, report_file, hardlink_array,
                      &size_of_hardlink_array);
    } else if (0 == strcmp(flag, "-d")) {
        recursive_dir("./", decode_file, report_file, hardlink_array,
                      &size_of_hardlink_array);
    } else {
        fprintf(stderr,
                "Invalid flag, Usage: ./codec_4061 -[ed] <input_directory> "
                "<output_directory>\n");
        return -1;
    }

    // sort the report file
    fclose(report_file);
    if (-1 == sort_file(report_file_path)) {
        return -1;
    }

    // free resources and return
    return 0;
}
