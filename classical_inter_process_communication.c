#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS];
sem_t mutex;

void think(int philosopher) {
    printf("Philosopher %d is thinking.\n", philosopher);
    sleep(1); // Simulate thinking
}

void eat(int philosopher) {
    printf("Philosopher %d is eating.\n", philosopher);
    sleep(2); // Simulate eating
}

void* philosopher(void* num) {
    int philosopher = *(int*)num;

    while (1) {
        think(philosopher);

        // Pick up forks
        sem_wait(&mutex);
        sem_wait(&forks[philosopher]);
        sem_wait(&forks[(philosopher + 1) % NUM_PHILOSOPHERS]);
        sem_post(&mutex);

        eat(philosopher);

        // Put down forks
        sem_post(&forks[philosopher]);
        sem_post(&forks[(philosopher + 1) % NUM_PHILOSOPHERS]);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize the semaphores
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for threads to finish (they won't in this example)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
