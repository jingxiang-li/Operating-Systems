/**
 * This file contains implementations for encode.h
 *
 * Author: Jingxiang Li
 * Date: Fri 23 Oct 2015 01:13:56 AM CDT
 */

#include "./codec.h"
#include "./encode.h"
#include "./utility.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const int kNumRead = 3;
static const int kNumWrite = 4;

int encode_file(char *filepath) {
#ifdef DEBUG
    printf("processing %s\n", filepath);
#endif  // DEBUG

    if (!file_exists(filepath)) {
        fprintf(stderr, "Failed to decode file %s\n, file doesn't exitsts",
                filepath);
        return -1;
    }

    /**
     * Check if the file to be encoded is empty, if so, we don't need
     * to process it.
     */
    struct stat filestat;
    if (-1 == stat(filepath, &filestat)) {
        perror(NULL);
        return -1;
    }
    if (0 == filestat.st_size) return 0;

    /**
     * If the file is not empty, we encode it and add a newline sign
     * when finish encoding the file
     */
    char *input_path = clone_file(filepath);
    char *output_path = filepath;

    if (!file_exists(input_path) || !file_exists(output_path)) {
        fprintf(stderr, "Failed to make a clone of file %s\n", filepath);
        return -1;
    }

    int input_fd = open(input_path, O_RDONLY);
    int output_fd = open(output_path, O_WRONLY | O_TRUNC);

    if (-1 == input_fd || -1 == output_fd) {
        perror(NULL);
        return -1;
    }

    uint8_t read_buf[kNumRead], write_buf[kNumWrite];
    int num_read, num_write;

    while (1) {
        memset(read_buf, 0, kNumRead);
        memset(write_buf, 0, kNumWrite);
        num_read = read(input_fd, read_buf, kNumRead);

        if (0 == num_read) break;  // finish reading the file

        if (-1 == num_read) {
            fprintf(stderr, "Failed to read bytes from %s\n", input_path);
            return -1;
        }

        num_write = encode_block(read_buf, write_buf, num_read);
        if (num_write != write(output_fd, write_buf, num_write)) {
            fprintf(stderr, "Failed to write %d bytes into %s\n", num_write,
                    output_path);
            return -1;
        }
    }

    /**
     * write a newline sign to the output_fd
     */
    write_buf[0] = 0x0a;
    if (1 != write(output_fd, write_buf, 1)) {
        fprintf(stderr, "Failed to write the newline sign to %s\n",
                output_path);
        return -1;
    }

    /**
     * close everything, remove the file_clone
     */
    close(input_fd);
    close(output_fd);
    if (-1 == unlink(input_path)) {
        fprintf(stderr, "Failed to remove file %s\n", input_path);
        return -1;
    }
    return 0;
}
