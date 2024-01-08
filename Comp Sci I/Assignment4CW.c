#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846

struct Group {
    double angle;
    int size;
};

typedef struct Group Group;

void merge_sort(Group * arr, int n) { //pointer to group array and elements in array
    // Base case
    if (n <= 1) return;

    // Compute the number of half the array
    int n2 = n / 2;

    merge_sort(arr, n2);
    merge_sort(arr + n2, n - n2);

    Group * tmp = (Group *) malloc(sizeof(Group) * n);
    int fptr = 0;  // front of the first half
    int bptr = n2; // front of the back half

    for (int i = 0; i < n; i++) {
        if (fptr == n2) {
            // front is empty
            tmp[i] = arr[bptr];
            bptr++;
        } else if (bptr == n) {
            // back is empty
            tmp[i] = arr[fptr];
            fptr++;
        } else if (arr[fptr].angle < arr[bptr].angle) {
            // front was smaller than the back
            tmp[i] = arr[fptr];
            fptr++;
        } else {
            // back was good enough
            tmp[i] = arr[bptr];
            bptr++;
        }
    }

    // Move the temp values into the orignal array
    for (int i = 0; i < n; i++) {
        arr[i] = tmp[i];
    }

    // Remove the temp array
    free(tmp);
}

int main() {
    int numOfGroups, ogAngle, totalPeople = 0;

    scanf("%d %d", &numOfGroups, &ogAngle); //input start

    Group* groups = (Group*)malloc(sizeof(Group) * numOfGroups);
    if (!groups) {
        printf("Failed to allocate memory\n");
        return 1;
    }

    for(int i = 0; i < numOfGroups; i++) {
        int x, y, size;
        scanf("%d %d %d", &x, &y, &size);
        groups[i].angle = atan2(x, y) * (180.0 / M_PI); //x, y instead of y,x cause atan2 goes off polar coords (starts to right of circle) when we want 360/0 degrees to start at top, converts to degrees from radians
        if(groups[i].angle < 0) {
            groups[i].angle += 360; // if angle is negative, will be equiv
        }
        totalPeople += size;
        groups[i].size = size;
    } //input end

    merge_sort(groups, numOfGroups); //sorts all groups based off angles

    int front = 0, back = 0; //find min people in projection with given angle start
    int minPeople = totalPeople; 

    int currentPeople = 0;
    while(front < numOfGroups) {
        while(front < numOfGroups && groups[front].angle - groups[back].angle <= ogAngle) { //if difference of angles less than angle required move front pointer forward till it isnt
            currentPeople += groups[front].size;
            front++;
        }

        minPeople = (currentPeople < minPeople) ? currentPeople : minPeople;

        while (groups[front].angle - groups[back].angle >= ogAngle) {
            if(back < numOfGroups - 1 && front < numOfGroups && back + 1 == front && groups[front].angle - groups[back].angle > ogAngle) { //if front and back are next to each other in the array and the difference b/w their angles is greater than the ogAngle, then min people = 0 
                minPeople = 0; 
            }
            currentPeople -= groups[back].size;
            back++;
        }
    }
    
    double leftOverAngle;
    int i;
    //consider before first group going backwards, omptimal angle b/w e.x. 330 and 30
    if(groups[0].angle < ogAngle) {
        leftOverAngle = ogAngle - groups[0].angle;
        i = 1;
        currentPeople = 0;
        while(groups[numOfGroups - i].angle > (360 - leftOverAngle) && i <= numOfGroups) {
            currentPeople += groups[i].size;
            i++;
        }
        minPeople = (currentPeople < minPeople) ? currentPeople : minPeople;
    }

    //consider after last group has least people going forwards, omptimal angle b/w e.x. 330 and 30
    double lastAngleTo360 = (360 - groups[numOfGroups - 1].angle);
    if(lastAngleTo360 < ogAngle) {
        leftOverAngle = ogAngle - lastAngleTo360;
        i = 0;
        currentPeople = 0;
        while(groups[i].angle < leftOverAngle && i < numOfGroups) {
            currentPeople += groups[i].size;
            i++;
        }
        minPeople = (currentPeople < minPeople) ? currentPeople : minPeople;
    } //find min people in projection with given angle end


    double maxAngle = 0.0; //find max angle start

    for(int i = 1; i < numOfGroups; i++) {
        double angleDiff = groups[i].angle - groups[i-1].angle;
        maxAngle = (angleDiff > maxAngle) ? angleDiff : maxAngle;
    }

    // Consider the case where the last and first group angles are the boundary
    double angleDiffBoundary = 360.0 - groups[numOfGroups - 1].angle + groups[0].angle;
    maxAngle = (angleDiffBoundary > maxAngle) ? angleDiffBoundary : maxAngle;
    if(maxAngle > ogAngle || numOfGroups == 1) { //if the maxangle is greater than og angle then there exist an angle where OgAngle has no one in projection
        minPeople = 0;
    }
    printf("%d\n", minPeople);
    printf("%.4lf", maxAngle); //find max angle end

    free(groups); // Free allocated memory

   return 0;
}