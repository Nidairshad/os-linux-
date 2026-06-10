// fcfs.c – First Come First Served scheduling
#include "scheduler.h"

void fcfs(Process proc[], int n)
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║         FCFS – First Come First Served               ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");

    // Sort by arrival time (stable insertion sort)
    for (int i = 1; i < n; i++) {
        Process key = proc[i];
        int j = i - 1;
        while (j >= 0 && proc[j].arrival > key.arrival) {
            proc[j + 1] = proc[j];
            j--;
        }
        proc[j + 1] = key;
    }

    GanttSlot gantt[MAX_GANTT];
    int g = 0, time = 0, idle_time = 0;

    for (int i = 0; i < n; i++) {
        Process *p = &proc[i];

        // CPU idle gap
        if (time < p->arrival) {
            gantt[g++] = (GanttSlot){-1, time, p->arrival};
            idle_time += p->arrival - time;
            time = p->arrival;
        }

        p->first_run  = time;
        p->response   = time - p->arrival;
        gantt[g++]    = (GanttSlot){p->pid, time, time + p->burst};
        time         += p->burst;
        p->finish     = time;
        p->turnaround = p->finish - p->arrival;
        p->waiting    = p->turnaround - p->burst;
    }

    print_gantt(gantt, g);
    print_table(proc, n);
    Summary s = compute_summary(proc, n, time, idle_time);
    print_summary(s);
}
