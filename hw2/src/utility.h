#ifndef UTILITY_H
#define UTILITY_H

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
 * process all files inside the directory using function fn recursively
 * @param  filepath path to the file
 * @param  f        a function which will be applied to all regular
 * files inside the directory.
 * @return          0 on success; -1 otherwise.
 */
int recursive_dir(char *dirpath, int (*fn)(char *filepath));

#endif  // UTILITY_H
