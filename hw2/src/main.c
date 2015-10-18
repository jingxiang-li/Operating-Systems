#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <errno.h>
#include "./utility.h"
#include "./encode.h"

// static int kPathSize = 1024;
static const int kBufferSize = 4096;
static const int kMaxInodeNum = 64;

int main(int argc, char **argv) {
    // check arguments
    if (argc != 4) {
        fprintf(
            stderr,
            "Usage: ./codec_4061 -[ed] <input_directory> <output_directory>\n");
        return 0;
    }
    char *flag = argv[1];
    char *input_dir = argv[2];
    char *output_dir = argv[3];

    // make and change to the output folder
    if (-1 == make_output_folder(output_dir, input_dir)) return 0;

    // make the report file
    char report_file_path[kBufferSize];
    if (sprintf(report_file_path, "../%s_report.txt", basename(input_dir)) <
        0) {
        fprintf(stderr, "Failed to construct path to the report file\n");
        perror(NULL);
        return 0;
    }
    FILE *report_file = fopen(report_file_path, "w");

    // make the hard link array
    ino_t hardlink_array[kMaxInodeNum];
    int size_of_hardlink_array = 0;

    // recursively process the directory
    recursive_dir("./", encode_file, report_file, hardlink_array,
                  &size_of_hardlink_array);

    // free resources and return
    fclose(report_file);
    return 0;
}
