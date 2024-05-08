sched: scheduler.o workload.o queue.o output.o
    gcc -o sched scheduler.o workload.o queue.o output.o

scheduler.o: scheduler.c scheduler.h queue.h
    gcc -c scheduler.c

workload.o: workload.c scheduler.h
    gcc -c workload.c

queue.o: queue.c queue.h
    gcc -c queue.c

output.o: output.c scheduler.h
    gcc -c output.c

clean:
    rm -f *.o sched
