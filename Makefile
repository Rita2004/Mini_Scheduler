# Name of your executable 
sched: scheduler.o workload.o queue.o output.o

# Compilation commands with flags. Adjust if needed.
scheduler.o: scheduler.c scheduler.h queue.h
    gcc -c -Wall scheduler.c

workload.o: workload.c scheduler.h
    gcc -c -Wall workload.c

queue.o: queue.c queue.h
    gcc -c -Wall queue.c

output.o: output.c scheduler.h
    gcc -c -Wall output.c

# Linking command
sched: scheduler.o workload.o queue.o output.o
    gcc -o sched scheduler.o workload.o queue.o output.o

# Clean target to remove object files and the executable
clean:
    rm -f *.o sched 
