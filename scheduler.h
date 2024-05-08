#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h> // For file I/O
#include <stdlib.h> // For memory allocation

// Workload item struct
struct workload_item_t {
    int pid;       //< the event id
    int ppid;      //< the event parent id
    size_t ts;     //< start date
    size_t tf;     //< finish date
    size_t idle;   //< total time the process has been idle;
    char *cmd;     //< the binary name
    int prio;      //< process priority
};
typedef struct workload_item_t workload_item;

// Function declarations
int load_workload(const char *filename, workload_item **workload_out, size_t *workload_size_out);
void run_scheduler(workload_item *workload, size_t workload_size); 
void print_chronogram(workload_item *workload, size_t workload_size);

#endif // SCHEDULER_H
