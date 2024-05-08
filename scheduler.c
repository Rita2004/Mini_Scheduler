#include "scheduler.h"
#include "queue.h"
#include <stdio.h>
#include <string.h> 

#define CPU_CAPABILITY 20 

int load_workload(const char *filename, workload_item **workload_out, size_t *workload_size_out); // In workload.c

void run_scheduler(workload_item *workload, size_t workload_size) {
    node_t *running_queue = NULL;
    node_t *pending_queue = NULL;

    int max_timestep = 0;
    for (int i = 0; i < workload_size; i++) {
        if (workload[i].tf > max_timestep) {
            max_timestep = workload[i].tf;
        }
    }

    for (int t = 0; t <= max_timestep; t++) {
        printf("[t=%d]\n", t);

        // 1. Identify possible and current processes
        for (int i = 0; i < workload_size; i++) {
            if (workload[i].tf <= t) { 
                // Process is finished
                continue; 
            }
            if (workload[i].ts <= t && workload[i].tf > t) {
                // Process is current, add it to the pending queue if not already present
                if (!is_empty(running_queue) && peek(running_queue) != workload[i].pid) {   
                    enqueue(&pending_queue, workload[i].prio, workload[i].pid);
                } else if (is_empty(running_queue)){
                    enqueue(&pending_queue, workload[i].prio, workload[i].pid);
                }
            }
        }

        // 2. Schedule processes from pending to running queue
        int cpu_occupancy = 0;
        while (!is_empty(pending_queue) && cpu_occupancy < CPU_CAPABILITY) {
            int pid = dequeue(&pending_queue);
            int prio = 0; 
            for (int i = 0; i < workload_size; i++) {
                if (workload[i].pid == pid) {
                    prio = workload[i].prio;
                    break;
                }
            }
            if (cpu_occupancy + prio <= CPU_CAPABILITY) {
                enqueue(&running_queue, prio, pid);
                cpu_occupancy += prio;
                printf("  > schedule pid=%d prio=%d ('%s') ... added to running queue\n",
                       pid, prio, get_process_cmd(workload, workload_size, pid));
            } else { 
                enqueue(&pending_queue, prio, pid); 
                break; 
            }
        }

        // 3. Handle de-scheduling if the CPU is full
        while (!is_empty(running_queue) && cpu_occupancy > CPU_CAPABILITY) { 
            int pid = dequeue(&running_queue);
            int prio = 0;
            for (int i = 0; i < workload_size; i++) {
                if (workload[i].pid == pid) {
                    prio = workload[i].prio;
                    workload[i].idle++; // Increment idle time
                    workload[i].tf++;    // Delay estimated finish
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
}

// Helper to get a process's command name
char* get_process_cmd(workload_item* workload, size_t size, int pid) {
    for (int i = 0; i < size; i++) {
        if (workload[i].pid == pid) {
            return workload[i].cmd;
        }
    }
    return ""; 
}

// Helper to display the queues 
char* queue_to_string(node_t* queue) {
    // ... Implementation from previous response ...
}

int main(int argc, char *argv[]) {
    workload_item *workload = NULL;
    size_t workload_size = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <workload_file>\n", argv[0]);
        return 1;
    }

    if (load_workload(argv[1], &workload, &workload_size) != 0) {
        fprintf(stderr, "Error loading workload\n");
        return 1;
    }

    run_scheduler(workload, workload_size);

    // ... (Clean up memory) ... 
    return 0;
}