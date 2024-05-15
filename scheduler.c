#include "scheduler.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // For INT_MAX

#define CPU_CAPABILITY 20

int load_workload(const char *filename, workload_item **workload_out, size_t *workload_size_out); // In workload.c

void run_scheduler(workload_item *workload, size_t workload_size)
{
    int cpu_occupancy = 0;
    node_t *running_queue = NULL;
    node_t *pending_queue = NULL;

    int max_timestep = 0;
    // Initialize the pending_queue with all workload items
    for (size_t i = 0; i < workload_size; i++) {
        enqueue(&pending_queue, workload[i].prio, workload[i].pid);
    }

    // Main scheduling loop
    while (!is_empty(pending_queue)) {
        // 1. Move processes from pending to running as long as there is CPU capability
        while (!is_empty(pending_queue) && cpu_occupancy < CPU_CAPABILITY) {
            int pid = dequeue(&pending_queue);
            int prio = 0;

            for (size_t i = 0; i < workload_size; i++) {
                if (workload[i].pid == pid) {
                    prio = workload[i].prio;
                    break;
                }
            }

            if (cpu_occupancy + prio <= CPU_CAPABILITY) {
                enqueue(&running_queue, prio, pid);
                cpu_occupancy += prio;
                printf("  > schedule pid=%d prio=%d ('%s') ... added to running queue\n", pid, prio, get_process_cmd(workload, workload_size, pid));
            } else {
                // Find the minimum priority in the running queue
                int min_prio = INT_MAX;
                node_t *min_prio_node = running_queue;
                node_t *current = running_queue;
                node_t *prev = NULL;
                node_t *min_prio_prev = NULL;

                while (current != NULL) {
                    if (current->prio < min_prio) {
                        min_prio = current->prio;
                        min_prio_node = current;
                        min_prio_prev = prev;
                    }
                    prev = current;
                    current = current->next;
                }

                if (min_prio < prio) {
                    // Replace the process with the minimum priority if it has a lower priority than the new process
                    int min_pid = min_prio_node->pid;
                    if (min_prio_prev == NULL) {
                        dequeue(&running_queue);
                    } else {
                        min_prio_prev->next = min_prio_node->next;
                        free(min_prio_node);
                    }
                    enqueue(&running_queue, prio, pid);
                    cpu_occupancy = cpu_occupancy - min_prio + prio;

                    for (size_t i = 0; i < workload_size; i++) {
                        if (workload[i].pid == min_pid) {
                            workload[i].idle++;
                            workload[i].tf++;
                            break;
                        }
                    }

                    enqueue(&pending_queue, min_prio, min_pid);
                    printf("  > schedule pid=%d prio=%d ('%s') ... added to running queue\n", pid, prio, get_process_cmd(workload, workload_size, pid));
                    printf("  > schedule pid=%d prio=%d ('%s') ... removed from running queue\n", min_pid, min_prio, get_process_cmd(workload, workload_size, min_pid));
                } else {
                    enqueue(&pending_queue, prio, pid); // Put back in pending
                    printf("  > schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: None, as min prio: pid=%d prio=%d ('%s') has greater priority\n",
                           pid, prio, get_process_cmd(workload, workload_size, pid), min_prio_node->pid, min_prio, get_process_cmd(workload, workload_size, min_prio_node->pid));
                    break;
                }
            }
        }
    }
    
    // Free the queues at the end
    while (!is_empty(running_queue))
    {
        dequeue(&running_queue);
    }
    while (!is_empty(pending_queue))
    {
        dequeue(&pending_queue);
    }
}

int contains_pid(node_t *queue, int pid)
{
    while (queue != NULL)
    {
        if (queue->pid == pid)
        {
            return 1; // Found
        }
        queue = queue->next;
    }
    return 0; // Not found
}

// Helper to get a process's command name
char *get_process_cmd(workload_item *workload, size_t size, int pid)
{
    for (int i = 0; i < size; i++)
    {
        if (workload[i].pid == pid)
        {
            return workload[i].cmd;
        }
    }
    return ""; // Process not found
}

int main()
{
    // Load the workload from a file
    workload_item *workload;
    size_t workload_size;
    if (load_workload("workload_1.txt", &workload, &workload_size) != 0)
    {
        fprintf(stderr, "Error loading workload.\n");
        return 1;
    }

    // Run the scheduler
    run_scheduler(workload, workload_size);

    // Free allocated memory
    for (size_t i = 0; i < workload_size; ++i)
    {
        free(workload[i].cmd);
    }
    free(workload);

    return 0;
}
