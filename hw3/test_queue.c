#include "./queue.h"
#include <stdlib.h>
#include <stdio.h>


int main() {
    Queue *queue = create_queue(5);
    push(queue, 1);
    push(queue, 2);
    push(queue, 3);
    push(queue, 4);
    push(queue, 5);

    int val[5];
    for (int i = 0; i != 5; i++) {
        pop(queue, val + i);
        printf("%d\n", val[i]);
    }

    pop(queue, val);

    push(queue, 5);
    push(queue, 4);
    push(queue, 3);
    push(queue, 2);
    push(queue, 1);

    for (int i = 0; i != 5; i++) {
        pop(queue, val + i);
        printf("%d\n", val[i]);
    }

    push(queue, 5);
    push(queue, 4);
    push(queue, 3);
    push(queue, 2);
    push(queue, 1);
    push(queue, 0);

    return 0;
}
