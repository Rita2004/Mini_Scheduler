#include "scheduler.h"
#include <stdio.h>
#include <string.h>

void print_chronogram(workload_item *workload, size_t workload_size)
{
    int max_tf = 0;
    for (int i = 0; i < workload_size; i++)
    {
        max_tf = (workload[i].tf > max_tf) ? workload[i].tf : max_tf; // Ternary operator for max_tf
    }

    printf("\nChronogram:\n");
    printf("     |"); // Initial timeline marker
    for (int t = 0; t <= max_tf; t++)
    {
        printf("....|");
    }
    printf("\n");

    for (int i = 0; i < workload_size; i++)
    {
        printf("%-5s |", workload[i].cmd); // Print process command name (left-aligned)

        for (int t = 0; t <= max_tf; t++)
        {
            printf("%-4c|", (t >= workload[i].ts && t <= workload[i].tf) ? 'X' : ' ');
        }

        // Cast size_t to int for printing using %d (if needed for your MinGW)
        printf(" (%d,%d)\n", (int)workload[i].tf, (int)workload[i].idle);
    }
}
