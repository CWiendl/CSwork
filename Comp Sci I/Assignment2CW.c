#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_Qs 12

typedef struct Customer Customer;
typedef struct Node Node;
typedef struct Queue Queue;

struct Customer {
    char name[50];
    int numOfTix;
    int lineNum;
    int arriveTime;
};

struct Node {
    Customer* customer; 
    Node* next;
};

struct Queue {
    Node* head;
    Node* tail;
    int size;
};

//creates customer in system and fills in given values
Customer* createCustomer(char name[], int numOftix, int lineNum, int arriveTime) {
    Customer * customer = (Customer *) malloc(sizeof(Customer));
    strcpy(customer->name, name);
    customer->numOfTix = numOftix;
    customer->lineNum = lineNum;
    customer->arriveTime = arriveTime;
    return customer;
}

//for creating nodes for queue (a linked list)
Node* createNode(Customer* customer) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->customer = customer;
    newNode->next = NULL;
    return newNode;
}

//makes the queue it self
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->head = queue->tail = NULL;
    queue->size = 0;
    return queue;
}

//checks if queue is empty
int isEmpty(Queue* queue) {
    return queue->size == 0;
}

//adds a node to the tail of the queue
void enqueue(Queue* queue, Customer* customer) {
    Node* newNode = createNode(customer);
    if(isEmpty(queue)) {
        queue->head = queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size++;
}

//removes and reassigns head of queue - returns the customer that was removed
Customer* dequeue(Queue* queue) {
    if(isEmpty(queue)) {
        return NULL;
    }
    Node* temp = queue->head;
    Customer* customer = temp->customer;
    queue->head = queue->head->next;
    if(queue->head == NULL) {
        queue->tail = NULL;
    }
    free(temp);
    return customer;
}

//returns the head of the queue
Node* peek(Queue* queue) {
    if(isEmpty(queue)){
        return NULL;
    }
    return queue->head;
}

int main() {
    int booths;
    int numOfCust;
    scanf("%d %d", &numOfCust, &booths);
    Queue arrayOfQueues[NUM_OF_Qs]; //Have to sub 1 when accessing, queues are 1-12, this has index's 0 - 11
    for(int i = 0; i < 12; i++) { //Initalizes all queues into an array - will have to sub 1 when trying to access, queues are numbered 1-13
       arrayOfQueues[i] = *createQueue();
    } 
    //take in customers (lineNum = ASCII - 'A' of first letter of name % 13)
    char name[50];
    int numOfTix;
    int queueNum;
    int arriveTime;
    int numOfNonEmptyQs = 0;

    //find the number of nonempty (at least one cutomer ever) queues (k) and number of booths (b) and calculate how many queues go to each booth (k/b) (since int will be floored automatically)
    //find how many booths get one extra queue (k%b) and assign them to the first booths (queues 1, 2, 3 get 1 extra booth if k%b = 3)
    //ex. 3 booths (b), queues (Q) 1, 3, 4, 6, 10 are nonempty
    //b1 gets Qs 1, 3
    //b2 gets Q2 4, 6
    //b3 gets Q 10

    for(int i = 0; i < numOfCust; i++) {
        scanf("%s %d %d", &name, &numOfTix, &arriveTime);

        queueNum = ((name[0] - 'A') % 13) - 1; //have to sub 1; line 99 for info
        if(queueNum == -1){ //edge case of first cahr of name is A
            if(numOfNonEmptyQs == 0) { //edge case of 0 nonEmptyQs
                queueNum = 0;

            } else {
                int qWithLeastCusts = 0;
                int sizeOfLeastCustQ = 10000;
                for(int j = 0; j < NUM_OF_Qs; j++) { //figure out first nonEmptyQueue with least num of cust
                    if(arrayOfQueues[j].size < sizeOfLeastCustQ && !isEmpty(&arrayOfQueues[j])){
                        qWithLeastCusts = j;
                        sizeOfLeastCustQ = arrayOfQueues[j].size;
                    }
                }
                queueNum = qWithLeastCusts;
            }
        }
        if(isEmpty(&arrayOfQueues[queueNum])){
            numOfNonEmptyQs++;
        }
        enqueue(&arrayOfQueues[queueNum], createCustomer(name, numOfTix, queueNum, arriveTime));
    }

    int qsToEachBooth = numOfNonEmptyQs/booths; //k/b
    int numOfBoothsWithExtraQs = numOfNonEmptyQs%booths; //k%b
    int totalNumOfQsToCurrBooth;
    int currQ = 0;
    for(int i = 0; i < booths; i++) {
        printf("\nBooth %d\n", i + 1);
        totalNumOfQsToCurrBooth = qsToEachBooth;

        if(numOfBoothsWithExtraQs != 0) {
            totalNumOfQsToCurrBooth++;
            numOfBoothsWithExtraQs--;
        }
        int currBoothTime = 0;

        while(totalNumOfQsToCurrBooth > 0) {
            while(isEmpty(&arrayOfQueues[currQ])){
                currQ++;
            }
            Customer* currCust = dequeue(&arrayOfQueues[currQ]);
            if(currBoothTime < currCust->arriveTime) {
                currBoothTime = currCust->arriveTime;
            }
            currBoothTime += currCust->numOfTix * 5 + 30;
            printf("%s from line %d checks out at time %d\n", currCust->name, currCust->lineNum + 1, currBoothTime);
            if(arrayOfQueues[currQ].head == NULL) {
                totalNumOfQsToCurrBooth--;
                currQ++;
            }
            free(currCust);
        }
    }

    return 0;
}