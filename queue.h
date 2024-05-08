#ifndef QUEUE_H
#define QUEUE_H

#include "scheduler.h" // Include the workload_item type 

// Node for our queue implementation 
typedef struct node_t {
    int prio; 
    int pid; 
    struct node_t *next;
} node_t;

// Could be implemented with more advanced techniques 
// if performance is critical

// Queue operations
node_t *create_node(int prio, int pid);
void enqueue(node_t **queue, int prio, int pid);
int dequeue(node_t **queue);  // Gets (and removes) the head element's pid 
int peek(node_t *queue); // Gets the head element's pid without removing
int is_empty(node_t *queue); 

#endif // QUEUE_H 
