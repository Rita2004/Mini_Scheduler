#include "queue.h"
#include <string.h>
#include <stdlib.h>

// Create a new queue node
node_t *create_node(int prio, int pid) {
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation error!\n");
        exit(1); 
    }

    new_node->prio = prio;
    new_node->pid = pid;
    new_node->next = NULL;
    return new_node;
}

// Add a node to the end of the queue
void enqueue(node_t **queue, int prio, int pid) {
    node_t *new_node = create_node(prio, pid);

    if (*queue == NULL) {
        *queue = new_node;
    } else {
        node_t *current = *queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Remove and return the pid at the front of the queue
int dequeue(node_t **queue) {
    if (*queue == NULL) { 
        return -1; // Or handle the empty queue case differently
    }

    node_t *temp = *queue;
    int pid = temp->pid;
    *queue = (*queue)->next;
    free(temp);
    return pid;
}

// Get the pid at the front of the queue without removing 
int peek(node_t *queue) {
    if (queue == NULL) {
        return -1; // Or handle the empty queue case differently
    }
    return queue->pid;
}

// Check if the queue is empty
int is_empty(node_t *queue) {
    return queue == NULL;
}

// Helper function (optional) - For formatting the queue as a string
char* queue_to_string(node_t* queue) {
    char *str = malloc(100 * sizeof(char)); 
    str[0] = '\0'; 

    if (is_empty(queue)) {
        strcat(str, "[]");
    } else {
        strcat(str, "[");
        node_t *current = queue;
        while (current != NULL) {
            char buffer[20];
            sprintf(buffer, "(%d,%d)", current->prio, current->pid); 
            strcat(str, buffer);
            current = current->next;
            if (current != NULL) {
                strcat(str, ", ");
            }
        }
        strcat(str, "]");
    }
    return str;
}
