// compare.c – Run all 4 algorithms and print a side-by-side comparison
#include "scheduler.h"

/* ── helpers ────────────────────────────────────────────────────────────── */
static int has_starvation(Process proc[], int n)
{
    // A simple heuristic: starvation if any process waits > 3× average burst
    double avg_burst = 0;
    for (int i = 0; i < n; i++) avg_burst += proc[i].burst;
    avg_burst /= n;
    for (int i = 0; i < n; i++)
        if (proc[i].waiting > 3 * avg_burst) return 1;
    return 0;
}

static double compute_fairness(Process proc[], int n)
{
    // Jain's fairness index on waiting times: 1.0 = perfectly fair
    double sum = 0, sum_sq = 0;
    for (int i = 0; i < n; i++) {
        sum    += proc[i].waiting;
        sum_sq += (double)proc[i].waiting * proc[i].waiting;
    }
    if (sum_sq == 0) return 1.0;
    return (sum * sum) / (n * sum_sq);
}

/* ── run one algorithm silently and return its summary ──────────────────── */
typedef enum { A_FCFS, A_SJF, A_RR, A_PRIO } Algo;

static Summary run_silent(Process original[], int n, int quantum, Algo algo,
                           Process out[])
{
    reset_processes(original, out, n);

    // Redirect stdout temporarily
    FILE *null = fopen("/dev/null", "w");
    FILE *real  = stdout;
    stdout = null;

    switch (algo) {
        case A_FCFS: fcfs(out, n);                  break;
        case A_SJF:  sjf(out, n);                   break;
        case A_RR:   round_robin(out, n, quantum);   break;
        case A_PRIO: priority_sched(out, n);         break;
    }

    fflush(stdout);
    fclose(null);
    stdout = real;

    // Compute total time = max finish
    int total = 0;
    for (int i = 0; i < n; i++)
        if (out[i].finish > total) total = out[i].finish;

    return compute_summary(out, n, total, 0 /* idle already reflected */);
}

/* ── main comparison function ───────────────────────────────────────────── */
void compare_all(Process original[], int n, int quantum)
{
    Process tmp[MAX_PROCESSES];
    Summary s[4];
    double  fairness[4];
    int     starvation[4];

    const char *names[4] = {"FCFS", "SJF", "Round Robin", "Priority"};
    Algo algos[4]        = {A_FCFS, A_SJF, A_RR, A_PRIO};

    for (int a = 0; a < 4; a++) {
        s[a]          = run_silent(original, n, quantum, algos[a], tmp);
        fairness[a]   = compute_fairness(tmp, n);
        starvation[a] = has_starvation(tmp, n);
    }

    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                   ALGORITHM COMPARISON REPORT                           ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");

    // Header
    printf("\n  %-20s %-14s %-14s %-14s %-14s\n",
           "Metric", names[0], names[1], names[2], names[3]);
    printf("  ");
    for (int i = 0; i < 76; i++) printf("-");
    printf("\n");

    printf("  %-20s %-14.2f %-14.2f %-14.2f %-14.2f\n",
           "Avg Waiting",
           s[0].avg_waiting, s[1].avg_waiting, s[2].avg_waiting, s[3].avg_waiting);

    printf("  %-20s %-14.2f %-14.2f %-14.2f %-14.2f\n",
           "Avg Turnaround",
           s[0].avg_turnaround, s[1].avg_turnaround,
           s[2].avg_turnaround, s[3].avg_turnaround);

    printf("  %-20s %-14.2f %-14.2f %-14.2f %-14.2f\n",
           "Avg Response",
           s[0].avg_response, s[1].avg_response,
           s[2].avg_response, s[3].avg_response);

    printf("  %-20s %-14.1f %-14.1f %-14.1f %-14.1f\n",
           "CPU Util (%)",
           s[0].cpu_utilization, s[1].cpu_utilization,
           s[2].cpu_utilization, s[3].cpu_utilization);

    printf("  %-20s %-14.4f %-14.4f %-14.4f %-14.4f\n",
           "Throughput",
           s[0].throughput, s[1].throughput,
           s[2].throughput, s[3].throughput);

    printf("  %-20s %-14.3f %-14.3f %-14.3f %-14.3f\n",
           "Fairness (0-1)",
           fairness[0], fairness[1], fairness[2], fairness[3]);

    printf("  %-20s %-14s %-14s %-14s %-14s\n",
           "Starvation Risk",
           starvation[0]?"YES":"NO",
           starvation[1]?"YES":"NO",
           starvation[2]?"YES":"NO",
           starvation[3]?"YES":"NO");

    printf("\n");

    /* ── Best algorithm per metric ─────────────────────────────────────── */
    printf("  ┌─────────────────────────────────────────────────────┐\n");
    printf("  │              BEST ALGORITHM PER METRIC              │\n");
    printf("  ├─────────────────────────────────────────────────────┤\n");

    // Find best (lowest) for waiting, turnaround, response
    double vals[4];

    // Waiting
    for (int i = 0; i < 4; i++) vals[i] = s[i].avg_waiting;
    int best = 0;
    for (int i = 1; i < 4; i++) if (vals[i] < vals[best]) best = i;
    printf("  │  Least Avg Waiting    : %-28s│\n", names[best]);

    // Turnaround
    for (int i = 0; i < 4; i++) vals[i] = s[i].avg_turnaround;
    best = 0;
    for (int i = 1; i < 4; i++) if (vals[i] < vals[best]) best = i;
    printf("  │  Least Avg Turnaround : %-28s│\n", names[best]);

    // Response
    for (int i = 0; i < 4; i++) vals[i] = s[i].avg_response;
    best = 0;
    for (int i = 1; i < 4; i++) if (vals[i] < vals[best]) best = i;
    printf("  │  Least Avg Response   : %-28s│\n", names[best]);

    // Fairness (highest)
    for (int i = 0; i < 4; i++) vals[i] = fairness[i];
    best = 0;
    for (int i = 1; i < 4; i++) if (vals[i] > vals[best]) best = i;
    printf("  │  Most Fair            : %-28s│\n", names[best]);

    // Throughput (highest)
    for (int i = 0; i < 4; i++) vals[i] = s[i].throughput;
    best = 0;
    for (int i = 1; i < 4; i++) if (vals[i] > vals[best]) best = i;
    printf("  │  Best Throughput      : %-28s│\n", names[best]);

    printf("  └─────────────────────────────────────────────────────┘\n");

    /* ── Qualitative analysis ─────────────────────────────────────────── */
    printf("\n  QUALITATIVE ANALYSIS (Social Media Context)\n");
    printf("  ─────────────────────────────────────────────\n");
    printf("  FCFS        : Simple, fair order, but long streams block messages.\n");
    printf("  SJF         : Best avg wait; notifications/msgs (short) finish fast.\n");
    printf("                Risk: uploads/streams (long) may starve.\n");
    printf("  Round Robin : Most fair; all tasks get timeslices. Good for live\n");
    printf("                streams & interactive notifications.\n");
    printf("  Priority    : Critical alerts handled first. Risk of low-priority\n");
    printf("                uploads starving if high-priority tasks keep arriving.\n");
}
