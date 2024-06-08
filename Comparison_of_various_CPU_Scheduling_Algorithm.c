#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burstTime;
    int priority;
    int arrivalTime;
    int waitingTime;
    int turnaroundTime;
} Process;

// Utility function to sort processes by burst time for SJF
int compareBurstTime(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->burstTime - p2->burstTime;
}

// Utility function to sort processes by priority
int comparePriority(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->priority - p2->priority;
}

// Function to calculate waiting time and turnaround time
void calculateMetrics(Process p[], int n) {
    int total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        p[i].turnaroundTime = p[i].burstTime + p[i].waitingTime;
        total_wt += p[i].waitingTime;
        total_tat += p[i].turnaroundTime;
    }
    printf("Average Waiting Time = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);
}

// First Come First Serve Algorithm
void FCFS(Process p[], int n) {
    printf("FCFS:\n");
    Process processes[n];
    for (int i = 0; i < n; i++) processes[i] = p[i];
    
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
    }
    calculateMetrics(processes, n);
}

// Shortest Job First Algorithm (non-preemptive)
void SJF(Process p[], int n) {
    printf("SJF:\n");
    Process processes[n];
    for (int i = 0; i < n; i++) processes[i] = p[i];
    
    qsort(processes, n, sizeof(Process), compareBurstTime);
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
    }
    calculateMetrics(processes, n);
}

// Priority Scheduling Algorithm (non-preemptive)
void PriorityScheduling(Process p[], int n) {
    printf("Priority:\n");
    Process processes[n];
    for (int i = 0; i < n; i++) processes[i] = p[i];
    
    qsort(processes, n, sizeof(Process), comparePriority);
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
    }
    calculateMetrics(processes, n);
}

// Round Robin Algorithm
void RoundRobin(Process p[], int n, int quantum) {
    printf("Round Robin:\n");
    Process processes[n];
    for (int i = 0; i < n; i++) processes[i] = p[i];
    
    int remainingTime[n];
    for (int i = 0; i < n; i++) {
        remainingTime[i] = processes[i].burstTime;
    }
    int currentTime = 0;
    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (remainingTime[i] > 0) {
                done = 0;
                if (remainingTime[i] > quantum) {
                    currentTime += quantum;
                    remainingTime[i] -= quantum;
                } else {
                    currentTime += remainingTime[i];
                    processes[i].waitingTime = currentTime - processes[i].burstTime - processes[i].arrivalTime;
                    remainingTime[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }
    calculateMetrics(processes, n);
}

int main() {
    Process processes[] = {{1, 6, 2, 1}, {2, 8, 3, 1}, {3, 7, 1, 2}, {4, 3, 4, 3}};
    int n = sizeof(processes) / sizeof(processes[0]);
    int quantum = 4;
    FCFS(processes, n);
    SJF(processes, n);
    PriorityScheduling(processes, n);
    RoundRobin(processes, n, quantum);
    return 0;
}
