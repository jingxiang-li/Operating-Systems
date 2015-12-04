#include "./server_utility.h"
#include "./message.h"
#include <errno.h>
#include <libgen.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern sem_t sem_queue, sem_items, sem_slots;

static int kBufferSize = 32;

void *process_queue(Queue *queue, DB *twitter_db, int thread_id) {
    if (queue == NULL || twitter_db == NULL) {
        fprintf(stderr,
                "ERROR, failed to process the queue, at least one of queue, "
                "twitter_db is NULL pointer, illegal access\n");
        return NULL;
    }

    while (1) {
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

        // pop element from the queue
        int sockfd;
        if (-1 == pop(queue, &sockfd)) {
            fprintf(stderr, "Failed to pop element from the queue\n");
            sem_post(&sem_queue);
            return NULL;
        }

        // unlock the queue access
        if (-1 == sem_post(&sem_queue)) {
            fprintf(stderr, "failed to wake up threads waiting for access to the queue\n");
            perror(NULL);
            return NULL;
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

        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        if (-1 == getpeername(sockfd, (struct sockaddr *)&cli_addr, &cli_len)) {
            perror("ERROR, failed to get name of the client");
            return NULL;
        }
        char client_name [kBufferSize];
        sprintf(client_name, "%s,%d", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        printf("Thread %d is handling client %s\n", thread_id, client_name);


        // INTERACT WITH CLIENT ======================================

        uint32_t msg_id, payload_len;
        char *payload = NULL;
        int status;

        // send hand shake message
        if (-1 == send_msg(sockfd, HANDSHAKE_ID, 0, NULL)) {
            fprintf(stderr,
                    "ERROR, failed to send hand shake message to host\n");
            exit(EXIT_FAILURE);
        }
        printf("server sends handshaking: (100,0,)\n");

        // receive hand shake response
        receive_msg(sockfd, &msg_id, &status);
        if (-1 == status || HANDSHAKE_RESPONSE_ID != msg_id) {
            fprintf(stderr,
                    "ERROR, failed to read hand shake response from host\n");
            exit(EXIT_FAILURE);
        }

        // receive keywords request
        while (1) {
            payload = receive_msg(sockfd, &msg_id, &status);
            if (-1 == status) {
                fprintf(
                    stderr,
                    "ERROR, failed to receive keyword message from client\n");
                return NULL;
            }

            if (END_OF_REQUEST_ID == msg_id) {
                // if the message is end of request, then jump out of this loop
                break;
            } else if (TWITTERTREND_REQUEST_ID != msg_id) {
                // if the message is neither end of request, nor twittertrend
                // request, prompt error and return
                fprintf(
                    stderr,
                    "ERROR, server receive %d when waiting for %ld or %ld\n",
                    msg_id, END_OF_REQUEST_ID, TWITTERTREND_REQUEST_ID);
                return NULL;
            } else {
                // this is a twittertrend request
                // get keywords
                char *city_name = payload;
                char *keywords = get_keywords(twitter_db, city_name);
                if (NULL == keywords) {
                    keywords = (char *)malloc(8 * sizeof(char));
                    sprintf(keywords, "NA");
                }

                // send response
                payload_len = strlen(keywords);
                payload = keywords;
                if (-1 == send_msg(sockfd, RESPONSE_MESSAGE_ID, payload_len,
                                   payload)) {
                    fprintf(stderr,
                            "ERROR, failed to send response to client\n");
                    return NULL;
                }
                printf("server sends twitterTrend response: (103,%d,\"%s\")\n", payload_len, payload);

                // send end of response
                if (-1 == send_msg(sockfd, END_OF_RESPONSE_ID, 0, NULL)) {
                    fprintf(stderr,
                            "ERROR, failed to send end of response message to "
                            "client\n");
                    return NULL;
                }
                printf("server sends end of response: (105,0,)\n");
            }
        }
        close(sockfd);
        printf("server closes the connection\n");
        printf("Thread %d finished handling client %s\n", thread_id, client_name);
    }
    return NULL;
}

void *process_queue_mt(void *_argv) {
    // parse _argv to a pointer to Queue_Process_Argv
    Queue_Process_Argv *argv = (Queue_Process_Argv *)_argv;

    Queue *queue = argv->queue;
    DB *twitter_db = argv->twitter_db;
    int thread_id = argv->thread_id;

    // call single thread version process_queue
    return process_queue(queue, twitter_db, thread_id);
}
