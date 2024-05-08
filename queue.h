#ifndef QUEUE_H
#define QUEUE_H

#include "scheduler.h" 

typedef struct node_t {
    int prio; 
    int pid; 
    struct node_t *next;
} node_t;

node_t *create_node(int prio, int pid);
void enqueue(node_t **queue, int prio, int pid);
int dequeue(node_t **queue);  
int peek(node_t *queue); 
int is_empty(node_t *queue); 
char* queue_to_string(node_t* queue); 

#endif 
