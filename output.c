#include "scheduler.h"
#include <stdio.h>
#include <string.h>

void print_chronogram(workload_item *workload, size_t workload_size) {
    int max_tf = 0;
    for (int i = 0; i < workload_size; i++) {
        if (workload[i].tf > max_tf) {
            max_tf = workload[i].tf;
        }
    }

    printf("\nChronogram:\n");
    printf("|"); // Initial timeline marker
    for (int t = 0; t <= max_tf; t++) {
        printf("....|"); 
    }
    printf("\n");

    for (int i = 0; i < workload_size; i++) {
        char *cmd = workload[i].cmd;
        printf("%s ", cmd);
        for (int t = 0; t <= max_tf; t++) {
            if (t >= workload[i].ts && t <= workload[i].tf) {
                printf("X"); 
            } else {
                printf(" "); 
            }
            printf("   |");
        }
        printf(" (%d,%d)\n", workload[i].tf, workload[i].idle); 
    }
}
