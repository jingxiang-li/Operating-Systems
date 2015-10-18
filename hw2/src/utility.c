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
#include <sys/wait.h>
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
#ifdef DEBUG
                fprintf(report_file, "%s, %s, %d, %d\n", basename(entry_path),
                        "hard link", 0, 0);
#endif  // DEBUG
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

int make_output_folder(char *output_dir, char *input_dir) {
    /**
     * make the output folder
     */
    DIR *dir = opendir(output_dir);
    if (dir) {
        // find the output dir, close it
        closedir(dir);
    } else if (ENOENT == errno) {
        // didn't find the output dir, make it
        if (-1 == mkdir(output_dir, S_IRWXU | S_IRWXG | S_IRWXO)) {
            perror("Failed to make output directory");
            return -1;
        }
    } else {
        // failed for some reason, return -1
        fprintf(stderr, "Failed to check if folder %s exists\n", output_dir);
        perror(NULL);
        return -1;
    }

    /**
     * remove directory with the name same as the input folder,
     * inside the output folder
     */
    char system_call_rm[kBufferSize];
    if (sprintf(system_call_rm, "rm -rf %s/%s", output_dir,
                basename(input_dir)) < 0) {
        perror("Failed to make copy argument");
        return -1;
    }
    if (-1 == system(system_call_rm)) {
        perror("Failed to remove directory with the input directory's name");
        return -1;
    }

    /**
     * copy input folder into the output folder
     */
    char system_call_cp[kBufferSize];
    if (sprintf(system_call_cp, "cp -ar %s %s/", input_dir, output_dir) < 0) {
        perror("Failed to make copy argument");
        return -1;
    }
    if (-1 == system(system_call_cp)) {
        perror("Failed to copy input dir into the output dir");
        return -1;
    }

    /**
     * change the current working directory to the output folder
     */
    char new_wd[kBufferSize];
    if (sprintf(new_wd, "%s/%s", output_dir, basename(input_dir)) < 0) {
        perror("Failed to make new working directory argument");
        return -1;
    }
    if (-1 == chdir(new_wd)) {
        perror("Failed to change working directory");
        return -1;
    }
    return 0;
}

int sort_file(char *filepath) {
    if (!file_exists(filepath)) {
        fprintf(stderr, "Failed to sort file %s, file doesn't exists\n",
                filepath);
        return -1;
    }
    char *input_path = clone_file(filepath);
    char *output_path = filepath;

    pid_t child_id = fork();
    int childExitStatus;
    if (child_id == -1) {
        fprintf(stderr, "Failed to sort file %s, can't fork child\n", filepath);
        perror(NULL);
    }

    if (child_id == 0) {
        // child goes here, redirect stdout and execute sort
        int output_fd = open(output_path, O_WRONLY | O_TRUNC);
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
        execlp("sort", "sort", input_path, (char *)0);
    } else {
        pid_t ws = wait(&childExitStatus);
        if (ws == -1) {
            fprintf(stderr,
                    "Failed to wait child process while sorting file %s\n",
                    filepath);
            perror(NULL);
            return -1;
        }
        if (!WIFEXITED(childExitStatus)) {
            fprintf(stderr, "Failed to sort file %s\n", filepath);
            perror(NULL);
            return -1;
        }
    }
    if (-1 == unlink(input_path)) {
        fprintf(stderr, "Failed to remove file %s\n", input_path);
        return -1;
    }
    return 0;
}
