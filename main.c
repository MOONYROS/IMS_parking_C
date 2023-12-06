#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 3
#define COLUMNS 4

#define STEPS 60

#define MIN_TIME 10
#define MAX_TIME 20

int counter = 1;

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
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (!spots[i][j]) {
                spots[i][j] = true; // free space found

                // Vytvoření nového vozidla
                t_car* newCar = malloc(sizeof(t_car));
                newCar->row = i;
                newCar->col = j;
                newCar->arrivalTime = counter;
                newCar->stayDuration = randomBetween(MIN_TIME, MAX_TIME);
                newCar->next = NULL;

                if (head == NULL) {
                    head = newCar;
                }
                else {
                    t_car* current = head;
                    while (current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newCar;
                }

                return true; // car successfully added
            }
        }
    }
    return false; // parking lot full
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

void depart() {
    int row = rand() % ROWS;
    int col = rand() % COLUMNS;
    spots[row][col] = false; // random car departs
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
        if (rand() % 2) {
            arrive();
        }
//        else {
//            depart();
//        }
        updateCars();

        printStatus();
    }

    return 0;
}
