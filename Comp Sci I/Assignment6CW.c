#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 19
#define TABLESIZE 300007

int totalMoney = 100000;
int totalComplexity = 0;

typedef struct item {
    char name[MAXLEN+1];
    int quantity;
    int saleprice;
} Item;

typedef struct node {
    Item* iPtr;
    struct node* next;
} Node;

typedef struct hashtable {
    Node** lists;
    int size;
} HashTable;

HashTable hashTable;

Node* llSearch(Node* current, char* item) { //LL search, NULL if not found.
    while (current != NULL) {
        if (strcmp(current->iPtr->name, item) == 0) {
            totalComplexity++;
            return current;
        }
        current = current->next;
        totalComplexity++;
    }
    return NULL;
}

int hashfunc(char* word, int size) {
    int len = strlen(word);
    int res = 0;
    for (int i=0; i<len; i++)
    res = (1151*res + (word[i]-'a'))%size;
    return res;
}

int buy(char* item, int quantity, int totalPrice) {
    int hashValue = hashfunc(item, hashTable.size);
    Node* current = hashTable.lists[hashValue];
    current = llSearch(current, item);
    if(current != NULL) {
        current->iPtr->quantity += quantity;
        totalMoney -= totalPrice;
        printf("%s %d %d\n", item, current->iPtr->quantity, totalMoney);
        return 0;
    }
    //if item not found
    Item* newItem = (Item*)malloc(sizeof(Item));
    strcpy(newItem->name, item);
    newItem->quantity = quantity;
    //insert into hashTable
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->iPtr = newItem;
    newNode->next = hashTable.lists[hashValue];
    hashTable.lists[hashValue] = newNode;
    //adjust totalMoney and totalComplexity
    totalMoney -= totalPrice;
    totalComplexity++;
    printf("%s %d %d\n", item, quantity, totalMoney);
    return 0;
}

int sell(char* item, int quantity) {
    int hashValue = hashfunc(item, hashTable.size);
    Node* current = hashTable.lists[hashValue];
    int amountSold;
    current = llSearch(current, item);
    if(current->iPtr->quantity <= quantity) { //if there isnt enough quantity in stock, sell what we have
        amountSold = current->iPtr->quantity;
        current->iPtr->quantity = 0;
    } else { //if enough in stock
        amountSold = quantity;
        current->iPtr->quantity -= quantity;
    }
    totalMoney += amountSold * current->iPtr->saleprice;
    printf("%s %d %d\n", item, current->iPtr->quantity, totalMoney);
    return 0;
}

int change_price(char* item, int newPrice) {
    int hashValue = hashfunc(item, hashTable.size);
    Node* current = hashTable.lists[hashValue];
    current = llSearch(current, item);
    current->iPtr->saleprice = newPrice;
    return 0;
}

int main() {
    int numOfCommands, quantity, salesPrice, newPrice, totalPrice;
    char command[15], item[MAXLEN+1];
    hashTable.lists = (Node**)malloc(TABLESIZE * sizeof(Node*));
    hashTable.size = TABLESIZE;
    for(int i = 0; i < TABLESIZE; i++) {
        hashTable.lists[i] = NULL;
    }
    scanf("%d", &numOfCommands);
    for(int i = 0; i < numOfCommands; i++) {
        scanf("%s", command);
        if(strcmp(command, "buy") == 0) { //buy command
            scanf("%s %d %d", item, &quantity, &totalPrice);
            buy(item, quantity, totalPrice);
        } else if(strcmp(command, "sell") == 0) { //sell command
            scanf("%s %d", item, &quantity);
            sell(item, quantity);
        } else if(strcmp(command, "change_price") == 0) { //change_price command
            scanf("%s %d", item, &newPrice);
            change_price(item, newPrice);
        }  
    }
    printf("%d\n%d", totalMoney, totalComplexity);
    for(int i = 0; i < TABLESIZE; i++) { //free everything
    Node* current = hashTable.lists[i];
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp->iPtr);
        free(temp);
    }
}
    free(hashTable.lists);
    return 0;
}