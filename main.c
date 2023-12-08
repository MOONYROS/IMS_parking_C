#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 10
#define COLUMNS 20

#define STEPS 180

#define MIN_TIME 20
#define MAX_TIME 40

#define ENTRY_ROW 0
#define ENTRY_COL 10

#define LOW_FREQ 2
#define HIGH_FREQ 4

#define ATTEMPTS 4

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

t_car* createCar() {
    t_car* newCar = malloc(sizeof(t_car));
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

    return newCar;
}

void removeCar(t_car* carToRemove) {
    if (head == NULL || carToRemove == NULL) {
        return;
    }

    if (head == carToRemove) {
        head = head->next;
        free(carToRemove);
        return;
    }

    t_car* current = head;
    while (current != NULL && current->next != carToRemove) {
        current = current->next;
    }

    if (current != NULL && current->next == carToRemove) {
        current->next = carToRemove->next;
        free(carToRemove);
    }
}

bool arrive() {
    for (int attempt = 0; attempt < ATTEMPTS; attempt++) { // 4 chances to find a parking spot
        for (int i = 0; i < ROWS; i++) { // search by rows
            int closestDistanceInRow = COLUMNS + 1; // setting the highest row distance
            int targetCol = -1;

            for (int j = 0; j < COLUMNS; j++) {
                if (!spots[i][j]) { // for each free column, we check how close it is
                    int distance = abs(j - ENTRY_COL);
                    if (distance < closestDistanceInRow) {
                        closestDistanceInRow = distance;
                        targetCol = j;
                    }
                }
            }

            // after finding the closest parking spot, we occupy it
            if (targetCol != -1) {
                spots[i][targetCol] = true;

                t_car* newCar = createCar();
                newCar->row = i;
                newCar->col = targetCol;

                return true;
            }
        }
        counter++;
    }

    return false;
}


void updateCars() {
    t_car** current = &head;
    while (*current) {
        t_car* car = *current;

        if (counter >= car->stayDuration + car->arrivalTime) {
            spots[car->row][car->col] = false;
            removeCar(car);
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
        for (int i = 0; i < randomBetween(LOW_FREQ, HIGH_FREQ); i++) {
            arrive();
        }
        updateCars();

        if (time % 10 == 0) {
            printStatus();
        }
    }

    return 0;
}
