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
#define ENTRY_COL 20

#define LOW_FREQ 2
#define HIGH_FREQ 4

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
    for (int attempt = 0; attempt < 4; attempt++) { // Pravidlo 3 a 4: Auto má 4 pokusy (minuty) na hledání místa
        for (int i = 0; i < ROWS; i++) { // Průchod po řadách, Pravidlo 2: Postupné procházení řad
            int closestDistanceInRow = COLUMNS + 1; // Nastavit na maximální možnou vzdálenost v řadě
            int targetCol = -1;

            for (int j = 0; j < COLUMNS; j++) {
                if (!spots[i][j]) {
                    int distance = abs(j - entryCol);
                    if (distance < closestDistanceInRow) {
                        closestDistanceInRow = distance;
                        targetCol = j;
                    }
                }
            }

            if (targetCol != -1) {
                spots[i][targetCol] = true; // Auto zaparkovalo

                // Vytvoření nového vozidla
                t_car* newCar = malloc(sizeof(t_car));
                newCar->row = i;
                newCar->col = targetCol;
                newCar->arrivalTime = counter;
                newCar->stayDuration = randomBetween(MIN_TIME, MAX_TIME);
                newCar->next = NULL;

                // Přidání auta na konec seznamu
                if (head == NULL) {
                    head = newCar;
                } else {
                    t_car* current = head;
                    while (current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newCar;
                }

                return true; // Auto úspěšně zaparkovalo
            }
        }
        counter++; // Přidání času na další pokus
    }

    return false; // Auto opouští systém po 4 pokusech
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
