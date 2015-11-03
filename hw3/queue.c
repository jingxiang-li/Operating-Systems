/**
 * This file contains implementations for queue.h based on Deque
 *
 * Author: Jingxiang Li
 * Date: Mon 02 Nov 2015 11:54:11 PM CST
 */

#include <stdio.h>
#include <stdlib.h>
#include "./queue.h"

Queue *create_queue(int max_size) {
    if (max_size < 1) {
        fprintf(stderr,
                "Failed to create the Queue, max_size should be strictly "
                "larger than 0, not %d\n",
                max_size);
        return NULL;
    }

    Queue *queue = (Queue *)malloc(sizeof(Queue));

    if (NULL == queue) {
        perror("Failed to create the Queue");
        return NULL;
    }

    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
    queue->max_size = max_size;

    return queue;
}

int free_queue(Queue *queue) {
    if (NULL == queue) {
        fprintf(stderr,
                "Failed to free the Queue, illegal access to a NULL pointer\n");
        return -1;
    }

    // free each Queue_Node in the Queue
    if (NULL != queue->back) {
        Queue_Node *tmp_node = queue->back;
        queue->back = queue->back->next;
        free(tmp_node);
    }

    // free the Queue
    free(queue);

    return 0;
}

int size_of(Queue *queue) {
    if (NULL == queue) {
        fprintf(stderr,
                "Failed to free the Queue, illegal access to a NULL pointer\n");
        return -1;
    }

    return queue->size;
}

int is_empty(Queue *queue) {
    if (NULL == queue) {
        fprintf(stderr,
                "Failed to free the Queue, illegal access to a NULL pointer\n");
        return -1;
    }

    return 0 == queue->size;
}

int is_full(Queue *queue) {
    if (NULL == queue) {
        fprintf(stderr,
                "Failed to free the Queue, illegal access to a NULL pointer\n");
        return -1;
    }

    return queue->max_size == queue->size;
}

int push(Queue *queue, int val) {
    if (NULL == queue) {
        fprintf(stderr,
                "Failed to free the Queue, illegal access to a NULL pointer\n");
        return -1;
    }

    if (is_full(queue)) {
        printf("Failed to add value to the Queue, the Queue is full now\n");
        return -1;
    }

    Queue_Node *queue_node = (Queue_Node *)malloc(sizeof(Queue_Node));
    if (NULL == queue_node) {
        perror("Failed to create the queue_node");
        return -1;
    }

    // initialize Queue_Node
    queue_node->val = val;
    queue_node->next = queue->back;
    queue_node->prev = NULL;

    // modify Queue
    if (is_empty(queue)) {
        queue->back = queue_node;
        queue->front = queue_node;
    } else {
        queue->back->prev = queue_node;
        queue->back = queue_node;
    }

    (queue->size)++;

    return 0;
}

int pop(Queue *queue, int *val) {
    if (NULL == queue) {
        fprintf(stderr,
                "Failed to free the Queue, illegal access to a NULL pointer\n");
        return -1;
    }

    if (is_empty(queue)) {
        printf("Failed to pop value from the Queue, the Queue is empty now\n");
        return -1;
    }

    // write the next element to val
    *val = queue->front->val;

    // remove the front element from the Queue
    Queue_Node *tmp_node = queue->front;

    if (1 == size_of(queue)) {
        queue->front = NULL;
        queue->back = NULL;
    } else {
        queue->front = queue->front->prev;
        queue->front->next = NULL;
    }

    (queue->size)--;

    free(tmp_node);

    return 0;
}
