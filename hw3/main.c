/**
 * This file contains the main function for the executable file twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Tue 03 Nov 2015 02:20:47 AM CST
 */

#include "./client_db.h"
#include "./queue.h"
#include "./twitter_db.h"
#include "./utility.h"
#include <libgen.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef LOCKS
#define LOCKS
sem_t sem_queue, sem_items, sem_slots;
#endif

int main(int argc, char **argv) {
    if (3 != argc) {
        fprintf(stderr, "Usage: ./twitterTrend input_file_path num_threads\n");
        exit(EXIT_FAILURE);
    }

    char *input_file_path = argv[1];
    int num_threads = atoi(argv[2]);

    if (num_threads < 1) {
        fprintf(stderr, "number of threads should be larger than 0\n");
        exit(EXIT_FAILURE);
    }

    // create the client queue
    Queue *queue = create_queue(num_threads);
    if (NULL == queue) exit(EXIT_FAILURE);

    // create the client database
    Client_DB *client_db = create_client_db(input_file_path);
    if (NULL == client_db) exit(EXIT_FAILURE);

    // change working directory to where the input file is
    if (-1 == change_wd(input_file_path)) exit(EXIT_FAILURE);

    // create the twitter keywords database
    DB *twitter_db = create_db("./TwitterDB.txt");
    if (NULL == twitter_db) exit(EXIT_FAILURE);

    // specify the number of clients need to be processed
    int *num_clients_left = (int *)malloc(sizeof(int));
    *num_clients_left = client_db->size;

    //=========================================================================
    // multi-thread programming from here

    sem_init(&sem_queue, 0, 1);
    sem_init(&sem_items, 0, 0);
    sem_init(&sem_slots, 0, queue->max_size);

    int push_result;
    pthread_t threads[num_threads];

    // initialize the argvs for each thread
    Queue_Process_Argv thread_argvs[num_threads];

    // create threads for pop elements from the queue and process them
    for (int i = 0; i != num_threads; i++) {
        thread_argvs[i].queue = queue;
        thread_argvs[i].client_db = client_db;
        thread_argvs[i].twitter_db = twitter_db;
        thread_argvs[i].thread_id = i + 1;
        thread_argvs[i].num_clients_left = num_clients_left;
        if (0 != pthread_create(threads + i, NULL, process_queue_mt,
                                thread_argvs + i)) {
            fprintf(stderr, "Failed to create thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    int sem_slots_val;

    // push element to the queue in tha main thread
    for (int i = 0; i < client_db->size; i++) {
        // CRITICAL SECTION====================================================

        // get the current value from the semaphore of queue slots
        // to determine if the main thread need to wait for a
        // empty slot
        if (-1 == sem_getvalue(&sem_slots, &sem_slots_val)) {
            fprintf(stderr,
                    "Failed to get value from the semaphore of queue slots\n");
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        if (0 == sem_slots_val) {
            printf("Waiting to add clients to the full queue\n");
        }

        // wait for a empty slot in the queue
        if (-1 == sem_wait(&sem_slots)) {
            fprintf(stderr, "Failed to wait empty slots in the queue\n");
            perror(NULL);
            exit(EXIT_FAILURE);
        }

        // wait for the queue access
        if (-1 == sem_wait(&sem_queue)) {
            fprintf(stderr,
                    "Failed to wait the queue_access in the main thread\n");
            perror(NULL);
            exit(EXIT_FAILURE);
        }

        push_result = push(queue, i);
        if (-1 == push_result) {
            fprintf(stderr, "Failed to push integer %d to the queue\n", i);
            exit(EXIT_FAILURE);
        }

        if (-1 == sem_post(&sem_queue)) {
            fprintf(stderr,
                    "Main thread failed to wake up threads waiting for the "
                    "queue access\n");
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        if (-1 == sem_post(&sem_items)) {
            fprintf(stderr,
                    "Failed to wake up threads waiting for avaliable items in "
                    "the queue\n");
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        // END OF CRITICAL SECTION=============================================
    }

    // Join threads
    for (int i = 0; i != num_threads; i++) {
        if (0 != pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Failed to join thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    // free resources
    free_queue(queue);
    free_db(twitter_db);
    free_client_db(client_db);
    free(num_clients_left);
    exit(EXIT_SUCCESS);
}
