#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <errno.h>

// static int kPathSize = 1024;
static int kBufferSize = 4096;

int make_output_folder(char *output_dir, char *flag, char *dirname);

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(
            stderr,
            "Usage: ./codec_4061 -[ed] <input_directory> <output_directory>\n");
        return 0;
    }
    char *flag = argv[1];
    char *input_dir = argv[2];
    char *output_dir = argv[3];
    if (-1 == make_output_folder(output_dir, flag, input_dir))
        return 0;
    printf("%s\n", getcwd(NULL, 1024));
}

int make_output_folder(char *output_dir, char *flag, char *input_dir) {
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
    if (sprintf(system_call_cp, "cp -rf %s %s/", input_dir, output_dir) < 0) {
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
