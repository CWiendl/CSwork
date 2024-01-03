#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int ogNumOfChars[26] = {0}; // postion 0 are A's, postion 25 are Z's. Sets all elements to 0
    int newNumOfChars[26] = {0}; 
    char ogSign[100002]; //for null char & newline
    char newSign[100002];

    //input
    fgets(ogSign, 100002, stdin);
    fgets(newSign, 100002, stdin); //for null & newline

    int ogSignLen = strlen(ogSign);
    int newSignLen = strlen(newSign);

//frequency of each letter in each sign
    for(int i = 0; i < ogSignLen; i++) {
        if(ogSign[i] >= 65 && ogSign[i] <= 90) { //in ASCII 65 is A and 90 is Z
            ogNumOfChars[ogSign[i] - 65]++;
        }
    }

    for(int i = 0; i < newSignLen; i++) {
        if(newSign[i] >= 65 && newSign[i] <= 90) { 
            newNumOfChars[newSign[i] - 65]++;
        }
    }

    int newCharsNeeded = 0;
    //calculates the amount of letters we need 
    for(int i = 0; i < 26; i++) {
        if(newNumOfChars[i] > ogNumOfChars[i]) {
            newCharsNeeded = newCharsNeeded + (newNumOfChars[i] - ogNumOfChars[i]);
        }
        
    }

    printf("%d", newCharsNeeded);

    return 0;
}