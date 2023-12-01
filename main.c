#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 3
#define COLUMNS 4

#define STEPS 30

int counter = 1;

bool spots[ROWS][COLUMNS];

void initializeParkingLot() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            spots[i][j] = false; // intially all the spots are free
        }
    }
}

bool arrive() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (!spots[i][j]) {
                spots[i][j] = true; // free space found
                return true;
            }
        }
    }
    return false; // parking lot full
}

void depart() {
    int row = rand() % ROWS;
    int col = rand() % COLUMNS;
    spots[row][col] = false; // random car departs
}

// Výpis stavu parkoviště
void printStatus() {
    printf("\n-----KROK %d-----\n", counter);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%c ", spots[i][j] ? 'X' : '_');
        }
        printf("\n");
    }
    counter++;
}

int main() {
    srand(time(NULL));
    initializeParkingLot();

    for (int time = 0; time < STEPS; time++) {
        if (rand() % 2) { // chances are 50/50 to depart or arrive
            arrive();
        } else {
            depart();
        }

        printStatus();
    }

    return 0;
}
