#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

int pages[MAX_PAGES];
int pageFrames[MAX_FRAMES];
int total_pages, frames;

// Prototypes
void FIFO();
void LRU();
void OPTIMAL();
int findPage(int page, int pageFrames[], int frames);

int findPage(int page, int pageFrames[], int frames) {
    for (int i = 0; i < frames; i++) {
        if (pageFrames[i] == page) {
            return i;
        }
    }
    return -1;
}

void initialize() {
    for (int i = 0; i < frames; i++) {
        pageFrames[i] = -1; // initialize page frames to -1 indicating they're empty
    }
}

void printPageFrames() {
    for (int i = 0; i < frames; i++) {
        if (pageFrames[i] != -1) {
            printf("%d ", pageFrames[i]);
        } else {
            printf("-- ");
        }
    }
    printf("\n");
}

void FIFO() {
    int next_replace = 0;
    int page_faults = 0;

    initialize();

    printf("\nFIFO Page Replacement:\n");

    for (int i = 0; i < total_pages; i++) {
        if (findPage(pages[i], pageFrames, frames) == -1) {
            pageFrames[next_replace] = pages[i];
            next_replace = (next_replace + 1) % frames;
            page_faults++;
            printPageFrames();
        }
    }
    printf("FIFO - Total Page Faults: %d\n", page_faults);
}

void LRU() {
    int page_faults = 0;
    int used[MAX_FRAMES] = {0}; // Used to keep track of usage

    initialize();

    printf("\nLRU Page Replacement:\n");

    for (int i = 0; i < total_pages; i++) {
        int found = findPage(pages[i], pageFrames, frames);
        if (found == -1) { // Page fault
            int lru = 0;
            for (int j = 1; j < frames; j++) {
                if (used[j] < used[lru]) {
                    lru = j;
                }
            }
            pageFrames[lru] = pages[i];
            page_faults++;
            printPageFrames();
        }
        for (int k = 0; k < frames; k++) {
            used[k]++; // Increment the use time of all frames
        }
        if (found != -1) {
            used[found] = 0; // Reset the frame that was just referenced
        }
    }
    printf("LRU - Total Page Faults: %d\n", page_faults);
}

void OPTIMAL() {
    int page_faults = 0;

    initialize();

    printf("\nOptimal Page Replacement:\n");

    for (int i = 0; i < total_pages; i++) {
        if (findPage(pages[i], pageFrames, frames) == -1) { // Page fault
            int opt_index = -1, farthest = i;
            for (int j = 0; j < frames; j++) {
                int k;
                for (k = i + 1; k < total_pages; k++) {
                    if (pageFrames[j] == pages[k]) {
                        if (k > farthest) {
                            farthest = k;
                            opt_index = j;
                        }
                        break;
                    }
                }
                if (k == total_pages) { // Not found in the future
                    opt_index = j;
                    break;
                }
            }
            if (opt_index == -1) {
                opt_index = 0; // If all are equally likely
            }
            pageFrames[opt_index] = pages[i];
            page_faults++;
            printPageFrames();
        }
    }
    printf("Optimal - Total Page Faults: %d\n", page_faults);
}

int main() {
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of pages: ");
    scanf("%d", &total_pages);
    printf("Enter page reference string:\n");
    for (int i = 0; i < total_pages; i++) {
        scanf("%d", &pages[i]);
    }
    FIFO();
    LRU();
    OPTIMAL();
    return 0;
}
