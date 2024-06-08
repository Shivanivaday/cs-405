#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Number of readers and writers
#define NUM_READERS 5
#define NUM_WRITERS 3

// Shared variables
int read_count = 0; // Number of readers currently reading
sem_t mutex;        // Semaphore to protect read_count
sem_t resource;     // Semaphore to protect the shared resource

void* reader(void* param) {
    int reader_id = *((int*)param);

    while (1) {
        // Entry section
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&resource); // First reader locks the resource
        }
        sem_post(&mutex);

        // Critical section
        printf("Reader %d is reading\n", reader_id);
        sleep(1); // Simulate reading time
        printf("Reader %d finished reading\n", reader_id);

        // Exit section
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&resource); // Last reader unlocks the resource
        }
        sem_post(&mutex);

        sleep(1); // Simulate time between reads
    }

    pthread_exit(0);
}

void* writer(void* param) {
    int writer_id = *((int*)param);

    while (1) {
        // Entry section
        sem_wait(&resource);

        // Critical section
        printf("Writer %d is writing\n", writer_id);
        sleep(2); // Simulate writing time
        printf("Writer %d finished writing\n", writer_id);

        // Exit section
        sem_post(&resource);

        sleep(1); // Simulate time between writes
    }

    pthread_exit(0);
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&resource, 0, 1);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for threads to finish (they won't in this example)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&resource);

    return 0;
}
