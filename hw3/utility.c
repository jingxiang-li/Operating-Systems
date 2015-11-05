#include "./utility.h"
#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>

static const int kBufferSize = 256;

extern pthread_mutex_t queue_access;
extern pthread_cond_t queue_not_full;
extern pthread_cond_t queue_not_empty;

extern sem_t sem_queue, sem_items, sem_slots;

int change_wd(char *input_file_path) {
    char *input_path_dup, *wd;
    input_path_dup = strdup(input_file_path);
    wd = dirname(input_path_dup);
    if (NULL == wd) {
        fprintf(stderr, "Failed to get the parent directory for %s\n",
                input_path_dup);
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
    char output_path[kBufferSize];
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

        // CRITICAL SECTION====================================================
        if (-1 == sem_wait(&sem_items)) {
            fprintf(stderr, "Failed to wait items in the queue\n");
            perror(NULL);
            return NULL;
        }

        if (-1 == sem_wait(&sem_queue)) {
            fprintf(stderr, "Failed to unlock the queue when it's free\n");
            perror(NULL);
            return NULL;
        }

        /**
         * if no client is left for processing, unlock the queue access
         * then return immediately
         */
        if (0 == *num_clients_left) {
            sem_post(&sem_queue);
            return NULL;
        }

        // pop element from the queue
        pop_result = pop(queue, &index);
        if (-1 == pop_result) {
            fprintf(stderr, "Failed to pop element from the queue\n");
            sem_post(&sem_queue);
            return NULL;
        }
        // decrement the counter for clients
        (*num_clients_left)--;

        // get the client information using the index
        Client *client = get_client(client_db, index);
        if (NULL == client) {
            fprintf(stderr, "Failed to get client from the client_db\n");
            sem_post(&sem_queue);
            return NULL;
        }

        char *client_name = client->name;
        char *city_name = client->city_name;

        printf("Thread %d is handling client %s\n", thread_id, client_name);
        // end of processing part

        // unlock the queue access
        sem_post(&sem_queue);

        /**
         * if no client is left for processing, wake up all threads
         * waiting for items
         */
        if (0 == *num_clients_left) {
            for (int i = 0; i != queue->max_size; i++)
                if (-1 == sem_post(&sem_items)) {
                    if (EAGAIN == errno)
                        break;
                    else {
                        fprintf(
                            stderr,
                            "Failed to wake up threads waiting for items\n");
                        perror(NULL);
                        return NULL;
                    }
                }
        }

        /**
         * now there is at least one empty slot in the queue, wake
         * up threads waiting for empty slots
         */
        if (-1 == sem_post(&sem_slots)) {
            fprintf(stderr,
                    "Failed to wake up threads waiting for empty slots\n");
            perror(NULL);
            return NULL;
        }
        // END OF CRITICAL SECTION=============================================

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

void *process_queue_mt(void *_argv) {
    // parse _argv to a pointer to Queue_Process_Argv
    Queue_Process_Argv *argv = (Queue_Process_Argv *)_argv;

    Queue *queue = argv->queue;
    Client_DB *client_db = argv->client_db;
    DB *twitter_db = argv->twitter_db;
    int thread_id = argv->thread_id;
    int *num_clients_left = argv->num_clients_left;

    // call single thread version process_queue
    return process_queue(queue, client_db, twitter_db, thread_id,
                         num_clients_left);
}
