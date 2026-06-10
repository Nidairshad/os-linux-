#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 20
#define MAX_GANTT     200
#define INF           99999

// Social media task types
typedef enum {
    MSG       = 0,
    NOTIF     = 1,
    UPLOAD    = 2,
    LIVESTREAM= 3
} TaskType;

extern const char *TASK_NAMES[];

// One process/task
typedef struct {
    int    pid;
    char   name[32];
    TaskType type;
    int    arrival;
    int    burst;
    int    priority;      // lower = higher priority
    int    remaining;     // for RR
    // results
    int    finish;
    int    waiting;
    int    turnaround;
    int    response;
    int    started;       // flag: has it been scheduled once?
    int    first_run;     // time it first got CPU
} Process;

// One Gantt slot
typedef struct {
    int pid;        // -1 = idle
    int start;
    int end;
} GanttSlot;

// Results summary
typedef struct {
    double avg_waiting;
    double avg_turnaround;
    double avg_response;
    double cpu_utilization;
    double throughput;
} Summary;

// Shared utilities (utils.c)
void  input_processes(Process proc[], int *n);
void  reset_processes(Process src[], Process dst[], int n);
void  print_gantt(GanttSlot gantt[], int g);
void  print_table(Process proc[], int n);
Summary compute_summary(Process proc[], int n, int total_time, int idle_time);
void  print_summary(Summary s);

// Algorithm functions
void fcfs(Process proc[], int n);
void sjf(Process proc[], int n);
void round_robin(Process proc[], int n, int quantum);
void priority_sched(Process proc[], int n);

// Comparison
void compare_all(Process original[], int n, int quantum);

#endif
