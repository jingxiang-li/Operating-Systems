#include "./utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include "./copyfile.h"

static const int kBufferSize = 4096;

int file_exists(char *filepath) {
    if (NULL == filepath) {
        perror(
            "Failed to check if the file exists, illegal access to a NULL "
            "pointer");
        return -1;
    }
    return 0 == access(filepath, F_OK);
}

int dir_exists(char *dirpath) {
    if (NULL == dirpath) {
        perror(
            "Failed to check if the directory exists, illegal access to a NULL "
            "pointer");
        return -1;
    }

    DIR *dir = opendir(dirpath);
    if (dir) {
        // find the output dir, close it
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        // didn't find the output dir
        return 0;
    } else {
        // failed for some reason, return -1
        fprintf(stderr, "Failed to check if folder %s exists\n", dirpath);
        perror(NULL);
        return -1;
    }
    return -1;
}

char *clone_file(char *filepath) {
    if (1 != file_exists(filepath)) return NULL;

    // construct the name of the cloned file
    char *new_filepath = (char *)malloc(kBufferSize * sizeof(char));

    if (NULL == new_filepath) {
        perror("Failed to allocate memory for the cloned file path");
        return NULL;
    }

    if (sprintf(new_filepath, "%s_clone", filepath) < 0) {
        fprintf(stderr,
                "Failed to construct the name of the cloned version of %s\n",
                filepath);
        return NULL;
    }

    // copy file filepath to new_filepath
    if (-1 == copy(filepath, new_filepath)) return NULL;

    return new_filepath;
}

int recursive_dir(char *dirpath, int (*fn)(char *filepath), FILE *report_file,
                  ino_t *hardlink_array, int *size) {
    if (!dir_exists(dirpath)) return -1;

    DIR *dirp = opendir(dirpath);
    if (NULL == dirp) {
        fprintf(stderr, "Failed to open directory %s\n", dirpath);
        perror(NULL);
        return -1;
    }

    struct dirent *dir_entry;
    struct stat entry_stat;
    char entry_path[kBufferSize];
    while (NULL != (dir_entry = readdir(dirp))) {
        if (strcmp(dir_entry->d_name, ".") == 0 ||
            strcmp(dir_entry->d_name, "..") == 0) {
            continue;
        }

        if (sprintf(entry_path, "%s/%s", dirpath, dir_entry->d_name) < 0) {
            fprintf(stderr, "Failed to construct path for %s\n",
                    dir_entry->d_name);
            return -1;
        }

        if (-1 == lstat(entry_path, &entry_stat)) {
            fprintf(stderr, "Failed to get stat information for file %s\n",
                    entry_path);
            return -1;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            // this is a directory, call recursive_dir on this directory
            fprintf(report_file, "%s, %s, %d, %d\n", basename(entry_path),
                    "directory", 0, 0);
            if (-1 == recursive_dir(entry_path, fn, report_file, hardlink_array,
                                    size))
                return -1;
        } else if (entry_stat.st_nlink > 1) {
            // check if this is a hard link
            printf("%s is a potential hard link\n", entry_path);
            if (is_useless_hardlink(&entry_stat, hardlink_array, *size)) {
                // this is a useless hard link, treat it as symbol link
                fprintf(report_file, "%s, %s, %d, %d\n", basename(entry_path),
                        "hard link", 0, 0);
                continue;
            } else {
                // this is a useful hard link, add it to the hardlink_array
                // and increase the size
                hardlink_array[*size] = entry_stat.st_ino;
                (*size) += 1;

                // treat it as a regular file
                fprintf(report_file, "%s, %s, %ld, ", basename(entry_path),
                        "regular file", entry_stat.st_size);
                if (-1 == fn(entry_path)) return -1;
                if (-1 == lstat(entry_path, &entry_stat)) {
                    fprintf(stderr,
                            "Failed to get stat information for file %s\n",
                            entry_path);
                    return -1;
                }
                fprintf(report_file, "%ld\n", entry_stat.st_size);
            }
        } else if (S_ISREG(entry_stat.st_mode)) {
            // this is a regular file, call fn on this file
            fprintf(report_file, "%s, %s, %ld, ", basename(entry_path),
                    "regular file", entry_stat.st_size);
            if (-1 == fn(entry_path)) return -1;
            if (-1 == lstat(entry_path, &entry_stat)) {
                fprintf(stderr, "Failed to get stat information for file %s\n",
                        entry_path);
                return -1;
            }
            fprintf(report_file, "%ld\n", entry_stat.st_size);
        } else if (S_ISLNK(entry_stat.st_mode)) {
            // this is a symbolic link, skip it
            fprintf(report_file, "%s, %s, %d, %d\n", basename(entry_path),
                    "sym link", 0, 0);
            continue;
        } else {
            fprintf(stderr, "some unkown type of file %s\n", entry_path);
            return -1;
        }
    }

    closedir(dirp);
    return 0;
}

int is_useless_hardlink(struct stat *entry_stat, ino_t *hardlink_array,
                        int size) {
    for (int i = 0; i != size; i++) {
        if (hardlink_array[i] == entry_stat->st_ino) return 1;
    }
    return 0;
}
