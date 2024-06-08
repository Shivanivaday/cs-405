#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int num_processes, num_resources;

void initialize() {
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter number of resources: ");
    scanf("%d", &num_resources);

    printf("Enter available resources:\n");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &available[i]);
    }

    printf("Enter maximum resources for each process:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    printf("Enter allocation resources for each process:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

int isSafe() {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int safe_sequence[MAX_PROCESSES];
    int num_finished = 0;

    // Initialize work to available
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }

    while (num_finished < num_processes) {
        int found = 0;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == num_resources) {
                    // Process can be allocated
                    for (int k = 0; k < num_resources; k++) {
                        work[k] += allocation[i][k];
                    }
                    safe_sequence[num_finished++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            // Deadlock detected
            return 0;
        }
    }

    // Print safe sequence
    printf("Safe sequence: ");
    for (int i = 0; i < num_processes; i++) {
        printf("%d ", safe_sequence[i]);
    }
    printf("\n");

    return 1;
}

int requestResources(int process_num, int request[]) {
    // Check if request is valid
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > need[process_num][i] || request[i] > available[i]) {
            return 0;
        }
    }

    // Try allocating resources
    for (int i = 0; i < num_resources; i++) {
        available[i] -= request[i];
        allocation[process_num][i] += request[i];
        need[process_num][i] -= request[i];
    }

    // Check if the new state is safe
    if (!isSafe()) {
        // Undo allocation
        for (int i = 0; i < num_resources; i++) {
            available[i] += request[i];
            allocation[process_num][i] -= request[i];
            need[process_num][i] += request[i];
        }
        return 0;
    }

    return 1;
}

int main() {
    initialize();

    if (isSafe()) {
        printf("System is in a safe state.\n");
    } else {
        printf("System is in an unsafe state.\n");
        return 1;
    }

    int process_num;
    printf("Enter process number to request resources: ");
    scanf("%d", &process_num);

    int request[MAX_RESOURCES];
    printf("Enter resource request for process %d: ", process_num);
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &request[i]);
    }

    if (requestResources(process_num, request)) {
        printf("Request granted.\n");
    } else {
        printf("Request denied. System remains in a safe state.\n");
    }

    return 0;
}
