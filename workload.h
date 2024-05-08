#ifndef WORKLOAD_H
#define WORKLOAD_H

#include "scheduler.h"  // For the workload_item struct

int load_workload(const char *filename, workload_item **workload_out, size_t *workload_size_out);

#endif

