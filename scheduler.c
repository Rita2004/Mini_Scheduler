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
    node_t *running_queue = NULL;
    node_t *pending_queue = NULL;

    int max_timestep = 0;
    for (int i = 0; i < workload_size; i++)
    {
        if (workload[i].tf > max_timestep)
        {
            max_timestep = workload[i].tf;
        }
    }

    for (int t = 0; t <= max_timestep; t++)
    {
        printf("[t=%d]\n", t);

        // 1. Identify possible and current processes
        for (int i = 0; i < workload_size; i++)
        {
            if (workload[i].tf <= t)
            {
                continue; // Process is finished
            }
            if (workload[i].ts <= t && workload[i].tf > t)
            {
                // Process is current, add it to the pending queue if not already present
                if (!contains_pid(running_queue, workload[i].pid) && !contains_pid(pending_queue, workload[i].pid))
                {
                    enqueue(&pending_queue, workload[i].prio, workload[i].pid);
                }
            }
        }

        // 2. Schedule processes from pending to running queue
        int cpu_occupancy = 0;
        while (!is_empty(pending_queue) && cpu_occupancy < CPU_CAPABILITY)
        {
            int pid = dequeue(&pending_queue);
            int prio = 0;

            for (int i = 0; i < workload_size; i++)
            {
                if (workload[i].pid == pid)
                {
                    prio = workload[i].prio;
                    break;
                }
            }

            if (cpu_occupancy + prio <= CPU_CAPABILITY)
            {
                enqueue(&running_queue, prio, pid);
                cpu_occupancy += prio;
                printf("  > schedule pid=%d prio=%d ('%s') ... added to running queue\n", pid, prio, get_process_cmd(workload, workload_size, pid));
            }
            else
            {
                enqueue(&pending_queue, prio, pid); // Put back in pending
                break;
            }
        }

        // 3. Handle de-scheduling if the CPU is full
        while (!is_empty(running_queue) && cpu_occupancy > CPU_CAPABILITY)
        {
            // Find the node with the lowest priority
            int min_prio = INT_MAX;
            node_t *pre = NULL;
            node_t *target_node = running_queue;

            while (target_node != NULL)
            {
                if (target_node->prio < min_prio)
                {
                    min_prio = target_node->prio;
                    pre = target_node;
                }
                target_node = target_node->next;
            }

            // Dequeue and free the node with the lowest priority
            int pid, prio;
            if (pre == NULL)
            { // Dequeue from the head
                pid = dequeue(&running_queue);
            }
            else
            {
                pid = pre->next->pid;
                prio = pre->next->prio;
                node_t *temp = pre->next;
                pre->next = pre->next->next;
                free(temp); // Free the memory
            }

            for (int i = 0; i < workload_size; i++)
            {
                if (workload[i].pid == pid)
                {
                    workload[i].idle++;
                    workload[i].tf++;
                    break;
                }
            }
            enqueue(&pending_queue, prio, pid);
            cpu_occupancy -= prio;

            printf("  > schedule pid=%d prio=%d ('%s') ... removed from running queue\n",
                   pid, prio, get_process_cmd(workload, workload_size, pid));
        }

        printf("  > CPU occupation: CPU[0]=%d \n", cpu_occupancy);
        printf("  > running: %s\n", queue_to_string(running_queue));
        printf("  > pending: %s\n", queue_to_string(pending_queue));
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
