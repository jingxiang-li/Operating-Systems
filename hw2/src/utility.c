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

int recursive_dir(char *dirpath, int (*fn)(char *filepath)) {
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
            if (-1 == recursive_dir(entry_path, fn)) return -1;
        } else if (S_ISREG(entry_stat.st_mode)) {
            // this is a regular file, call fn on this file
            if (-1 == fn(entry_path)) return -1;
        } else if (S_ISLNK(entry_stat.st_mode)) {
            // this is a symbolic link, skip it
            continue;
        } else {
            fprintf(stderr, "some unkown type of file %s\n", entry_path);
            return -1;
        }
    }

    closedir(dirp);
    return 0;
}
