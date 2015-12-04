/**
 * This file contains declarations of some utility functions for twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Tue 03 Nov 2015 04:34:02 PM CST
 */

#ifndef SERVER_UTILITY_H
#define SERVER_UTILITY_H

#include "./queue.h"
#include "./twitter_db.h"
#include <pthread.h>

// argv for the queue_process function
typedef struct Queue_Process_Argv_t {
    Queue *queue;
    DB *twitter_db;
    int thread_id;
} Queue_Process_Argv;

/**
 * process the client queue using a single thread
 * @param  queue      address of a queue instance
 * @param  twitter_db address of a DB instance
 * @param  thread_id  id of this thread
 * @return            NULL
 */
void *process_queue(Queue *queue, DB *twitter_db, int thread_id);

/**
 * process the client queue, for multi-thread programming
 * @param  argv pointer to Queue_Process_Argv
 * @return      NULL
 */
void *process_queue_mt(void *_argv);

#endif  // SERVER_UTILITY_H
