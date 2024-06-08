#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

void printFrames(int frames[], int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == -1) {
            printf("[ ] ");
        } else {
            printf("[%d] ", frames[i]);
        }
    }
    printf("\n");
}

int findOptimal(int pages[], int frames[], int num_frames, int num_pages, int current_index) {
    int farthest = current_index;
    int index = -1;
    for (int i = 0; i < num_frames; i++) {
        int j;
        for (j = current_index; j < num_pages; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    index = i;
                }
                break;
            }
        }
        if (j == num_pages) return i;
    }
    return (index == -1) ? 0 : index;
}

void fifo(int pages[], int num_pages, int num_frames) {
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int index = 0;

    for (int i = 0; i < num_frames; i++) frames[i] = -1;

    printf("\nFIFO Page Replacement:\n");

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            frames[index] = page;
            index = (index + 1) % num_frames;
            page_faults++;
            printFrames(frames, num_frames);
        }
    }

    printf("Total page faults using FIFO: %d\n", page_faults);
}

void lru(int pages[], int num_pages, int num_frames) {
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int page_faults = 0;

    for (int i = 0; i < num_frames; i++) frames[i] = -1;

    printf("\nLRU Page Replacement:\n");

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int found = 0;
        int lru_index = 0;

        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                last_used[j] = i;
                break;
            }
        }

        if (!found) {
            for (int j = 1; j < num_frames; j++) {
                if (last_used[j] < last_used[lru_index]) lru_index = j;
            }

            frames[lru_index] = page;
            last_used[lru_index] = i;
            page_faults++;
            printFrames(frames, num_frames);
        }
    }

    printf("Total page faults using LRU: %d\n", page_faults);
}

void optimal(int pages[], int num_pages, int num_frames) {
    int frames[MAX_FRAMES];
    int page_faults = 0;

    for (int i = 0; i < num_frames; i++) frames[i] = -1;

    printf("\nOptimal Page Replacement:\n");

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            if (frames[0] == -1) {
                frames[0] = page;
            } else {
                int index = findOptimal(pages, frames, num_frames, num_pages, i + 1);
                frames[index] = page;
            }
            page_faults++;
            printFrames(frames, num_frames);
        }
    }

    printf("Total page faults using Optimal: %d\n", page_faults);
}

int main() {
    int pages[MAX_PAGES];
    int num_pages, num_frames;

    printf("Enter number of pages: ");
    scanf("%d", &num_pages);

    printf("Enter the page reference sequence:\n");
    for (int i = 0; i < num_pages; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter number of frames: ");
    scanf("%d", &num_frames);

    fifo(pages, num_pages, num_frames);
    lru(pages, num_pages, num_frames);
    optimal(pages, num_pages, num_frames);

    return 0;
}
