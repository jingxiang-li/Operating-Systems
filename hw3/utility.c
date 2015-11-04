#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"

static const int kBufferSize = 256;

int change_wd (char *input_file_path) {
    char *input_path_dup, *wd;
    input_path_dup = strdup(input_file_path);
    wd = dirname(input_path_dup);
    if (NULL == wd) {
        fprintf(stderr, "Failed to get the parent directory for %s\n", input_path_dup);
        perror(NULL);
        return -1;
    }
    if (-1 == chdir(wd)) {
        fprintf(stderr, "Failed to change working directory to %s\n", wd);
        perror(NULL);
        return -1;
    }
    free(input_path_dup);
    return 0;
}

int compile_output(char *client_name, char *city_name, char *keywords) {
    char output_path [kBufferSize];
    sprintf(output_path, "./%s.result", client_name);

    FILE *output_file = fopen(output_path, "w+");
    if (NULL == output_file) {
        fprintf(stderr, "Failed to create file %s\n", output_path);
        perror(NULL);
        return -1;
    }

    fprintf(output_file, "%s : %s\n", city_name, keywords);

    fclose(output_file);
    return 0;
}

void *process_queue(Queue *queue, Client_DB *client_db, DB *twitter_db,
                    int thread_id, int *num_clients_left) {
    while (num_clients_left && *num_clients_left != 0) {
        // if there are clients waiting to be processed
        if (queue == NULL || client_db == NULL || twitter_db == NULL) {
            fprintf(stderr,
                    "Failed to process the queue, at least one of queue, "
                    "client_db, twitter_db is NULL pointer, illegal access\n");
            return NULL;
        }

        int index, pop_result;

        // CRITICAL SECTION
        pop_result = pop(queue, &index);
        (*num_clients_left)--;
        // END OF CRITICAL SECTION

        if (-1 == pop_result) {
            fprintf(stderr, "Failed to pop element from the queue\n");
            return NULL;
        }

        Client *client = get_client(client_db, index);
        if (NULL == client) {
            fprintf(stderr, "Failed to get client from the client_db\n");
            return NULL;
        }

        char *client_name = client->name;
        char *city_name = client->city_name;

        printf("Thread %d is handling client %s\n", thread_id, client_name);

        char *keywords = get_keywords(twitter_db, city_name);
        if (NULL == keywords) {
            fprintf(stderr, "Failed to match %s in the twitter database\n",
                    city_name);
            return NULL;
        }

        // compile the result file
        if (-1 == compile_output(client_name, city_name, keywords)) {
            fprintf(stderr, "Failed to compile the output\n");
            return NULL;
        }

        printf("Thread %d finished handling client %s\n", thread_id,
               client_name);
    }
    return NULL;
}

