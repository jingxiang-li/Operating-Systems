/**
 * This file contains declarations of a struct Queue for integers
 *
 * Author: Jingxiang Li
 * Date: Mon 02 Nov 2015 11:35:52 PM CST
 */

#ifndef QUEUE_H
#define QUEUE_H

/**
 * definition of Queue_Node which is the member of a Queue
 */
typedef struct Queue_Node_t {
    int val;
    struct Queue_Node_t *prev;
    struct Queue_Node_t *next;
} Queue_Node;

/**
 * definition of Queue
 */
typedef struct Queue_t {
    Queue_Node *front;
    Queue_Node *back;
    int size;
    int max_size;
} Queue;

/**
 * initialize a Queue instance
 * @param  max_size maximum size of the Queue
 * @return          pointer to the Queue instance on success;
 * otherwise NULL.
 */
Queue *create_queue(int max_size);

/**
 * free the Queue instance
 * @param  queue pointer to the Queue instance
 * @return       0 on success; otherwise -1.
 */
int free_queue(Queue *queue);

/**
 * give the size of the Queue instance
 * @param  queue pointer to the Queue instance
 * @return       the size of the Queue on success; otherwise -1.
 */
int size_of(Queue *queue);

/**
 * check if the queue is empty
 * @param  queue pointer to the Queue instance
 * @return       1 if it's empty, 0 if not, -1 on error.
 */
int is_empty(Queue *queue);

/**
 * check if the queue is full
 * @param  queue pointer to the Queue instance
 * @return       1 if it's full, 0 if not, -1 on error.
 */
int is_full(Queue *queue);

/**
 * add one integer to the Queue instance
 * @param  queue pointer to the Queue instance
 * @param  val   the value to be pushed to the Queue
 * @return       0 on success; otherwise -1.
 */
int push(Queue *queue, int val);

/**
 * write and remove the next integer from the Queue instance
 * @param  queue pointer to the Queue instance
 * @param  val   pointer of an integer to be written as the
 * poped integer value
 * @return       0 on success; otherwise -1.
 */
int pop(Queue *queue, int *val);

#endif  // QUEUE_H
