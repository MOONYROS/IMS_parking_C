#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 10
#define COLUMNS 20

#define STEPS 180

#define MIN_TIME 20
#define MAX_TIME 40

#define MIN_ATTEMPTS 1
#define MAX_ATTEMPTS 3

#define ENTRY_ROW 0
#define ENTRY_COL 10

#define LOW_FREQ 2
#define HIGH_FREQ 4

int counter = 1;

// STATISTICS
int createdCars = 0;
int parkedCars = 0;
int expiredCars = 0;

bool spots[ROWS][COLUMNS];

typedef struct car {
    int row, col;
    int arrivalTime;
    int stayDuration;
    int parkAttempts;
    int maxAttempts;
    bool parked;
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
    newCar->parkAttempts = 0;
    newCar->maxAttempts = randomBetween(MIN_ATTEMPTS, MAX_ATTEMPTS);
    newCar->parked = false;

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

void clearCarList() {
    t_car* current = head;
    while (current != NULL) {
        t_car* nextCar = current->next;
        free(current);
        current = nextCar;
    }
    head = NULL;
}

bool parkCar(t_car* car) {
    for (int i = 0; i < ROWS; i++) { // first we start with first row
        int closestDistanceInRow = COLUMNS + 1; // maximum distance
        int targetCol = -1;

        for (int j = 0; j < COLUMNS; j++) { // we search for a spot in a row
            if (spots[i][j] == false) { // if a parking spot is free, we calculate its distance from entrance
                int distance = abs(j - ENTRY_COL);
                if (distance < closestDistanceInRow) {
                    closestDistanceInRow = distance;
                    targetCol = j;
                }
            }
        }

        if (targetCol != -1) { // if we found a free spot, we occupy it
            spots[i][targetCol] = true;
            car->row = i;
            car->col = targetCol;
            car->parked = true;
            parkedCars++;
            return true;
        }
    }
    return false;
}

void attemptToPark() {
    t_car** current = &head;
    while (*current) {
        t_car* car = *current;

        if (car->parked == false) { // we try to park every car, that has not yet parked
            if (car->parkAttempts < car->maxAttempts) { // if the parking limit has not expired, we try to park
                if (parkCar(car) == true) { // if a car has managed to find a parking spot, we move to next car in list.
                    current = &(*current)->next;
                    continue;
                }
                else {
                   car->parkAttempts++; // if not, we increment attempts to park
                }
            }
            else { // if the parking limit expired, we remove the car from the list
                removeCar(car);
                expiredCars++;
                continue;
            }
        }
        else { // if the car is parked, we move on to the next car in the list
            current = &(*current)->next;
        }
    }
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
}

int main() {
    srand(time(NULL));
    initializeParkingLot();

    while (counter < STEPS) {
        for (int i = 0; i < randomBetween(LOW_FREQ, HIGH_FREQ); i++) {
            createCar();
            createdCars++;
        }
        attemptToPark();
        updateCars();

        if (counter % 10 == 0) {
            printStatus();
        }

        counter++;
    }

    printf("Number of parking spots: %d\n", ROWS * COLUMNS);
    printf("Created cars: %d\n", createdCars);
    printf("Expired cars: %d\n", expiredCars);
    printf("Parked cars: %d\n", parkedCars);

    clearCarList();
    return 0;
}
