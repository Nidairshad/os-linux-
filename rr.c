// round_robin.c – Round Robin Scheduling
#include "scheduler.h"

void round_robin(Process proc[], int n, int quantum)
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║         Round Robin (Quantum = %2d)                   ║\n", quantum);
    printf("╚══════════════════════════════════════════════════════╝\n");

    // Work on copies of remaining burst
    int remaining[MAX_PROCESSES];
    int done[MAX_PROCESSES];
    for (int i = 0; i < n; i++) { remaining[i] = proc[i].burst; done[i] = 0; }

    GanttSlot gantt[MAX_GANTT];
    int g = 0, time = 0, idle_time = 0, completed = 0;
    int queue[MAX_PROCESSES * 100], qfront = 0, qrear = 0;
    int in_queue[MAX_PROCESSES];
    memset(in_queue, 0, sizeof(in_queue));

    // Sort by arrival to seed queue
    // We'll add processes to queue as they arrive
    // Start: enqueue all arrived at time 0
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival == 0) {
            queue[qrear++] = i;
            in_queue[i] = 1;
        }
    }

    while (completed < n) {
        if (qfront == qrear) {
            // Queue empty, CPU idle
            int next = INF;
            for (int i = 0; i < n; i++)
                if (!done[i] && proc[i].arrival < next) next = proc[i].arrival;
            if (next == INF) break;
            gantt[g++] = (GanttSlot){-1, time, next};
            idle_time += next - time;
            time = next;
            // Enqueue newly arrived
            for (int i = 0; i < n; i++)
                if (!done[i] && !in_queue[i] && proc[i].arrival <= time) {
                    queue[qrear++] = i;
                    in_queue[i] = 1;
                }
            continue;
        }

        int idx = queue[qfront++];
        Process *p = &proc[idx];

        // First time on CPU
        if (!p->started) {
            p->first_run = time;
            p->response  = time - p->arrival;
            p->started   = 1;
        }

        int run = (remaining[idx] < quantum) ? remaining[idx] : quantum;
        gantt[g++] = (GanttSlot){p->pid, time, time + run};
        time       += run;
        remaining[idx] -= run;

        // Enqueue processes that arrived during this slice
        for (int i = 0; i < n; i++)
            if (!done[i] && !in_queue[i] && proc[i].arrival <= time && i != idx)
                { queue[qrear++] = i; in_queue[i] = 1; }

        if (remaining[idx] == 0) {
            p->finish     = time;
            p->turnaround = p->finish - p->arrival;
            p->waiting    = p->turnaround - p->burst;
            done[idx]     = 1;
            in_queue[idx] = 0;
            completed++;
        } else {
            // Re-queue
            in_queue[idx] = 0;
            queue[qrear++] = idx;
            in_queue[idx]  = 1;
        }
    }

    print_gantt(gantt, g);
    print_table(proc, n);
    Summary s = compute_summary(proc, n, time, idle_time);
    print_summary(s);
}
