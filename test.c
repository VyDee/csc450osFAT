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
    if(head == NULL)
    {
        head->data = startBlock;
        head->name = txtname;
        head->next =NULL;
    }

    Node* current = head;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = (Node*) malloc(sizeof(Node));
    current->next->data = startBlock;
    current->next->name = txtname;
    current->next->next = NULL;
}

int arr[][];
void runProgram()
{
    char text[100];
    printf("Enter a file name: ");
    scanf("%s", text);

    int bytes;
    printf("Enter number of bytes want to store: ");
    scanf("%d",&bytes);

    int sector = sectorCount(bytes);

    int start;
    storeData(&start,blockCount(sector),arr);

    struct Node* head = NULL;
    head = (Node*) malloc(sizeof(head));
    head->next = NULL;
    push(head,start,text);
    printf("Print head->data is %d \n", head->data);
    printf("Print head->name is %s \n", head->name);
}

int main() {
    // char textName[100];
    // printf("Enter a file name: ");
    // scanf("%s", textName);

    // int bytes;
    // printf("Enter number of bytes want to store: ");
    // scanf("%d",&bytes);

    // int sector = sectorCount(bytes);
    row = 20; //blockCount(sector)+2; 
    column = 3;
    int test[row][column];
    arr[][] = test;
    initializeArray(test);
    int rerun = 1;
    while(rerun == 1){
        printf("Print array before \n");
        printFileTable(test);
        runProgram(test);
        printf("Print array after \n");
        printFileTable(test);

        printf("Do you want to continue (type 1 or 0)? ");
        scanf("%d",rerun);
    }
    
    // int start;

    //test[3][1] = 1;
    
    // storeData(&start,blockCount(sector), test);
    
    // printf("startBlock is %d \n", start);

    // struct Node* head = NULL;
    // head = (Node*) malloc(sizeof(head));
    // head->next = NULL;
    // push(head,start,textName);
    // printf("Print head->data is %d \n", head->data);
    // printf("Print head->name is %s \n", head->name);

    return 0;
}

