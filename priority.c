// priority.c – Priority Scheduling (Non-Preemptive, lower number = higher priority)
#include "scheduler.h"

void priority_sched(Process proc[], int n)
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║         Priority Scheduling (Non-Preemptive)         ║\n");
    printf("║         (Lower priority number = Higher priority)    ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");

    int done[MAX_PROCESSES] = {0};
    GanttSlot gantt[MAX_GANTT];
    int g = 0, time = 0, idle_time = 0, completed = 0;

    while (completed < n) {
        int idx = -1, best_pri = INF;

        for (int i = 0; i < n; i++) {
            if (!done[i] && proc[i].arrival <= time) {
                if (proc[i].priority < best_pri ||
                   (proc[i].priority == best_pri && proc[i].arrival < proc[idx].arrival)) {
                    best_pri = proc[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // CPU idle
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
