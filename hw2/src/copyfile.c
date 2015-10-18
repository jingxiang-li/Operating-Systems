#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int copy(char *source, char *dest) {
    int childExitStatus;
    pid_t pid;

    if (!source || !dest) {
        fprintf(stderr, "Illegal access to NULL pointer when copy files");
        return -1;
    }

    pid = fork();
    if (pid == 0) {
        // child comes here, execute cp
        execl("/bin/cp", "/bin/cp", source, dest, (char *)0);
    } else if (pid < 0) {
        fprintf(stderr, "Failed to fork child process when copy file %s\n",
                source);
        return -1;
    } else {
        // parent comes here, wait for the child
        pid_t ws = waitpid(pid, &childExitStatus, 0);
        if (ws == -1) {
            fprintf(stderr,
                    "Failed to wait child process when copying file %s\n",
                    source);
            perror(NULL);
            return -1;
        }

        if (!WIFEXITED(childExitStatus)) {
            fprintf(stderr, "Failed to copy file %s\n", source);
            perror(NULL);
            return -1;
        }
    }
    return 0;
}
