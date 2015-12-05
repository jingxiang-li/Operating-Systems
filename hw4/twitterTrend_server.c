/**
 * This file contains the main function for the server side of twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 05:02:13 PM CST
 */

#include "./client_db.h"
#include "./message.h"
#include "./server_utility.h"
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#ifndef LOCKS
#define LOCKS
sem_t sem_queue, sem_items, sem_slots;
#endif

int main(int argc, char **argv) {
    // parse arguments
    int portno, num_threads;
    if (2 == argc) {
        portno = atoi(argv[1]);
        num_threads = 5;
    } else if (3 == argc) {
        portno = atoi(argv[1]);
        num_threads = atoi(argv[2]);
    } else {
        fprintf(stderr, "./twitterTrend_server port_number num_threads\n");
        exit(EXIT_FAILURE);
    }
    if (num_threads < 1) {
        fprintf(stderr, "number of threads should be larger than 0\n");
        exit(EXIT_FAILURE);
    }

    //===========================================================
    // set-up TCP server
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        perror("ERROR, failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (-1 == bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
        perror("ERROR, failed to bind address to socket");
        exit(EXIT_FAILURE);
    }

    //=========================================================================
    // multi-thread programming from here
    // create the socket queue
    Queue *queue = create_queue(num_threads);
    if (NULL == queue) exit(EXIT_FAILURE);

    // create the twitter keywords database
    DB *twitter_db = create_db("./TwitterDB.txt");
    if (NULL == twitter_db) exit(EXIT_FAILURE);

    sem_init(&sem_queue, 0, 1);
    sem_init(&sem_items, 0, 0);
    sem_init(&sem_slots, 0, queue->max_size);

    pthread_t threads[num_threads];

    // initialize the argvs for each thread
    Queue_Process_Argv thread_argvs[num_threads];

    // create threads for pop elements from the queue and process them
    for (int i = 0; i != num_threads; i++) {
        thread_argvs[i].queue = queue;
        thread_argvs[i].twitter_db = twitter_db;
        thread_argvs[i].thread_id = i + 1;
        if (0 != pthread_create(threads + i, NULL, process_queue_mt,
                                thread_argvs + i)) {
            fprintf(stderr, "Failed to create thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    listen(sockfd, 100);
    printf("server listens\n");

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        printf("server accepts connection\n");
        // don't close it, child thread will close it
        if (-1 == newsockfd) {
            fprintf(stderr, "ERROR, Failed to accept connection\n");
            exit(EXIT_FAILURE);
        }

        // CRITICAL SECTION====================================================
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

        if (-1 == push(queue, newsockfd)) {
            fprintf(stderr, "Failed to push socket to the queue\n");
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

    close(sockfd);
    return 0;
}
