#ifndef SCHEDULER_H
#define SCHEDULER_H
typedef struct node_t node_t;
#include <stdio.h>
#include <string.h>

typedef struct workload_item_t
{
    int pid;
    int ppid;
    size_t ts;
    size_t tf;
    size_t idle;
    char *cmd;
    int prio;
} workload_item;

int load_workload(const char *filename, workload_item **workload_out, size_t *workload_size_out);
void run_scheduler(workload_item *workload, size_t workload_size);
char *get_process_cmd(workload_item *workload, size_t size, int pid);
int contains_pid(node_t *queue, int pid);

#endif
