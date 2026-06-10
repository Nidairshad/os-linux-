// utils.c – shared helper functions
#include "scheduler.h"

const char *TASK_NAMES[] = {"Message", "Notification", "Upload", "LiveStream"};

/* ── Input ─────────────────────────────────────────────────────────────── */
void input_processes(Process proc[], int *n)
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║   Social Media Notification Scheduler – Input        ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");
    printf("Task types: 0=Message  1=Notification  2=Upload  3=LiveStream\n\n");

    printf("Enter number of tasks (1-%d): ", MAX_PROCESSES);
    scanf("%d", n);
    if (*n < 1 || *n > MAX_PROCESSES) { printf("Invalid count.\n"); exit(1); }

    for (int i = 0; i < *n; i++) {
        Process *p = &proc[i];
        p->pid     = i + 1;
        p->started = 0;

        printf("\n--- Task %d ---\n", p->pid);
        printf("  Name           : "); scanf("%31s", p->name);
        printf("  Type (0-3)     : "); scanf("%d", (int*)&p->type);
        if (p->type < 0 || p->type > 3) p->type = MSG;
        printf("  Arrival Time   : "); scanf("%d", &p->arrival);
        printf("  Burst Time     : "); scanf("%d", &p->burst);
        printf("  Priority (1=hi): "); scanf("%d", &p->priority);

        p->remaining = p->burst;
    }
}

/* ── Deep-copy process array and reset result fields ────────────────────── */
void reset_processes(Process src[], Process dst[], int n)
{
    for (int i = 0; i < n; i++) {
        dst[i]           = src[i];
        dst[i].remaining = src[i].burst;
        dst[i].finish    = 0;
        dst[i].waiting   = 0;
        dst[i].turnaround= 0;
        dst[i].response  = 0;
        dst[i].started   = 0;
        dst[i].first_run = 0;
    }
}

/* ── Gantt Chart ─────────────────────────────────────────────────────────── */
void print_gantt(GanttSlot gantt[], int g)
{
    printf("\n  Gantt Chart:\n  ");

    // Top border
    for (int i = 0; i < g; i++) {
        int width = gantt[i].end - gantt[i].start;
        if (width < 1) width = 1;
        for (int w = 0; w < width * 2 + 1; w++) printf("-");
    }
    printf("\n  |");

    // Process labels
    for (int i = 0; i < g; i++) {
        int width = gantt[i].end - gantt[i].start;
        if (width < 1) width = 1;
        if (gantt[i].pid == -1)
            printf(" IDLE%*s|", (width * 2 - 4) > 0 ? width * 2 - 4 : 0, "");
        else
            printf(" P%-*d|", width * 2 - 1, gantt[i].pid);
    }

    // Bottom border
    printf("\n  ");
    for (int i = 0; i < g; i++) {
        int width = gantt[i].end - gantt[i].start;
        if (width < 1) width = 1;
        for (int w = 0; w < width * 2 + 1; w++) printf("-");
    }

    // Time markers
    printf("\n  ");
    for (int i = 0; i < g; i++) {
        int width = gantt[i].end - gantt[i].start;
        if (width < 1) width = 1;
        printf("%-*d", width * 2 + 1, gantt[i].start);
    }
    printf("%d\n", gantt[g - 1].end);
}

/* ── Per-process result table ────────────────────────────────────────────── */
void print_table(Process proc[], int n)
{
    printf("\n  %-4s %-14s %-12s %-8s %-8s %-11s %-11s %-9s %s\n",
           "PID","Name","Type","Arrival","Burst","Finish","Turnaround","Waiting","Response");
    printf("  ");
    for (int i = 0; i < 88; i++) printf("-");
    printf("\n");

    for (int i = 0; i < n; i++) {
        Process *p = &proc[i];
        printf("  %-4d %-14s %-12s %-8d %-8d %-11d %-11d %-9d %d\n",
               p->pid, p->name, TASK_NAMES[p->type],
               p->arrival, p->burst, p->finish,
               p->turnaround, p->waiting, p->response);
    }
}

/* ── Compute averages ────────────────────────────────────────────────────── */
Summary compute_summary(Process proc[], int n, int total_time, int idle_time)
{
    Summary s = {0};
    for (int i = 0; i < n; i++) {
        s.avg_waiting    += proc[i].waiting;
        s.avg_turnaround += proc[i].turnaround;
        s.avg_response   += proc[i].response;
    }
    s.avg_waiting    /= n;
    s.avg_turnaround /= n;
    s.avg_response   /= n;
    s.cpu_utilization = total_time > 0
                        ? 100.0 * (total_time - idle_time) / total_time : 0;
    s.throughput      = total_time > 0 ? (double)n / total_time : 0;
    return s;
}

void print_summary(Summary s)
{
    printf("\n  Averages:\n");
    printf("    Waiting Time    : %.2f\n", s.avg_waiting);
    printf("    Turnaround Time : %.2f\n", s.avg_turnaround);
    printf("    Response Time   : %.2f\n", s.avg_response);
    printf("    CPU Utilization : %.1f%%\n", s.cpu_utilization);
    printf("    Throughput      : %.4f tasks/unit\n", s.throughput);
}
