/**
 * @file main.c
 * @brief Core file of this project that does all the work.
 * @author Ondrej Lukasek (xlukas15)
 * @date 2023-12-09
 * @copyright GNU GPL V3 LICENSE
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#define PRG_OK 0
#define PRG_ERROR 99

#define ARG_OK 0
#define ARG_HELP 1
#define ARG_ERROR 2
#define ARG_ROW_COL 3

#define DEF_ROWS 10
#define DEF_COLUMNS 20

#define STEPS 180

#define MIN_TIME 20
#define MAX_TIME 40

#define MIN_ATTEMPTS 1
#define MAX_ATTEMPTS 4

#define LOW_FREQ 3
#define HIGH_FREQ 4

#define DRIVER_SQUEEZER 1
#define DRIVER_FATHER 2
#define DRIVER_MOTHER 3

int counter = 1;

// GLOBAL VARIABLES
int global_rows = DEF_ROWS;
int global_cols = DEF_COLUMNS;
int global_steps = STEPS;
int global_entry;

// STATISTICS
int createdCars = 0;
int parkedCars = 0;
int expiredCars = 0;
int parkAttempts = 0;

bool **spots;

typedef struct car {
    int row, col;
    int arrivalTime;
    int stayDuration;
    int parkAttempts;
    int maxAttempts;
    int driverType;
    bool parked;
    struct car* next;
} t_car;

t_car* head = NULL;

/**
 * @Funtion that prints out hint about usage of this program.
 */
void printHelp() {
    printf("PARKING LOT CELLULAR AUTOMATON\n");
    printf("CREATED BY: Ondrej Lukasek (xlukas15)\n");
    printf("LICENSE: GNU GPL V3\n\n");
    printf("By default, this program is run with 10 rows and 20 columns.\n");
    printf("Number of steps is set to be 180.\n");
    printf("Entrance is set by default to be <number of columns> / 2.\n");
    printf("OPTIONS\n");
    printf("-h\n");
    printf("\tDisplays help on how to use this program on the standard output.\n");
    printf("-r <int number>\n");
    printf("\tSets the numbers of rows to be a specified number. ");
    printf("Needs to be used with -c <int number> option.\n");
    printf("-c <int number>\n");
    printf("\tSets the numbers of columns to be a specified number. ");
    printf("Needs to be used with -r <int number> option.\n");
    printf("-e <int number>\n");
    printf("\tSets the shopping mall entrance column to be a specified number. ");
    printf("-s <int number>\n");
    printf("\tSets the number of simulation steps to a specified number. Each step is represents 1 real-time minute.\n");
}

/**
 * @brief Function that parses the command line arguments.
 *
 * @param argc Argument count.
 * @param argv Array of arguments.
 * @return Numeral code based on ending state of the function.
 */
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

/**
 * Function to allocate memory for parking lot and initialization of parking lot.
 */
void initializeParkingLot() {
    spots = malloc(global_rows * sizeof(bool *));
    for (int i = 0; i < global_rows; i++) {
        spots[i] = malloc(global_cols * sizeof(bool));
    }

    for (int i = 0; i < global_rows; i++) {
        for (int j = 0; j < global_cols; j++) {
            spots[i][j] = false; // intially all the spots are free
        }
    }
}

/**
 * Function to free parking lot in memory.
 */
void clearParkingLot() {
    for (int i = 0; i < global_rows; i++) {
        free(spots[i]);
    }
    free(spots);
}

/**
 * @brief Function that will randomly select a number between 'a' and 'b'.
 *
 * @param a First number.
 * @param b Second number.
 * @return Random integer from interval (a, b).
 */
int randomBetween(int a, int b) {
    if (b < a) {
        int temp = a;
        a = b;
        b = temp;
    }
    return rand() % (b - a + 1) + a;
}

/**
 * @brief Function that will randomly decide (based on probability) type of car (behavior).
 *
 * @return Number of a car type.
 */
int getDriverType() {
    int probability = randomBetween(1, 10);
    if (probability > 0 && probability < 3) {
        return DRIVER_MOTHER;
    }
    else if (probability >= 3 && probability < 6) {
        return DRIVER_FATHER;
    }
    else {
        return DRIVER_SQUEEZER;
    }
}

/**
 * @brief Function that appends car to the end of the car list.
 *
 * @return Pointer to car structure.
 */
t_car* createCar() {
    t_car* newCar = malloc(sizeof(t_car));
    newCar->arrivalTime = counter;
    newCar->stayDuration = randomBetween(MIN_TIME, MAX_TIME);
    newCar->next = NULL;
    newCar->parkAttempts = 0;
    newCar->maxAttempts = randomBetween(MIN_ATTEMPTS, MAX_ATTEMPTS);
    newCar->parked = false;
    newCar->driverType = getDriverType();

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

/**
 * @brief Function that removes car from car list.
 *
 * @param carToRemove Pointer to car structure.
 */
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

/**
 * @brief Function that clears the list of cars.
 */
void clearCarList() {
    t_car* current = head;
    while (current != NULL) {
        t_car* nextCar = current->next;
        free(current);
        current = nextCar;
    }
    head = NULL;
}

/**
 * @brief Function that will assign a parking spot of a car.
 *
 * @param car Pointer to car structure.
 * @param row Parking row to occupy.
 * @param col Parking column to occupy.
 */
void assignSpot(t_car* car, int row, int col) {
    spots[row][col] = true;
    car->row = row;
    car->col = col;
    car->parked = true;
    parkedCars++;
}

/**
 * @brief Function that will park the car based on its parameters.
 *
 * @param car Pointer to a car structure.
 * @return True if parking was successful, false if unsuccessful.
 */
bool parkCar(t_car* car) {
    for (int i = 0; i < global_rows; i++) { // first we start with first row
        int minDistance = global_cols + 1; // maximum distance
        int targetCol = -1;

        for (int j = 0; j < global_cols; j++) { // we search for a spot in a row
            int distance = abs(j - global_entry);
            if (distance < minDistance) {
                if (car->driverType == DRIVER_SQUEEZER) {
                    if (spots[i][j] == false) {
                        minDistance = distance;
                        targetCol = j;
                    }
                }
                else if (car->driverType == DRIVER_FATHER) {
                    if (car->parkAttempts < car->maxAttempts) {
                        if (spots[i][j] == false &&
                            ((j + 1 < global_cols && spots[i][j + 1] == false) ||
                            (j - 1 >= 0 && spots[i][j - 1] == false))) {
                            minDistance = distance;
                            targetCol = j;
                        }
                    }
                    else {
                        if (spots[i][j] == false) {
                            minDistance = distance;
                            targetCol = j;
                        }
                    }
                }
                else if (car->driverType == DRIVER_MOTHER) {
                    if (j > 0 && j < global_cols - 1 &&
                        spots[i][j] == false &&
                        spots[i][j - 1] == false &&
                        spots[i][j + 1] == false) {
                        minDistance = distance;
                        targetCol = j;
                    }
                }
            }
        }

        if (targetCol != -1) { // if we found a free spot, we occupy it
            assignSpot(car, i, targetCol);
            return true;
        }
    }
    return false;
}

/**
 * @brief Function that will try to park a car and if the parking does not succeed,
 * it will increment parking attempts of the car.
 */
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
                   parkAttempts++;
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

/**
 * @brief Function that updates the car list by removing cars that have expired parking duration.
 */
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

/**
 * @brief Function that prints out current status of the program.
 */
void printStatus() {
    int occupied = 0;

    printf("\n-----STEP %d-----\n", counter);
    for (int i = 0; i < global_rows; i++) {
        for (int j = 0; j < global_cols; j++) {
            printf("%c ", spots[i][j] ? 'X' : '_');
            if (spots[i][j] == true) {
                occupied++;
            }
        }
        printf("\n");
    }
    printf("Occupied spots: %d\n", occupied);
}

/**
 * @brief Function that prints out stats on the standard output.
 */
void printStats() {
    printf("\n============== SIMULATION STATISTICS ==============\n");
    printf("Configuration: %15d rows %6d columns\n", global_rows, global_cols);
    printf("Number of parking spots: %6d\n", global_rows * global_cols);
    printf("Created cars: %17d\n", createdCars);
    printf("Expired cars: %17d\n", expiredCars);
    printf("Parked cars: %18d\n", parkedCars);
    printf("Average parking attempts:  %.2f\n", (float)parkAttempts/(float)createdCars);
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

    // intializing random number generator
    srand(time(NULL));

    initializeParkingLot();

    while (counter < global_steps + 1) {
        for (int i = 0; i < randomBetween(LOW_FREQ, HIGH_FREQ); i++) {
            createCar();
            createdCars++;
        }
        attemptToPark();
        updateCars();

        if (counter % 60 == 0) {
            printStatus();
        }

        counter++;
    }

    printStats();
    clearCarList();
    clearParkingLot();

    return PRG_OK;
}
