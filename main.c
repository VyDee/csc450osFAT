#include <stdio.h>
#include "sectors.h"
#include "FileTable.h"
#include "malloc.h"
// #include "Node.h"

typedef struct Node{
    char* name;
    int data;
    struct Node* next;
}Node;

void push(Node* head, int startBlock, char* txtname)
{
    if(head->next == NULL)
    {
        head->data = startBlock;
        head->name = txtname;
    }
    else{
        Node* current = head;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = (Node*) malloc(sizeof(Node));
        current->next->data = startBlock;
        current->next->name = txtname;
        current->next->next = NULL;

        printf("current->next->data %d \n", current->next->data);
        printf("current->next->name %s \n", current->next->name);
    }
}

int getData (Node* n)
{
    return n->data;
}
char* getName (Node* n)
{
    return n->name;
}

int main() {
    char textName[100];
    printf("Enter a file name: ");
    scanf("%s", textName);

    int bytes;
    printf("Enter number of bytes want to store: ");
    scanf("%d",&bytes);

    int sector = sectorCount(bytes);
    row = 20; //blockCount(sector)+2; 
    column = 3;
    int test[row][column];
    initializeArray(test);
    printf("Print array before \n");
    printFileTable(test);

    int start;

    //test[3][1] = 1;
    
    storeData(&start,blockCount(sector), test);
    printf("Print array after \n");
    printFileTable(test);
    printf("startBlock is %d \n", start);

    struct Node* head = NULL;
    head = (Node*) malloc(sizeof(head));
    head->next = NULL;
    push(head,start,textName);
    
    printf("Print head->data is %d \n", getData(head));
    printf("Print head->name is %s \n", getName(head));

    return 0;
}

