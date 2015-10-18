#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * check if the file exists
 * @param  filepath path to the file
 * @return          1 if the file exists, 0 if it doesn't exist, -1 on error.
 */
int file_exists(char *filepath);

/**
 * check if the directory exitsts
 * @param  dirpath path to the directory
 * @return         1 if the directory exists, 0 if it doesn't exist, -1 on
 * error.
 */
int dir_exists(char *dirpath);

/**
 * make a clone of the "file" named "file_clone" in the same directory
 * @param  filepath path to the file
 * @return          the path to the "file_clone" on success, otherwise NULL
 */
char *clone_file(char *filepath);

/**
 * recursively apply function fn to all the files in the dirpath
 * @param  dirpath        path to the directory
 * @param  fn             function to be applied to files in the directory
 * @param  report_file    file to be written as report
 * @param  hardlink_array array of hard links that has been seen
 * @param  size           the current size of the hardlink array
 * @return                0 on success, -1 otherwise
 */
int recursive_dir(char *dirpath, int (*fn)(char *filepath), FILE *report_file,
                  ino_t *hardlink_array, int *size);

/**
 * check if a file is an useless hard link (we call a hard link useless
 * if it has already been processed)
 * @param  entry_stat     the stat information of the file
 * @param  hardlink_array array of hardlinks that has been seen
 * @param  size           the current size of the hardlink array
 * @return                1 if it's useless, 0 if not.
 */
int is_useless_hardlink(struct stat *entry_stat, ino_t *hardlink_array,
                        int size);

#endif  // UTILITY_H
