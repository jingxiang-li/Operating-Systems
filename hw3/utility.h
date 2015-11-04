/**
 * This file contains declarations of some utility functions for twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Tue 03 Nov 2015 04:34:02 PM CST
 */

#ifndef UTILITY_H
#define UTILITY_H

#include "./queue.h"
#include "./client_db.h"
#include "./twitter_db.h"
#include <pthread.h>

// argv for the queue_process function
typedef struct Queue_Process_Argv_t {
    Queue *queue;
    Client_DB *client_db;
    DB *twitter_db;
    int thread_id;
    int *num_clients_left;
} Queue_Process_Argv;

/**
 * change the currect working directory to the directory holding
 * the input file
 * @param  input_file_path path to the input file
 * @return                 0 on success; otherwise -1
 */
int change_wd(char *input_file_path);

/**
 * compile the output file for the client, named "client_name.result"
 * @param  client_name name of the client (file)
 * @param  city_name   name of the city
 * @param  keywords    keywords
 * @return             0 on success; -1 otherwise
 */
int compile_output(char *client_name, char *city_name, char *keywords);

/**
 * process the client queue using a single thread
 * @param  queue            address of a queue instance
 * @param  client_db        address of a Client_DB instance
 * @param  twitter_db       address of a DB instance
 * @param  thread_id        id of this thread
 * @param  num_clients_left address to an integer for number of clients
 * need to processed
 * @return                  NULL
 */
void *process_queue(Queue *queue, Client_DB *client_db, DB *twitter_db,
                    int thread_id, int *num_clients_left);

/**
 * process the client queue, for multi-thread programming
 * @param  argv pointer to Queue_Process_Argv
 * @return      NULL
 */
void *process_queue_mt(void *_argv);

#endif  // UTILITY_H
