#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global vars
int numOfAttendees;
char attendees[10][20]; //array of names of people going to theater
int popcorn[10]; //stores if person has popcorn or not by index (if person one of attendees has popcorn popcorn[1] would = 1)
int badPairs[10][10] = { 0 }; //stores if person is a bad pair with another by index (if person one of attendees hates person 2 badPairs[1][2] and badBairs[2][1] would = 1, = 0 by default)

int isValidPermute(int currPermute[]) { //if valid return 1 else return 0;
    for(int i = 0; i < numOfAttendees; i++) {
        if(i == 0) {
            if(badPairs[currPermute[i]][currPermute[i + 1]]) return 0;
            if(!popcorn[currPermute[i]] && !popcorn[currPermute[i + 1]]) return 0;
            continue;
        }
        if(i == numOfAttendees - 1) {
            if(badPairs[currPermute[i]][currPermute[i - 1]]) return 0;
            if(!popcorn[currPermute[i]] && !popcorn[currPermute[i - 1]]) return 0;
            continue;
        }
        if(badPairs[currPermute[i]][currPermute[i - 1]] || badPairs[currPermute[i]][currPermute[i + 1]]) return 0;
        if(!popcorn[currPermute[i]] && !popcorn[currPermute[i - 1]] && !popcorn[currPermute[i + 1]]) return 0;
    }
    return 1;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//swaps value of start index w/ the index stored in currPermute[i], then calls its self but start index is + 1. 
//for example/flow chart: https://imgur.com/a/JVE0rYn
void permutate(int currPermute[], int startIndex) {
    if(startIndex == numOfAttendees -1) { //base case when a permutation is completed
        if(isValidPermute(currPermute)) {
            for(int i = 0; i < numOfAttendees; i++) {
                printf("%s\n", attendees[currPermute[i]]); //since input is in lexiographical order the first valid permutation will also be in correct order
            }
            exit(0); //this program done after finding first valid permutation
        }
        return;
    }

    for(int i = startIndex; i < numOfAttendees; i++) {
        swap(&currPermute[startIndex], &currPermute[i]);
        permutate(currPermute, startIndex + 1); //recursion
        swap(&currPermute[startIndex], &currPermute[i]);
    }
}

int main() {
    int numBadPairs;
    int currPermute[10];
    //Input
    scanf("%d %d", &numOfAttendees, &numBadPairs);
    for(int i = 0; i < numOfAttendees; i++) {
        scanf("%s %d", attendees[i], &popcorn[i]);
        currPermute[i] = i;
    }
    char bPName1[20], bPName2[20]; //BP = Bad pair
    int indexBP1 = -1, indexBP2 = -1;
    for(int i = 0; i < numBadPairs; i++) {
        scanf("%s %s", bPName1, bPName2);
        for(int j = 0; j < numOfAttendees; j++) {
            if(strcmp(bPName1, attendees[j]) == 0) indexBP1 = j;
            if(strcmp(bPName2, attendees[j]) == 0) indexBP2 = j;
        }
        badPairs[indexBP1][indexBP2] = 1;
        badPairs[indexBP2][indexBP1] = 1;
    }

    permutate(currPermute, 0);
    return 0;
}