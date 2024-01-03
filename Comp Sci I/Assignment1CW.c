#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ROWS 100000
#define MAX_RESERVATIONS 100000

struct Reservation {
int start, end; //Start and end of the reservation
char name[50]; //Name of the person who reserved this range
};

struct Row {
struct Reservation * resArray; //The array of reservations
int max_reservations; //the spots in the array
int num_reservations; //occupied spots in the array
};

struct Theater {
struct Row * row; //The array of rows in the theater
};

typedef struct Reservation Reservation;
typedef struct Row Row;
typedef struct Theater Theater;

int initializeRow(Theater *theater , int row) { //Return 1 for fail, 0 for success
    theater->row[row].max_reservations = MAX_RESERVATIONS; //max reservations in a row
    theater->row[row].num_reservations = 0; //num of reservations in a row
    theater->row[row].resArray = (Reservation *)malloc(MAX_RESERVATIONS * sizeof(Reservation));
    if(theater->row[row].resArray == NULL) {
        printf("mem alloc for reservation array failed");
        return 1;
    }
    return 0;
}

int makePurchase(Theater *theater, int row, int seatsStart, int seatsEnd, char name[]) { //Return 1 for fail, 0 for success
    //Defined just to shorten the following lines + efficiency
    int existingSeatsStart;
    int existingSeatsEnd;

    //following for a new row
    if(theater->row[row].resArray == NULL) {
        initializeRow(theater, row);
    }

    for(int i = 0; i < theater->row[row].num_reservations; i++) {
        existingSeatsStart = theater->row[row].resArray[i].start;
        existingSeatsEnd = theater->row[row].resArray[i].end;
        //if requested start of seats is contained in the range of an already existing reservation
        if(seatsStart >= existingSeatsStart && seatsEnd <= existingSeatsEnd) {
            printf("FAILURE\n");
            return 1;
        }
        //if requested end of seats is contained in the range of an already existing reservation
        if(seatsEnd >= existingSeatsStart && seatsEnd <= existingSeatsEnd) {
            printf("FAILURE\n");
            return 1;
        }
        //if range of requested start and end of seats contains an already existing reservation
        if(seatsStart <= existingSeatsStart && seatsEnd >= existingSeatsEnd) {
            printf("FAILURE\n");
            return 1;
        }
    }

    //following for a new reservation as long as its valid (i.e. no owns the seats)

    int temp_Num_Reservations = theater->row[row].num_reservations;
    theater->row[row].resArray[temp_Num_Reservations].start = seatsStart;
    theater->row[row].resArray[temp_Num_Reservations].end = seatsEnd;
    strcpy(theater->row[row].resArray[temp_Num_Reservations].name, name);
    theater->row[row].num_reservations++;
    printf("SUCCESS\n");
    return 0;
}

int lookup(Theater *theater, int row, int seat) { //LOOKUP function
    for(int i = 0; i < theater->row[row].num_reservations; i++) {
        //if given seat is within an existing reservation
        if(seat >= theater->row[row].resArray[i].start && seat <= theater->row[row].resArray[i].end) {
            printf("%s", theater->row[row].resArray[i].name);
            return 0;
        }
    }
    printf("No one\n");
    return 1;
}

void cleanUp(Theater *theater) {
    for (int i = 0; i < ROWS; i++) {
        if(theater->row[i].resArray != NULL){
            free(theater->row[i].resArray);
        }
    }
    free(theater->row);
    free(theater);
}

int menu(Theater *theater) {
    char input[100] = "";
    fgets(input, 100, stdin); //possible 50 char name + longest buy func
    char *command = strtok(input, " ");
    int row;
    int seatsStart;
    int seatsEnd;
    char name[50];

    if(strcmp(command, "BUY") == 0) { //if cmd (command variable) is BUY
        row = atoi(strtok(NULL, " "));
        seatsStart = atoi(strtok(NULL, " "));
        seatsEnd = atoi(strtok(NULL, " "));
        strcpy(name, strtok(NULL, " "));
        makePurchase(theater, row, seatsStart, seatsEnd, name);
        menu(theater);
    } 
    else if (strcmp(command, "LOOKUP") == 0){ //if cmd is LOOKUP
        row = atoi(strtok(NULL, " "));
        seatsStart = atoi(strtok(NULL, " "));
        lookup(theater, row, seatsStart);
        menu(theater);
    }
    else if (strcmp(command, "QUIT") == 0){ //if cmd is QUIT
        cleanUp(theater);
        exit(0);
    }
    return 1;
}

int main() {
    Theater theater;
    theater.row = (Row *)malloc(ROWS    q * sizeof(Row)); //Allocates memory for every row possible
    if(theater.row == NULL) {
        printf("mem alloc for row failed");
        return 1;
    }
    menu(&theater);
    return 1;
}