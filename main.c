#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 10
#define COLUMNS 20

#define STEPS 120

#define MIN_TIME 20
#define MAX_TIME 40

#define ENTRY_ROW 0
#define ENTRY_COL 10

int counter = 1;

int entryRow = ENTRY_ROW;
int entryCol = ENTRY_COL;

bool spots[ROWS][COLUMNS];

typedef struct car {
    int row, col;
    int arrivalTime;
    int stayDuration;
    struct car* next;
} t_car;

t_car* head = NULL;

void initializeParkingLot() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            spots[i][j] = false; // intially all the spots are free
        }
    }
}

int randomBetween(int a, int b) {
    if (b < a) {
        int temp = a;
        a = b;
        b = temp;
    }
    return rand() % (b - a + 1) + a;
}

bool arrive() {
    int minDistance = ROWS * COLUMNS;
    int targetRow = -1, targetCol = -1;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (!spots[i][j]) {
                int distance = abs(i - entryRow) + abs(j - entryCol);
                if (distance < minDistance) {
                    minDistance = distance;
                    targetRow = i;
                    targetCol = j;
                }
            }
        }
    }

    if (targetRow != -1 && targetCol != -1) {
        spots[targetRow][targetCol] = true;

        // NEW CAR CREATION
        t_car* newCar = malloc(sizeof(t_car));
        newCar->row = targetRow;
        newCar->col = targetCol;
        newCar->arrivalTime = counter;
        newCar->stayDuration = randomBetween(MIN_TIME, MAX_TIME);
        newCar->next = NULL;

        if (head == NULL) {
            head = newCar;
        } else {
            t_car* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newCar;
        }

        return true;
    }

    return false;
}

void updateCars() {
    t_car** current = &head;
    while (*current) {
        t_car* car = *current;

        if (counter >= car->stayDuration + car->arrivalTime) {
            spots[car->row][car->col] = false;
            *current = car->next;
            free(car);
        }
        else {
            current = &(*current)->next;
        }
    }
}

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
        for (int i = 0; i < randomBetween(2, 4); i++) {
            arrive();
        }
        updateCars();

        if (time % 10 == 0) {
            printStatus();
        }
    }

    return 0;
}
