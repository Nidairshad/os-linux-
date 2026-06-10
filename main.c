// main.c – Social Media Notification Scheduler
#include "scheduler.h"

int main(void)
{
    Process original[MAX_PROCESSES];
    Process working[MAX_PROCESSES];
    int n, quantum;

    // ── Dynamic Input ──────────────────────────────────────────────────
    input_processes(original, &n);

    printf("\nEnter Round Robin time quantum: ");
    scanf("%d", &quantum);
    if (quantum < 1) quantum = 2;

    // ── Run each algorithm (with full output) ──────────────────────────

    // 1. FCFS
    reset_processes(original, working, n);
    fcfs(working, n);

    // 2. SJF
    reset_processes(original, working, n);
    sjf(working, n);

    // 3. Round Robin
    reset_processes(original, working, n);
    round_robin(working, n, quantum);

    // 4. Priority
    reset_processes(original, working, n);
    priority_sched(working, n);

    // ── Comparison Report ─────────────────────────────────────────────
    compare_all(original, n, quantum);

    printf("\nDone. Thank you!\n\n");
    return 0;
}
