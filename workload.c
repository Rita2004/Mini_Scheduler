#include "workload.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_workload(const char *filename, workload_item **workload_out, size_t *workload_size_out) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening workload file '%s'\n", filename);
        return -1;
    }

    size_t capacity = 10; 
    workload_item *workload = malloc(capacity * sizeof(workload_item));
    if (workload == NULL) {
        fprintf(stderr, "Memory allocation error!\n");
        fclose(fp);
        return -1;
    }

    size_t count = 0;
    char line[100]; 
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *token = strtok(line, " \t\n"); 
        int i = 0;
        while (token != NULL) {
            switch (i) {
                case 0: workload[count].pid = atoi(token); break;
                case 1: workload[count].ppid = atoi(token); break;
                case 2: workload[count].ts = atoi(token); break;
                case 3: workload[count].tf = atoi(token); break;
                case 4: workload[count].idle = 0; break; 
                case 5: 
                    workload[count].cmd = malloc(strlen(token) + 1); 
                    strcpy(workload[count].cmd, token); 
                    break;
                case 6: workload[count].prio = atoi(token); break;
                default: 
                    fprintf(stderr, "Invalid workload file format\n");
                    fclose(fp);
                    free(workload);
                    return -1;
            }
            token = strtok(NULL, " \t\n");
            i++;
        }

        count++;
        if (count >= capacity) {
            capacity *= 2; 
            workload_item *temp = realloc(workload, capacity * sizeof(workload_item)); 
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation error!\n");
                fclose(fp);
                free(workload);
                return -1;
            }
            workload = temp;
        }
    }

    fclose(fp);
    *workload_out = workload;
    *workload_size_out = count;
    return 0; 
}
