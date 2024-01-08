#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 19

typedef struct customer {
char name[MAXLEN + 1];
int points;
} Customer;

typedef struct treenode {
Customer * cPtr;
int size;
struct treenode * left;
struct treenode * right;
} Treenode;

int numOfNodes = 0;

Treenode* createNode(char* name, int points) {
    Treenode* node = (Treenode*)malloc(sizeof(Treenode));
    node->cPtr = (Customer*)malloc(sizeof(Customer));
    strcpy(node->cPtr->name, name);
    node->cPtr->points = points;
    node->left = NULL;
    node->right = NULL;
    node->size = 1;
    return node;
}

Treenode* addPointsRecursive(Treenode* root, char* name, int points) {
    if (root == NULL) {
        //insert a new node if the current node is NULL
        numOfNodes++;
        return createNode(name, points);
    }

    //decide whether to insert in the left or right subtree
    if (strcmp(name, root->cPtr->name) < 0) {
        root->left = addPointsRecursive(root->left, name, points);
    } else if (strcmp(name, root->cPtr->name) > 0) {
        root->right = addPointsRecursive(root->right, name, points);
    } else {
        //if the customer already exists, update the points
        root->cPtr->points += points;
        return root;
    }

    //update the size of the current node
    root->size = 1 + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);
    return root;
}

Treenode* searchCustomer(Treenode* root, char* name) {
    if (root == NULL || strcmp(name, root->cPtr->name) == 0) {
        return root;
    }
    if (strcmp(name, root->cPtr->name) < 0) {
        return searchCustomer(root->left, name);
    } else {
        return searchCustomer(root->right, name);
    }
}

void addPoints(Treenode** root, char* name, int points) { //if customer doesnt exist create new node w/ customer otherwise add points to existing one
    *root = addPointsRecursive(*root, name, points);
    Treenode* currNode = searchCustomer(*root, name);
    printf("%s %d\n", currNode->cPtr->name, currNode->cPtr->points);
}

void subPoints(Treenode* root, char* name, int points) {
    Treenode* currNode = searchCustomer(root, name);
    if(currNode == NULL) {
    printf("%s not found\n", name);
    return;
    } else {
        currNode->cPtr->points -= points;
        if(currNode->cPtr->points < 0) {
            currNode->cPtr->points = 0;
        }
    }
    printf("%s %d\n", currNode->cPtr->name, currNode->cPtr->points);
    return;
}

Treenode* findMaxNode(Treenode* node) {
    while (node && node->right != NULL) {
        node = node->right;
    }
    return node;
}

Treenode* deleteNodeRecursive(Treenode* root, char* name) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(name, root->cPtr->name) < 0) {
        root->left = deleteNodeRecursive(root->left, name);
    } else if (strcmp(name, root->cPtr->name) > 0) {
        root->right = deleteNodeRecursive(root->right, name);
    } else {
        //node with only one child or no child
        if (root->left == NULL) {
            Treenode* temp = root->right;
            free(root->cPtr);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Treenode* temp = root->left;
            free(root->cPtr);
            free(root);
            return temp;
        }

        //node with two children
        Treenode* temp = findMaxNode(root->left);

        //copy the data
        strcpy(root->cPtr->name, temp->cPtr->name);
        root->cPtr->points = temp->cPtr->points;

        //delete the inorder predecessor
        root->left = deleteNodeRecursive(root->left, temp->cPtr->name);
    }

    //update size
    root->size = 1 + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);
    return root;
}

void delCustomer(Treenode** root, char* name) {
    *root = deleteNodeRecursive(*root, name);
    numOfNodes--;
    printf("%s deleted\n", name);
    return;
}

int countSmaller(Treenode* root, char* name) {
    if (root == NULL) {
        return 0;
    }

    if (strcmp(name, root->cPtr->name) <= 0) {
        //name is greater than or equal to current node's name, go left
        return countSmaller(root->left, name);
    } else {
        //name is greater, count this node, its left subtree, and go right
        int leftSubtreeSize = (root->left != NULL) ? root->left->size : 0;
        return 1 + leftSubtreeSize + countSmaller(root->right, name);
    }
}

void freeTree(Treenode* root) {
    if(root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root -> right);
    free(root->cPtr);
    free(root);
    return;
}

void merge_sort(Treenode** arr, int n) {
    // Base case
    if (n <= 1) return;

    // Compute the number of half the array
    int n2 = n / 2;

    merge_sort(arr, n2);
    merge_sort(arr + n2, n - n2);
    Treenode** tmp = (Treenode **) malloc(sizeof(Treenode*) * n);
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
        } else if (arr[fptr]->cPtr->points > arr[bptr]->cPtr->points) {
            // front was smaller than the back
            tmp[i] = arr[fptr];
            fptr++;
        }else if (arr[fptr]->cPtr->points == arr[bptr]->cPtr->points && strcmp(arr[fptr]->cPtr->name, arr[bptr]->cPtr->name) < 0){ //if points are the same, go alphabetically by name
            tmp[i] = arr[fptr];
            fptr++;
        } 
        else {
            // back was good enough
            tmp[i] = arr[bptr];
            bptr++;
        }
    }

    //move the temp values into the orignal array
    for (int i = 0; i < n; i++) {
        arr[i] = tmp[i];
    }

    //remove the temp array
    free(tmp);
}

void treeToArray(Treenode *node, Treenode **array, int *index) {
    if (node != NULL) {
        treeToArray(node->left, array, index);
        array[*index] = node;
        (*index)++;
        treeToArray(node->right, array, index);
    }
}

int treeDepth(Treenode* root, char* name) {
    int depth = 0;
    Treenode* currNode = root;
    while(currNode != NULL){
        if (strcmp(name, currNode->cPtr->name) == 0) {
            return depth;
        }
        if (strcmp(name, currNode->cPtr->name) < 0) {
            currNode = currNode->left;
        } else {
            currNode = currNode->right;
        }
        depth++;
    }
    return -1;
}

int main() {
int numOfCommands;
char command[10], name[20];
int points;
Treenode* root = NULL;
Treenode* node;
scanf("%d", &numOfCommands);
int numOfCommands1 = numOfCommands;
for(int i = 0; i < numOfCommands1; i++) {
    scanf("%s", command);
    if (strcmp(command, "add") == 0) { //add cmd
        scanf("%s %d", name, &points);
        addPoints(&root, name, points);
    } else if (strcmp(command, "sub") == 0) { //sub cmd
        scanf("%s %d", name, &points);
        subPoints(root, name, points);
    } else if (strcmp(command, "del") == 0) { //del cmd
        scanf("%s", name);
        if(searchCustomer(root, name) == NULL) {
            printf("%s not found\n", name);
        } else {
        delCustomer(&root, name);
        }
    } else if (strcmp(command, "search") == 0) { //search cmd
        scanf("%s", name);
        node = searchCustomer(root, name);
        if(node == NULL) {
            printf("%s not found\n", name);
        } else {
        printf("%s %d %d\n", name, node->cPtr->points, treeDepth(root, name));
        }
    } else if (strcmp(command, "count_smaller") == 0) { //countSmaller cmd
        scanf("%s", name);
        printf("%d\n", countSmaller(root, name));
    } else {
        printf("Unknown command\n");
    }
    if(i > numOfCommands - 1) {
        break;
    }
}

Treenode **nodeArray = (Treenode **)malloc(numOfNodes * sizeof(Treenode *));
int index = 0;
treeToArray(root, nodeArray, &index);
merge_sort(nodeArray, numOfNodes);
for(int i = 0; i < numOfNodes; i++) {
    printf("%s %d\n", nodeArray[i]->cPtr->name, nodeArray[i]->cPtr->points);
}

free(nodeArray);
freeTree(root);
return 0;
}