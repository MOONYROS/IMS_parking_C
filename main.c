#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define PRG_OK 0
#define PRG_ERROR 99

#define ARG_OK 0
#define ARG_HELP 1
#define ARG_ERROR 2
#define ARG_ROW_COL 3

#define ROWS 10
#define COLUMNS 20

#define STEPS 180

#define MIN_TIME 20
#define MAX_TIME 40

#define MIN_ATTEMPTS 1
#define MAX_ATTEMPTS 3

#define LOW_FREQ 2
#define HIGH_FREQ 4

int counter = 1;

// GLOBAL VARIABLES
int global_rows = ROWS;
int global_cols = COLUMNS;
int global_steps = STEPS;
int global_entry;

// STATISTICS
int createdCars = 0;
int parkedCars = 0;
int expiredCars = 0;

bool **spots;

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

void printHelp() {
    printf("THIS IS HELP FOR IMS PROJECT\n");
}

int parseArguments(int argc, char* argv[]) {
    int opt;
    char *endptr;

    bool rowsSet = false;
    bool colsSet = false;
    int entryCol = -1;

    while ((opt = getopt(argc, argv, "r:c:s:e:h")) != -1) {
        switch (opt) {
            case 'r':
                errno = 0;
                global_rows = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || errno != 0) {
                    fprintf(stderr, "Invalid number for -r: %s\n", optarg);
                    return ARG_ERROR;
                }
                else if (global_rows < 0) {
                    fprintf(stderr, "Invalid number input in -r argument: %d!\n", global_rows);
                    return ARG_ERROR;
                }
                rowsSet = true;
                break;
            case 'c':
                errno = 0;
                global_cols = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || errno != 0) {
                    fprintf(stderr, "Invalid number for -c: %s\n", optarg);
                    return ARG_ERROR;
                }
                else if (global_cols < 0) {
                    fprintf(stderr, "Invalid number input in -c argument: %d!\n", global_cols);
                    return ARG_ERROR;
                }
                colsSet = true;
                break;
            case 's':
                errno = 0;
                global_steps = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || errno != 0) {
                    fprintf(stderr, "Invalid number for -s: %s\n", optarg);
                    return ARG_ERROR;
                }
                else if (global_steps < 0) {
                    fprintf(stderr, "Invalid number input in -s argument: %d!\n", global_steps);
                    return ARG_ERROR;
                }
                break;
            case 'e':
                errno = 0;
                entryCol = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || errno != 0) {
                    fprintf(stderr, "Invalid number for -e: %s\n", optarg);
                    return ARG_ERROR;
                }
                else if (entryCol < 0) {
                    fprintf(stderr, "Invalid number input in -e argument: %d!\n", entryCol);
                    return ARG_ERROR;
                }
                break;
            case 'h':
                printHelp();
                return ARG_HELP;
            default:
                fprintf(stderr, "You've entered an unsupported argument.\n");
                fprintf(stderr, "To get information about this program, run it with the -h option.\n");
                return ARG_ERROR;
        }
    }

    if (entryCol != -1) { // if the user enetered entry
        if (entryCol > global_cols) { // and the entry is higher than the number of columns, we call error
            fprintf(stderr, "Entry number cannot be higher than number of columns: %d > %d!\n", entryCol, global_cols);
            return ARG_ERROR;
        }
        else { // otherwise we set it as entry
            global_entry = entryCol;
        }
    }
    else { // setting the entry to be in the middle of the top row in parking lot, if it was not set with -e
        global_entry = global_cols / 2;
    }
    if ((!rowsSet && !colsSet) || (rowsSet && colsSet)) {
        return ARG_OK;
    }
    else {
        fprintf(stderr, "You must specify both -r and -c options.\n");
        return ARG_ROW_COL;
    }
}

void initializeParkingLot() {
    for (int i = 0; i < global_rows; i++) {
        for (int j = 0; j < global_cols; j++) {
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
    for (int i = 0; i < global_rows; i++) { // first we start with first row
        int closestDistanceInRow = global_cols + 1; // maximum distance
        int targetCol = -1;

        for (int j = 0; j < global_cols; j++) { // we search for a spot in a row
            if (spots[i][j] == false) { // if a parking spot is free, we calculate its distance from entrance
                int distance = abs(j - global_entry);
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
    printf("\n-----STEP %d-----\n", counter);
    for (int i = 0; i < global_rows; i++) {
        for (int j = 0; j < global_cols; j++) {
            printf("%c ", spots[i][j] ? 'X' : '_');
        }
        printf("\n");
    }
}

void printStats() {
    printf("Number of parking spots: %d\n", global_rows * global_cols);
    printf("Created cars: %d\n", createdCars);
    printf("Expired cars: %d\n", expiredCars);
    printf("Parked cars: %d\n", parkedCars);
}

int main(int argc, char *argv[]) {
    switch (parseArguments(argc, argv)) {
        case ARG_HELP:
            return PRG_OK;
        case ARG_ROW_COL:
            return ARG_ROW_COL;
        case ARG_ERROR:
            return ARG_ERROR;
        case ARG_OK:
            break;
        default:
            fprintf(stderr, "Error while parsing arguments.\n");
            return PRG_ERROR;
    }

    spots = malloc(global_rows * sizeof(bool *));
    for (int i = 0; i < global_rows; i++) {
        spots[i] = malloc(global_cols * sizeof(bool));
    }

    // intializing random number generator
    srand(time(NULL));

    initializeParkingLot();

    while (counter < global_steps) {
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

    printStats();
    clearCarList();

    for (int i = 0; i < global_rows; i++) {
        free(spots[i]);
    }
    free(spots);

    return PRG_OK;
}
