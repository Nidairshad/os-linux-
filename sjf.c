// sjf.c – Shortest Job First (Non-Preemptive)
#include "scheduler.h"

void sjf(Process proc[], int n)
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║         SJF – Shortest Job First (Non-Preemptive)    ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");

    int done[MAX_PROCESSES] = {0};
    GanttSlot gantt[MAX_GANTT];
    int g = 0, time = 0, idle_time = 0, completed = 0;

    while (completed < n) {
        // Find shortest available job
        int idx = -1, shortest = INF;
        for (int i = 0; i < n; i++) {
            if (!done[i] && proc[i].arrival <= time) {
                if (proc[i].burst < shortest ||
                   (proc[i].burst == shortest && proc[i].arrival < proc[idx].arrival)) {
                    shortest = proc[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // CPU idle: advance to next arrival
            int next = INF;
            for (int i = 0; i < n; i++)
                if (!done[i] && proc[i].arrival < next) next = proc[i].arrival;
            gantt[g++] = (GanttSlot){-1, time, next};
            idle_time += next - time;
            time = next;
            continue;
        }

        Process *p = &proc[idx];
        p->first_run  = time;
        p->response   = time - p->arrival;
        gantt[g++]    = (GanttSlot){p->pid, time, time + p->burst};
        time         += p->burst;
        p->finish     = time;
        p->turnaround = p->finish - p->arrival;
        p->waiting    = p->turnaround - p->burst;
        done[idx]     = 1;
        completed++;
    }

    print_gantt(gantt, g);
    print_table(proc, n);
    Summary s = compute_summary(proc, n, time, idle_time);
    print_summary(s);
}
