CC = gcc
CFLAGS = -Wall -pedantic

# Object files
OBJS = scheduler.o queue.o workload.o output.o

# Build the executable
sched: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -mconsole

# Compile individual source files (with dependencies)
scheduler.o: scheduler.c scheduler.h queue.h workload.h 
	$(CC) $(CFLAGS) -c $<

queue.o: queue.c queue.h scheduler.h
	$(CC) $(CFLAGS) -c $<

workload.o: workload.c workload.h scheduler.h
	$(CC) $(CFLAGS) -c $<

output.o: output.c scheduler.h
	$(CC) $(CFLAGS) -c $<

# Clean up object files and the executable
clean:
	rm -f $(OBJS) sched

